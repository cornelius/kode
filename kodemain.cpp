/*
    This file is part of kdepim.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "code.h"
#include "license.h"
#include "printer.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QDate>
#include <QSettings>
#include <QTemporaryFile>

#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QRegExp>
#include <QTextStream>

#include <iostream>

void addPropertyFunctions(QString &out, const QString &type, const QString &name)
{
    // FIXME: Use KODE::Function

    bool isReference = type.endsWith("*") || type.endsWith("&");

    QString argument;
    QString upper = KODE::Style::upperFirst(name);
    if (!isReference) {
        argument = "const " + type + " &";
    } else
        argument = type;

    KODE::Code code;
    code.setIndent(4);
    code += "/**";
    code += "  Set .";
    code += "*/";
    code += "void set" + upper + "( " + argument + "v )";
    code += '{';
    code += "  m" + upper + " = v;";
    code += '}';

    code += "/**";
    code += "  Get " + name + ". See set" + upper + "().";
    code += "*/";
    code += type + (isReference ? "" : " ") + name + "() const";
    code += '{';
    code += "  return m" + upper + ';';
    code += '}';

    out += code.text();
}

void addPropertyVariable(QString &out, const QString &type, const QString &name)
{
    QString upper = KODE::Style::upperFirst(name);
    bool isReference = type.endsWith("*") || type.endsWith("&");

    KODE::Code code;
    code.setIndent(4);
    code += type + (isReference ? "" : " ") + 'm' + upper + ';';

    out += code.text();
}

