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

#include "automakefile.h"
#include "code.h"
#include "license.h"
#include "printer.h"

#include <kabc/stdaddressbook.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kconfiggroup.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <ksavefile.h>
#include <kstandarddirs.h>

#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>

#include <iostream>

void addPropertyFunctions( QString &out, const QString &type,
  const QString &name )
{
  // FIXME: Use KODE::Function

  bool isReference = type.endsWith( "*" ) || type.endsWith( "&" );

  QString argument;
  QString upper = KODE::Style::upperFirst( name );
  if ( !isReference ) {
    argument = "const " + type + " &";
  } else argument = type;

  KODE::Code code;
  code.setIndent( 4 );
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
  code += type + ( isReference ? "" : " " ) + name + "() const";
  code += '{';
  code += "  return m" + upper + ';';
  code += '}';

  out += code.text();
}

void addPropertyVariable( QString &out, const QString &type,
  const QString &name )
{
  QString upper = KODE::Style::upperFirst( name );
  bool isReference = type.endsWith( "*" ) || type.endsWith( "&" );

  KODE::Code code;
  code.setIndent( 4 );
  code += type + ( isReference ? "" : " " ) + 'm' + upper + ';';

  out += code.text();
}

// FIXME: Put addProperty in PropertyAdder class and add endReadAhead function.
int addProperty( KCmdLineArgs *args )
{
  if ( args->count() != 3 ) {
    std::cerr << "Usage: kode --add-property <class> <proprerty-type> "
      << "<property-name>" << std::endl;
    return 1;
  }

  QString className = args->arg( 0 );
  QString type = args->arg( 1 );
  QString name = args->arg( 2 );

  kDebug() <<"Add property: class" << className <<":" << type << name;

  QString headerFileName = className.toLower() + ".h";

  QFile headerFile( headerFileName );
  if ( !headerFile.open( QIODevice::ReadOnly ) ) {
    std::cerr << "Unable to open file '" << headerFileName.data()/*.toUtf8()*/ << "'" <<
      std::endl;
    return 1;
  }

  QTextStream in( &headerFile );

  enum State { FindClass, FindConstructor, FindProperties, FindPrivate,
    FindVariables, Finish };
  State state = FindClass;

  QString accessor;
  QString mutator;

  QString out;

  QString readAhead;

  QString line;
  while ( !( line = in.readLine() ).isNull() ) {
    kDebug() << int(state) <<" LINE:" << line;
    QString readAheadPrevious = readAhead;
    readAhead += line + '\n';
    switch( state ) {
      case FindClass:
        if ( line.indexOf( QRegExp( "^\\s*class\\s+" + className ) ) >= 0 ) {
          kDebug() <<"  FOUND CLASS";
          state = FindConstructor;
        }
        break;
      case FindConstructor:
        if ( line.indexOf( QRegExp( "^\\s*" + className + "\\s*\\(" ) ) >= 0 ) {
          kDebug() <<"  FOUND CONSTRUCTOR";
          out += readAhead;
          readAhead.clear();
          state = FindProperties;
        }
        break;
      case FindProperties:
        {
          QRegExp re( "(\\w+)\\s*\\(" );
          if ( re.indexIn( line ) >= 0 ) {
            QString function = re.cap( 1 ).toLower();
            if ( !function.isEmpty() ) {
              kDebug() <<"Function:" << function;
              if ( function == className || function == '~' + className ) {
                out += readAhead;
                readAhead.clear();
              } else {
                if ( function.startsWith( "set" ) ) {
                  mutator = function.mid( 3 );
                  kDebug() <<"MUTATOR:" << mutator;
                } else {
                  if ( function == mutator ) {
                    accessor = function;
                    kDebug() <<"ACCESSOR:" << accessor;
                    out += readAhead;
                    readAhead.clear();
                  } else {
                    kDebug() <<"CREATE PROPERTY";
                    out += readAheadPrevious;
                    addPropertyFunctions( out, type, name );
                    out += '\n';
                    readAhead = line + '\n';
                    state = FindPrivate;
                  }
                }
              }
            }
          } else if ( line.indexOf( QRegExp( "\\s*protected" ) ) >= 0 ) {
            state = FindPrivate;
          } else if ( line.indexOf( QRegExp( "\\s*private" ) ) >= 0 ) {
            if ( accessor.isEmpty() ) {
              addPropertyFunctions( out, type, name );
              out += readAhead;
              readAhead.clear();
              addPropertyVariable( out, type, name );
              state = Finish;
            } else {
              if ( accessor == mutator ) {
                out += readAheadPrevious;
                addPropertyFunctions( out, type, name );
                out += '\n';
                out += line + '\n';
                readAhead.clear();
              }
              state = FindVariables;
            }
          }
        }
        break;
      case FindPrivate:
        if ( line.indexOf( QRegExp( "\\s*private" ) ) >= 0 ) {
          if ( accessor.isEmpty() ) {
            out += readAhead;
            readAhead.clear();
            addPropertyVariable( out, type, name );
            state = Finish;
          } else {
            state = FindVariables;
          }
        }
        break;
      case FindVariables:
        {
          if ( line.indexOf( 'm' + accessor.toLower(), 0, Qt::CaseInsensitive ) >= 0 ) {
            out += readAhead;
            readAhead.clear();
            addPropertyVariable( out, type, name );
            state = Finish;
          }
        }
        break;
      case Finish:
        break;
    }
  }

  headerFile.close();

  out += readAhead;

  if ( args->isSet( "inplace" ) ) {
    QString headerFileNameOut = headerFileName + ".kodeorig" ;

    // XXX Why copy instead of rename?
    QFile::remove( headerFileNameOut );
    if ( !QFile::copy( headerFileName, headerFileNameOut ) ) {
      kError() <<"Copy failed";
    } else {
      kDebug() <<"Write to original file.";
      if ( !headerFile.open( QIODevice::WriteOnly ) ) {
        kError() <<"Unable to open file '" << headerFileName <<
          "' for writing.";
        return 1;
      }
      QTextStream o( &headerFile );
      o << out << endl;
    }
  } else {
    std::cout << out.data()/*.toUtf8()*/ << std::endl;
  }

  return 0;
}