// FIXME: Put addProperty in PropertyAdder class and add endReadAhead function.
int addProperty(QCommandLineParser *cmdLine)
{
    if (cmdLine->positionalArguments().count() != 2) {
        std::cerr << "Usage: kode --add-property <class> <proprerty-type> "
                  << "<property-name>" << std::endl;
        return 1;
    }

    QString className = cmdLine->value("add-property");
    QString type = cmdLine->positionalArguments().value(0);
    QString name = cmdLine->positionalArguments().value(1);

    qDebug() << "Add property: class" << className << ":" << type << name;

    QString headerFileName = className.toLower() + ".h";

    QFile headerFile(headerFileName);
    if (!headerFile.open(QIODevice::ReadOnly)) {
        std::cerr << "Unable to open file '" << headerFileName.data() /*.toUtf8()*/ << "'"
                  << std::endl;
        return 1;
    }

    QTextStream in(&headerFile);

    enum State { FindClass, FindConstructor, FindProperties, FindPrivate, FindVariables, Finish };
    State state = FindClass;

    QString accessor;
    QString mutator;

    QString out;

    QString readAhead;

    QString line;
    while (!(line = in.readLine()).isNull()) {
        qDebug() << int(state) << " LINE:" << line;
        QString readAheadPrevious = readAhead;
        readAhead += line + '\n';
        switch (state) {
        case FindClass:
            if (line.indexOf(QRegExp("^\\s*class\\s+" + className)) >= 0) {
                qDebug() << "  FOUND CLASS";
                state = FindConstructor;
            }
            break;
        case FindConstructor:
            if (line.indexOf(QRegExp("^\\s*" + className + "\\s*\\(")) >= 0) {
                qDebug() << "  FOUND CONSTRUCTOR";
                out += readAhead;
                readAhead.clear();
                state = FindProperties;
            }
            break;
        case FindProperties: {
            QRegExp re("(\\w+)\\s*\\(");
            if (re.indexIn(line) >= 0) {
                QString function = re.cap(1).toLower();
                if (!function.isEmpty()) {
                    qDebug() << "Function:" << function;
                    if (function == className || function == '~' + className) {
                        out += readAhead;
                        readAhead.clear();
                    } else {
                        if (function.startsWith("set")) {
                            mutator = function.mid(3);
                            qDebug() << "MUTATOR:" << mutator;
                        } else {
                            if (function == mutator) {
                                accessor = function;
                                qDebug() << "ACCESSOR:" << accessor;
                                out += readAhead;
                                readAhead.clear();
                            } else {
                                qDebug() << "CREATE PROPERTY";
                                out += readAheadPrevious;
                                addPropertyFunctions(out, type, name);
                                out += '\n';
                                readAhead = line + '\n';
                                state = FindPrivate;
                            }
                        }
                    }
                }
            } else if (line.indexOf(QRegExp("\\s*protected")) >= 0) {
                state = FindPrivate;
            } else if (line.indexOf(QRegExp("\\s*private")) >= 0) {
                if (accessor.isEmpty()) {
                    addPropertyFunctions(out, type, name);
                    out += readAhead;
                    readAhead.clear();
                    addPropertyVariable(out, type, name);
                    state = Finish;
                } else {
                    if (accessor == mutator) {
                        out += readAheadPrevious;
                        addPropertyFunctions(out, type, name);
                        out += '\n';
                        out += line + '\n';
                        readAhead.clear();
                    }
                    state = FindVariables;
                }
            }
        } break;
        case FindPrivate:
            if (line.indexOf(QRegExp("\\s*private")) >= 0) {
                if (accessor.isEmpty()) {
                    out += readAhead;
                    readAhead.clear();
                    addPropertyVariable(out, type, name);
                    state = Finish;
                } else {
                    state = FindVariables;
                }
            }
            break;
        case FindVariables: {
            if (line.indexOf('m' + accessor.toLower(), 0, Qt::CaseInsensitive) >= 0) {
                out += readAhead;
                readAhead.clear();
                addPropertyVariable(out, type, name);
                state = Finish;
            }
        } break;
        case Finish:
            break;
        }
    }

    headerFile.close();

    out += readAhead;

    if (cmdLine->isSet("inplace")) {
        QString headerFileNameOut = headerFileName + ".kodeorig";

        // XXX Why copy instead of rename?
        QFile::remove(headerFileNameOut);
        if (!QFile::copy(headerFileName, headerFileNameOut)) {
            qDebug() << "Copy failed";
        } else {
            qDebug() << "Write to original file.";
            if (!headerFile.open(QIODevice::WriteOnly)) {
                qDebug() << "Unable to open file '" << headerFileName << "' for writing.";
                return 1;
            }
            QTextStream o(&headerFile);
            o << out << endl;
        }
    } else {
        std::cout << out.data() /*.toUtf8()*/ << std::endl;
    }

    return 0;
}

int codify(QCommandLineParser *cmdLine)
{
    if (cmdLine->positionalArguments().count() != 1) {
        std::cerr << "Usage: kode --codify <sourcecodefile>" << std::endl;
        return 1;
    }

    QString filename = cmdLine->value("codify");

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to open file '" << filename << "'.";
        return 1;
    } else {
        std::cout << "KODE::Code code;" << std::endl;
        QTextStream ts(&f);
        QString line;
        while (!(line = ts.readLine()).isNull()) {
            line.replace("\\", "\\\\");
            line.replace("\"", "\\\"");
            line = "code += \"" + line;
            line.append("\";");
            std::cout << line.data() /*.toLocal8Bit()*/ << std::endl;
        }
    }

    return 0;
}