int codify( KCmdLineArgs *args )
{
  if ( args->count() != 1 ) {
    std::cerr << "Usage: kode --codify <sourcecodefile>" << std::endl;
    return 1;
  }

  QString filename = args->arg( 0 );

  QFile f( filename );
  if ( !f.open( QIODevice::ReadOnly ) ) {
    kError() <<"Unable to open file '" << filename <<"'.";
    return 1;
  } else {
    std::cout << "KODE::Code code;" << std::endl;
    QTextStream ts( &f );
    QString line;
    while( !( line = ts.readLine() ).isNull() ) {
      line.replace( "\\", "\\\\" );
      line.replace( "\"", "\\\"" );
      line = "code += \"" + line;
      line.append( "\";" );
      std::cout << line.data()/*.toLocal8Bit()*/ << std::endl;
    }
  }

  return 0;
}

int create( KCmdLineArgs *args )
{
  KODE::Printer p;
  if ( args->isSet( "warning" ) ) p.setCreationWarning( true );

  bool createKioslave = args->isSet( "create-kioslave" );
  bool createMain = args->isSet( "create-main" );

  QString filename = args->getOption( "filename" );

  if ( createMain ) {
    if ( filename.isEmpty() ) {
      kError() <<"Error: No file name given.";
      return 1;
    }

    if ( filename.endsWith( ".cpp" ) ) {
      filename = filename.left( filename.length() - 4 );
    }
  } else {
    if ( !args->isSet( "classname" ) ) {
      kError() <<"Error: No class name given.";
      return 1;
    }
  }

  QString className = args->getOption( "classname" );

  QString protocol;

  if ( createKioslave ) {
    if ( !args->isSet( "protocol" ) ) {
      protocol = className.toLower();
      kWarning() <<"Warning: No protocol for kioslave given. Assuming '"
                  << protocol << "'";
    } else {
      protocol = args->getOption( "protocol" );
    }
  }

  KODE::File file;

  file.setProject( args->getOption( "project" ) );

  QString authorEmail = args->getOption( "author-email" );
  QString authorName;
  KABC::Addressee a;
  if ( authorEmail.isEmpty() ) {
    a = KABC::StdAddressBook::self()->whoAmI();
    authorEmail = a.preferredEmail();
  } else {
    KABC::Addressee::List as =
        KABC::StdAddressBook::self()->findByEmail( authorEmail );
    if ( as.isEmpty() ) {
      kDebug() <<"Unable to find '" << authorEmail <<"' in address book.";
    } else {
      a = as.first();
    }
  }
  if ( !a.isEmpty() ) {
      authorName = a.realName();
  }
  if ( !authorEmail.isEmpty() ) {
    file.addCopyright( QDate::currentDate().year(), authorName, authorEmail );
  }

  KODE::License l;
  if ( args->isSet( "gpl" ) ) l = KODE::License( KODE::License::GPL );
  if ( args->isSet( "lgpl" ) ) l = KODE::License( KODE::License::LGPL );
  l.setQtException( args->isSet( "qt-exception" ) );
  file.setLicense( l );

  file.setNameSpace( args->getOption( "namespace" ) );

  if ( createMain ) {
    file.addInclude( "kaboutdata.h" );
    file.addInclude( "kapplication.h" );
    file.addInclude( "kdebug" );
    file.addInclude( "klocale" );
    file.addInclude( "kcmdlineargs" );

    KODE::Function main( "main", "int" );
    main.addArgument( "int argc" );
    main.addArgument( "char **argv" );

    KODE::Code code;
    code += "KAboutData aboutData(\"test\",0,ki18n(\"Test\"),\"0.1\");";
    code += "KCmdLineArgs::init(argc,argv,&aboutData);";
    code += "";
    code += "KCmdLineOptions options;";
    code += "options.add(\"verbose\", ki18n(\"Verbose output\"));";
    code += "KCmdLineArgs::addCmdLineOptions( options );";
    code += "";
    code += "KApplication app;";
    code += "";
    code += "KCmdLineArgs *args = KCmdLineArgs::parsedArgs();";
    code += "";
    code += "Q_UNUSED( args );";
    main.setBody( code );

    file.addFileFunction( main );

    file.setFilename( filename );

    p.printImplementation( file, false );

    return 0;
  }

  KODE::Class c( className );

  if ( args->isSet( "create-dialog" ) ) {
    c.addBaseClass( KODE::Class( "KDialog" ) );
    c.addInclude( "kdialog.h" );
  } else if ( createKioslave ) {
    c.setDocs( "This class implements a kioslave for ..." );

    c.addBaseClass( KODE::Class( "SlaveBase", "KIO" ) );
    c.addHeaderInclude( "kio/slavebase.h" );

    KODE::Function get( "get", "void" );
    get.addArgument( "const KUrl &url" );

    KODE::Code code;

    code += "kDebug(7000) << \"" + className +"::get()\";";
    code += "kDebug(7000) << \" URL: \" << url.url();";
    code += "#if 1";
    code += "kDebug(7000) << \" Path: \" << url.path();";
    code += "kDebug(7000) << \" Query: \" << url.query();";
    code += "kDebug(7000) << \" Protocol: \" << url.protocol();";
    code += "kDebug(7000) << \" Filename: \" << url.filename();";
    code += "#endif";
    code.newLine();

    code += "mimeType( \"text/plain\" );";
    code.newLine();

    code += "QCString str( \"Hello!\" );";
    code += "data( str );";
    code.newLine();

    code += "finished();";
    code.newLine();

    code += "kDebug(7000) << \"" + className +"CgiProtocol::get() done\";";

    get.setBody( code );

    c.addFunction( get );


    c.addInclude( "kinstance.h" );
    c.addInclude( "kdebug.h" );
    c.addInclude( "sys/types.h" );
    c.addInclude( "unistd.h" );
    c.addInclude( "stdlib.h" );

    KODE::Function main( "kdemain", "int" );
    main.addArgument( "int argc" );
    main.addArgument( "char **argv" );

    code.clear();

    code += "KComponentData instance( \"kio_" + protocol + "\" );";
    code += "";
    code += "kDebug(7000) << \"Starting kio_" + protocol +"(pid:  \" << getpid() << \")\";";
    code += "";
    code += "if (argc != 4) {";
    code.indent();
    code += "fprintf( stderr, \"Usage: kio_" + protocol + " protocol domain-socket1 domain-socket2\\n\");";
    code += "exit( -1 );";
    code.unindent();
    code += '}';
    code += "";
    code += className + " slave( argv[2], argv[3] );";
    code += "slave.dispatchLoop();";
    code += "";
    code += "return 0;";

    main.setBody( code );

    file.addFileFunction( main );

    file.addExternCDeclaration( p.functionSignature( main ) );
  }

  KODE::Function constructor( className );

  if ( args->isSet( "singleton" ) ) {
    constructor.setAccess( KODE::Function::Private );

    KODE::Function self( "self", className + " *" );
    self.setStatic( true );

    KODE::Code code;
    code += "if ( !mSelf ) {";
    code += "  selfDeleter.setObject( mSelf, new " + className + "() );";
    code += '}';
    code += "return mSelf;";

    self.setBody( code );

    c.addFunction( self );

    KODE::MemberVariable selfVar( "mSelf", className + " *", true );
    selfVar.setInitializer( "0" );

    c.addMemberVariable( selfVar );

    KODE::Variable staticDeleter( "selfDeleter",
                                  "K3StaticDeleter<" + className + '>',
                                  true );
    file.addFileVariable( staticDeleter );
    file.addInclude( "k3staticdeleter.h" );
  }

  if ( createKioslave ) {
    constructor.addArgument( "const QCString &pool" );
    constructor.addArgument( "const QCString &app" );

    constructor.addInitializer( "SlaveBase( \"" + protocol + "\", pool, app )" );
  }

  c.addFunction( constructor );

  file.insertClass( c );

  p.printHeader( file );
  p.printImplementation( file );

  if ( createKioslave ) {
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


    // Write protocol file
    QString protocolFilename = protocol + ".protocol";

    QFileInfo fi( protocolFilename );
    protocolFilename = fi.absoluteFilePath();

    KSaveFile::simpleBackupFile( protocolFilename, QString(), ".backup" );

    QFile::remove( protocolFilename );

    KConfig protocolFile( protocolFilename, KConfig::SimpleConfig);

    KConfigGroup group( &protocolFile, "Protocol" );
    group.writeEntry( "exec", "kio_" + protocol );
    group.writeEntry( "protocol", protocol );
    group.writeEntry( "input", "none" );
    group.writeEntry( "output", "filesystem" );
    group.writeEntry( "reading", "true" );
    group.writeEntry( "DocPath", "kioslave/" + protocol + ".html" );

    protocolFile.sync();
  }

  return 0;
}

int main(int argc,char **argv)
{
  KAboutData aboutData( "kode", 0, ki18n("KDE Code Generator"), "0.1" );
  aboutData.addAuthor( ki18n("Cornelius Schumacher"), KLocalizedString(), "schumacher@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );

  KCmdLineOptions options;
  options.add("c");
  options.add("create-class", ki18n("Create class"));
  options.add("d");
  options.add("create-dialog", ki18n("Create dialog"));
  options.add("create-kioslave", ki18n("Create kioslave"));
  options.add("create-main", ki18n("Create main function template"));
  options.add("y");
  options.add("codify", ki18n("Create generator code for given source"));
  options.add("add-property", ki18n("Add property to class"));
  options.add("inplace", ki18n("Change file in place"));
  options.add("author-email <name>", ki18n("Add author with given email address"));
  options.add("project <name>", ki18n("Name of project"));
  options.add("gpl", ki18n("Use GPL as license"));
  options.add("lgpl", ki18n("Use LGPL as license"));
  options.add("classname <name>", ki18n("Name of class"));
  options.add("filename <name>", ki18n("Name of file"));
  options.add("namespace <name>", ki18n("Namespace"));
  options.add("warning", ki18n("Create warning about code generation"));
  options.add("qt-exception", ki18n("Add Qt exception to GPL"));
  options.add("singleton", ki18n("Create a singleton class"));
  options.add("protocol", ki18n("kioslave protocol"));
  options.add("+[filename]", ki18n("Source code file name"));
  KCmdLineArgs::addCmdLineOptions( options );

  KApplication app;

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->isSet( "create-class" ) || args->isSet( "create-dialog" ) ||
       args->isSet( "create-kioslave" ) || args->isSet( "create-main" ) ) {
    return create( args );
  } else if ( args->isSet( "codify" ) ) {
    return codify( args );
  } else if ( args->isSet( "add-property" ) ) {
    return addProperty( args );
  } else {
    std::cerr << "Error: No command given." << std::endl;
    return 1;
  }
}