int create(QCommandLineParser *cmdLine)
{
    KODE::Printer p;
    if (cmdLine->isSet("warning"))
        p.setCreationWarning(true);

    bool createKioslave = cmdLine->isSet("create-kioslave");
    bool createMain = cmdLine->isSet("create-main");

    QString filename = cmdLine->value("filename");

    if (createMain) {
        if (filename.isEmpty()) {
            qDebug() << "Error: No file name given.";
            return 1;
        }

        if (filename.endsWith(".cpp")) {
            filename = filename.left(filename.length() - 4);
        }
    } else {
        if (!cmdLine->isSet("classname")) {
            qDebug() << "Error: No class name given.";
            return 1;
        }
    }

    QString className = cmdLine->value("classname");

    QString protocol;

    if (createKioslave) {
        if (!cmdLine->isSet("protocol")) {
            protocol = className.toLower();
            qWarning() << "Warning: No protocol for kioslave given. Assuming '" << protocol << "'";
        } else {
            protocol = cmdLine->value("protocol");
        }
    }

    KODE::File file;

    file.setProject(cmdLine->value("project"));

    QString authorEmail = cmdLine->value("author-email");
    QString authorName = cmdLine->value("author-name");
    if (!authorEmail.isEmpty()) {
        file.addCopyright(QDate::currentDate().year(), authorName, authorEmail);
    }

    KODE::License l;
    if (cmdLine->isSet("gpl"))
        l = KODE::License(KODE::License::GPL);
    if (cmdLine->isSet("lgpl"))
        l = KODE::License(KODE::License::LGPL);
    l.setQtException(cmdLine->isSet("qt-exception"));
    file.setLicense(l);

    file.setNameSpace(cmdLine->value("namespace"));

    if (createMain) {
        file.addInclude("kaboutdata.h");
        file.addInclude("kapplication.h");
        file.addInclude("qDebug");
        file.addInclude("klocale");
        file.addInclude("kcmdlinecmdLine");

        KODE::Function main("main", "int");
        main.addArgument("int argc");
        main.addArgument("char **argv");

        KODE::Code code;
        code += "KAboutData aboutData(\"test\",0,ki18n(\"Test\"),\"0.1\");";
        code += "KCmdLinecmdLine::init(argc,argv,&aboutData);";
        code += "";
        code += "KCmdLineOptions options;";
        code += "options.add(\"verbose\", ki18n(\"Verbose output\"));";
        code += "KCmdLinecmdLine::addCmdLineOptions( options );";
        code += "";
        code += "KApplication app;";
        code += "";
        code += "KCmdLinecmdLine *cmdLine = KCmdLinecmdLine::parsedcmdLine();";
        code += "";
        code += "Q_UNUSED( cmdLine );";
        main.setBody(code);

        file.addFileFunction(main);

        file.setImplementationFilename(filename);

        p.printImplementation(file, false);

        return 0;
    }

    KODE::Class c(className);

    if (cmdLine->isSet("create-dialog")) {
        c.addBaseClass(KODE::Class("KDialog"));
        c.addInclude("kdialog.h");
    } else if (createKioslave) {
        c.setDocs("This class implements a kioslave for ...");

        c.addBaseClass(KODE::Class("SlaveBase", "KIO"));
        c.addHeaderInclude("kio/slavebase.h");

        KODE::Function get("get", "void");
        get.addArgument("const KUrl &url");

        KODE::Code code;

        code += "qDebug(7000) << \"" + className + "::get()\";";
        code += "qDebug(7000) << \" URL: \" << url.url();";
        code += "#if 1";
        code += "qDebug(7000) << \" Path: \" << url.path();";
        code += "qDebug(7000) << \" Query: \" << url.query();";
        code += "qDebug(7000) << \" Protocol: \" << url.protocol();";
        code += "qDebug(7000) << \" Filename: \" << url.filename();";
        code += "#endif";
        code.newLine();

        code += "mimeType( \"text/plain\" );";
        code.newLine();

        code += "QCString str( \"Hello!\" );";
        code += "data( str );";
        code.newLine();

        code += "finished();";
        code.newLine();

        code += "qDebug(7000) << \"" + className + "CgiProtocol::get() done\";";

        get.setBody(code);

        c.addFunction(get);

        c.addInclude("kinstance.h");
        c.addInclude("qDebug.h");
        c.addInclude("sys/types.h");
        c.addInclude("unistd.h");
        c.addInclude("stdlib.h");

        KODE::Function main("kdemain", "int");
        main.addArgument("int argc");
        main.addArgument("char **argv");

        code.clear();

        code += "KComponentData instance( \"kio_" + protocol + "\" );";
        code += "";
        code += "qDebug(7000) << \"Starting kio_" + protocol + "(pid:  \" << getpid() << \")\";";
        code += "";
        code += "if (argc != 4) {";
        code.indent();
        code += "fprintf( stderr, \"Usage: kio_" + protocol
                + " protocol domain-socket1 domain-socket2\\n\");";
        code += "exit( -1 );";
        code.unindent();
        code += '}';
        code += "";
        code += className + " slave( argv[2], argv[3] );";
        code += "slave.dispatchLoop();";
        code += "";
        code += "return 0;";

        main.setBody(code);

        file.addFileFunction(main);

        file.addExternCDeclaration(p.functionSignature(main));
    }

    KODE::Function constructor(className);

    if (cmdLine->isSet("singleton")) {
        constructor.setAccess(KODE::Function::Private);

        KODE::Function self("self", className + " *");
        self.setStatic(true);

        KODE::Code code;
        code += "if ( !mSelf ) {";
        code += "  selfDeleter.setObject( mSelf, new " + className + "() );";
        code += '}';
        code += "return mSelf;";

        self.setBody(code);

        c.addFunction(self);

        KODE::MemberVariable selfVar("mSelf", className + " *", true);
        selfVar.setInitializer("0");

        c.addMemberVariable(selfVar);

        KODE::Variable staticDeleter("selfDeleter", "K3StaticDeleter<" + className + '>', true);
        file.addFileVariable(staticDeleter);
        file.addInclude("k3staticdeleter.h");
    }

    if (createKioslave) {
        constructor.addArgument("const QCString &pool");
        constructor.addArgument("const QCString &app");

        constructor.addInitializer("SlaveBase( \"" + protocol + "\", pool, app )");
    }

    c.addFunction(constructor);

    file.insertClass(c);

    p.printHeader(file);
    p.printImplementation(file);

    if (createKioslave) {
#if 0 // TODO: port to CMake
    // Write automake Makefile
    KODE::AutoMakefile am;

    am.addEntry( "INCLUDES", "$(all_includes)" );
    am.newLine();
    am.addEntry( "noinst_HEADERS", className.toLower() + ".h" );
    am.newLine();
    am.addEntry( "METASOURCES", "AUTO" );
    am.newLine();
    am.addEntry( "kdelnkdir", "$(kde_servicesdir)" );
    am.addEntry( "kdelnk_DATA", protocol + ".protocol" );

    KODE::AutoMakefile::Target t( "kde_module_LTLIBRARIES",
      "kio_" + protocol + ".la" );
    t.setSources( className.toLower() + ".cpp" );
    t.setLibAdd( "$(LIB_KIO)" );
    t.setLdFlags( "$(all_libraries) -module $(KDE_PLUGIN)" );

    am.addTarget( t );

    p.printAutoMakefile( am );
#endif

        // Write protocol file
        QString protocolFilename = protocol + ".protocol";

        QFileInfo fi(protocolFilename);
        protocolFilename = fi.absoluteFilePath();

        QFile protocolFile(protocolFilename);
        if (protocolFile.copy(protocolFilename + ".backup")) {
            QFile::remove(protocolFilename);

            QSettings protocolFileSettings(protocolFilename, QSettings::IniFormat);

            protocolFileSettings.beginGroup("Protocol");
            protocolFileSettings.setValue("exec", "kio_" + protocol);
            protocolFileSettings.setValue("protocol", protocol);
            protocolFileSettings.setValue("input", "none");
            protocolFileSettings.setValue("output", "filesystem");
            protocolFileSettings.setValue("reading", "true");
            protocolFileSettings.setValue("DocPath", "kioslave/" + protocol + ".html");
            protocolFileSettings.endGroup();
            protocolFileSettings.sync();
        } else {
            qDebug() << "Unable to crete the" << protocolFilename + ".backup"
                     << " file";
            return -1;
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("kode");
    QCoreApplication::setOrganizationName("KDE Code Generator");
    QCoreApplication::setApplicationName("0.1");

    QCommandLineParser cmdLine;
    cmdLine.setApplicationDescription("KDE Code Generator");
    cmdLine.addHelpOption();
    cmdLine.addVersionOption();

    QCommandLineOption createClass(QStringList() << "c"
                                                 << "create-class",
                                   QCoreApplication::translate("main", "Create class"));
    cmdLine.addOption(createClass);

    QCommandLineOption createDialog(QStringList() << "d"
                                                  << "create-dialog",
                                    QCoreApplication::translate("main", "Create dialog"));
    cmdLine.addOption(createDialog);

    QCommandLineOption createKIOSlave("create-kioslave",
                                      QCoreApplication::translate("main", "Create kioslave"));
    cmdLine.addOption(createKIOSlave);

    QCommandLineOption createMain(
            QStringList() << "d"
                          << "create-main",
            QCoreApplication::translate("main", "Create main function template"), "yes", "y");
    cmdLine.addOption(createMain);

    QCommandLineOption codifyOption(
            "codify",
            QCoreApplication::translate("main", "Create generator code for given source"));
    cmdLine.addOption(codifyOption);

    QCommandLineOption addPropertyOption(
            "add-property", QCoreApplication::translate("main", "Add property to class"));
    cmdLine.addOption(addPropertyOption);

    QCommandLineOption inplace("inplace",
                               QCoreApplication::translate("main", "Change file in place"));
    cmdLine.addOption(inplace);

    QCommandLineOption authorMail(
            "author-email",
            QCoreApplication::translate("main", "Add author with given email address"), "name");
    cmdLine.addOption(authorMail);

    QCommandLineOption project("project", QCoreApplication::translate("main", "Name of project"),
                               "name");
    cmdLine.addOption(authorMail);

    QCommandLineOption gpl("gpl", QCoreApplication::translate("main", "Use GPL as license"));
    cmdLine.addOption(gpl);

    QCommandLineOption className("classname", QCoreApplication::translate("main", "Name of class"),
                                 "name");
    cmdLine.addOption(className);

    QCommandLineOption fileName("filename", QCoreApplication::translate("main", "Name of file"),
                                "name");
    cmdLine.addOption(fileName);

    QCommandLineOption namespaceName("namespace", QCoreApplication::translate("main", "Namespace"),
                                     "name");
    cmdLine.addOption(namespaceName);

    QCommandLineOption warning(
            "warning", QCoreApplication::translate("main", "Create warning about code generation"));
    cmdLine.addOption(warning);

    QCommandLineOption qtException("qt-exception",
                                   QCoreApplication::translate("main", "Add Qt exception to GPL"));
    cmdLine.addOption(qtException);

    QCommandLineOption singleton("singleton",
                                 QCoreApplication::translate("main", "Create a singleton class"));
    cmdLine.addOption(singleton);

    QCommandLineOption protocol("protocol",
                                QCoreApplication::translate("main", "kioslave protocol"));
    cmdLine.addOption(protocol);

    QCommandLineOption filename("filename",
                                QCoreApplication::translate("main", "Source code file name"));
    cmdLine.addOption(filename);
    cmdLine.process(app);

    if (!cmdLine.parse(QCoreApplication::arguments())) {
        qDebug() << cmdLine.errorText();
        return -1;
    }

    if (cmdLine.isSet(createClass) || cmdLine.isSet(createDialog) || cmdLine.isSet(createKIOSlave)
        || cmdLine.isSet(createMain)) {
        return create(&cmdLine);
    } else if (cmdLine.isSet(codifyOption)) {
        return codify(&cmdLine);
    } else if (cmdLine.isSet(addPropertyOption)) {
        return addProperty(&cmdLine);
    } else {
        std::cerr << "Error: No command given." << std::endl;
        return 1;
    }
}
