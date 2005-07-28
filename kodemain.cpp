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
#include "printer.h"
#include "license.h"
#include "automakefile.h"

#include <kabc/stdaddressbook.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kstandarddirs.h>
#include <ksimpleconfig.h>
#include <ksavefile.h>
#include <kprocess.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qfileinfo.h>
#include <qregexp.h>

#include <iostream>

static const KCmdLineOptions options[] =
{
  { "c", 0, 0 },
  { "create-class", I18N_NOOP("Create class"), 0 },
  { "d", 0, 0 },
  { "create-dialog", I18N_NOOP("Create dialog"), 0 },
  { "create-kioslave", I18N_NOOP("Create kioslave"), 0 },
  { "create-main", I18N_NOOP("Create main function template"), 0 },
  { "y", 0, 0 },
  { "codify", I18N_NOOP("Create generator code for given source"), 0 },
  { "add-property", I18N_NOOP("Add property to class"), 0 },
  { "inplace", I18N_NOOP("Change file in place"), 0 },
  { "author-email <name>", I18N_NOOP("Add author with given email address"), 0 },
  { "project <name>", I18N_NOOP("Name of project"), 0 },
  { "gpl", I18N_NOOP("Use GPL as license"), 0 },
  { "lgpl", I18N_NOOP("Use LGPL as license"), 0 },
  { "classname <name>", I18N_NOOP("Name of class"), 0 },
  { "filename <name>", I18N_NOOP("Name of file"), 0 },
  { "namespace <name>", I18N_NOOP("Namespace"), 0 },
  { "warning", I18N_NOOP("Create warning about code generation"), 0 },
  { "qt-exception", I18N_NOOP("Add Qt exception to GPL"), 0 },
  { "singleton", I18N_NOOP("Create a singleton class"), 0 },
  { "protocol", I18N_NOOP("kioslave protocol"), 0 },
  { "+[filename]", I18N_NOOP("Source code file name"), 0 },
  KCmdLineLastOption
};

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
  code += "{";
  code += "  m" + upper + " = v;";
  code += "}";
  
  code += "/**";
  code += "  Get " + name + ". See set" + upper + "().";
  code += "*/";
  code += type + ( isReference ? "" : " " ) + name + "() const";
  code += "{";
  code += "  return m" + upper + ";";
  code += "}";

  out += code.text();
}

void addPropertyVariable( QString &out, const QString &type,
  const QString &name )
{
  QString upper = KODE::Style::upperFirst( name );
  bool isReference = type.endsWith( "*" ) || type.endsWith( "&" );

  KODE::Code code;
  code.setIndent( 4 );
  code += type + ( isReference ? "" : " " ) + "m" + upper + ";"; 

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

  kdDebug() << "Add property: class " << className << ": " << type << " " <<
    name << endl;

  QString headerFileName = className.lower() + ".h";
  
  QFile headerFile( headerFileName );
  if ( !headerFile.open( IO_ReadOnly ) ) {
    std::cerr << "Unable to open file '" << headerFileName.utf8() << "'" <<
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
//    std::cout << line.utf8() << std::endl;
    kdDebug() << state << " LINE: " << line << endl;
    QString readAheadPrevious = readAhead;
    readAhead += line + "\n";
//    out += line + "\n";
    switch( state ) {
      case FindClass:
//        if ( line.find( QRegExp( className ) ) >= 0 ) {
        if ( line.find( QRegExp( "^\\s*class\\s+" + className ) ) >= 0 ) {
          kdDebug() << "  FOUND CLASS" << endl;
          state = FindConstructor;
        }
        break;
      case FindConstructor:
        if ( line.find( QRegExp( "^\\s*" + className + "\\s*\\(" ) ) >= 0 ) {
          kdDebug() << "  FOUND CONSTRUCTOR" << endl;
          out += readAhead;
          readAhead = QString::null;
          state = FindProperties;
        }
        break;
      case FindProperties:
        {
          QRegExp re( "(\\w+)\\s*\\(" );
          if ( re.search( line ) >= 0 ) {
            QString function = re.cap( 1 ).lower();
            if ( !function.isEmpty() ) {
              kdDebug() << "Function: " << function << endl;
              if ( function == className || function == "~" + className ) {
                out += readAhead;
                readAhead = QString::null;
              } else {
                if ( function.startsWith( "set" ) ) {
                  mutator = function.mid( 3 );
                  kdDebug() << "MUTATOR: " << mutator << endl;
                } else {
                  if ( function == mutator ) {
                    accessor = function;
                    kdDebug() << "ACCESSOR: " << accessor << endl;
                    out += readAhead;
                    readAhead = QString::null;
                  } else {
                    kdDebug() << "CREATE PROPERTY" << endl;
                    out += readAheadPrevious;
                    addPropertyFunctions( out, type, name );
                    out += "\n";
                    readAhead = line + "\n";
                    state = FindPrivate;
                  }
                }
              }
            }
          } else if ( line.find( QRegExp( "\\s*protected" ) ) >= 0 ) {
            state = FindPrivate;
          } else if ( line.find( QRegExp( "\\s*private" ) ) >= 0 ) {
            if ( accessor.isEmpty() ) {
              addPropertyFunctions( out, type, name );
              out += readAhead;
              readAhead = QString::null;
              addPropertyVariable( out, type, name );
              state = Finish;
            } else {
              if ( accessor == mutator ) {
                out += readAheadPrevious;
                addPropertyFunctions( out, type, name );
                out += "\n";
                out += line + "\n";
                readAhead = QString::null;
              }
              state = FindVariables;
            }
          }
        }
        break;
      case FindPrivate:
        if ( line.find( QRegExp( "\\s*private" ) ) >= 0 ) {
          if ( accessor.isEmpty() ) {
            out += readAhead;
            readAhead = QString::null;
            addPropertyVariable( out, type, name );
            state = Finish;
          } else {
            state = FindVariables;
          }
        }
        break;
      case FindVariables:
        {
          if ( line.find( "m" + accessor.lower(), 0, false ) >= 0 ) {
            out += readAhead;
            readAhead = QString::null;
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

    KProcess proc;
    proc << "cp" << QFile::encodeName( headerFileName ) <<
      QFile::encodeName( headerFileNameOut );
    
    if ( !proc.start( KProcess::Block ) ) {
      kdError() << "Copy failed" << endl;
    } else {
      kdDebug() << "Write to original file." << endl;
      if ( !headerFile.open( IO_WriteOnly ) ) {
        kdError() << "Unable to open file '" << headerFileName <<
          "' for writing." << endl;
        return 1;
      }
      QTextStream o( &headerFile );
      o << out << endl;
    }
  } else {
    std::cout << out.utf8() << std::endl;
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
  if ( !f.open( IO_ReadOnly ) ) {
    kdError() << "Unable to open file '" << filename << "'." << endl;
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
      std::cout << line.local8Bit() << std::endl;
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
      kdError() << "Error: No file name given." << endl;
      return 1;
    }

    if ( filename.endsWith( ".cpp" ) ) {
      filename = filename.left( filename.length() - 4 );
    }
  } else {
    if ( !args->isSet( "classname" ) ) {
      kdError() << "Error: No class name given." << endl;
      return 1;
    }
  }

  QString className = args->getOption( "classname" );

  QString protocol;

  if ( createKioslave ) {
    if ( !args->isSet( "protocol" ) ) {
      protocol = className.lower();
      kdWarning() << "Warning: No protocol for kioslave given. Assuming '"
                  << protocol << "'" << endl;
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
      kdDebug() << "Unable to find '" << authorEmail << "' in address book."
                << endl;
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

    KODE::Code code;
    code += "static const KCmdLineOptions options[] =";
    code += "{";
    code += "  { \"verbose\", \"Verbose output\", 0 },";
    code += "  KCmdLineLastOption";
    code += "};";
    file.addFileCode( code );

    KODE::Function main( "main", "int" );
    main.addArgument( "int argc" );
    main.addArgument( "char **argv" );

    code.clear();
    code += "KAboutData aboutData(\"test\",\"Test\",\"0.1\");";
    code += "KCmdLineArgs::init(argc,argv,&aboutData);";
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
    c.addBaseClass( KODE::Class( "KDialogBase" ) );
    c.addInclude( "kdialogbase.h" );
  } else if ( createKioslave ) {
    c.setDocs( "This class implements a kioslave for ..." );
  
    c.addBaseClass( KODE::Class( "SlaveBase", "KIO" ) );
    c.addHeaderInclude( "kio/slavebase.h" );
    
    KODE::Function get( "get", "void" );
    get.addArgument( "const KURL &url" );

    KODE::Code code;
    
    code += "kdDebug(7000) << \"" + className + "::get()\" << endl;";
    code += "kdDebug(7000) << \" URL: \" << url.url() << endl;";
    code += "#if 1";
    code += "kdDebug(7000) << \" Path: \" << url.path() << endl;";
    code += "kdDebug(7000) << \" Query: \" << url.query() << endl;";
    code += "kdDebug(7000) << \" Protocol: \" << url.protocol() << endl;";
    code += "kdDebug(7000) << \" Filename: \" << url.filename() << endl;";
    code += "#endif";
    code.newLine();

    code += "mimeType( \"text/plain\" );";
    code.newLine();
    
    code += "QCString str( \"Hello!\" );";
    code += "data( str );";
    code.newLine();
    
    code += "finished();";
    code.newLine();
    
    code += "kdDebug(7000) << \"" + className + "CgiProtocol::get() done\" << endl;";

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

    code += "KInstance instance( \"kio_" + protocol + "\" );";
    code += "";
    code += "kdDebug(7000) << \"Starting kio_" + protocol + "(pid:  \" << getpid() << \")\" << endl;";
    code += "";
    code += "if (argc != 4) {";
    code.indent();
    code += "fprintf( stderr, \"Usage: kio_" + protocol + " protocol domain-socket1 domain-socket2\\n\");";
    code += "exit( -1 );";
    code.unindent();
    code += "}";
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
    code += "}";
    code += "return mSelf;";

    self.setBody( code );

    c.addFunction( self );

    KODE::MemberVariable selfVar( "mSelf", className + " *", true );
    selfVar.setInitializer( "0" );
  
    c.addMemberVariable( selfVar );

    KODE::Variable staticDeleter( "selfDeleter",
                                  "KStaticDeleter<" + className + ">",
                                  true );
    file.addFileVariable( staticDeleter );
    file.addInclude( "kstaticdeleter.h" );
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
    am.addEntry( "noinst_HEADERS", className.lower() + ".h" );
    am.newLine();
    am.addEntry( "METASOURCES", "AUTO" );
    am.newLine();
    am.addEntry( "kdelnkdir", "$(kde_servicesdir)" );
    am.addEntry( "kdelnk_DATA", protocol + ".protocol" );

    KODE::AutoMakefile::Target t( "kde_module_LTLIBRARIES",
      "kio_" + protocol + ".la" );
    t.setSources( className.lower() + ".cpp" );
    t.setLibAdd( "$(LIB_KIO)" );
    t.setLdFlags( "$(all_libraries) -module $(KDE_PLUGIN)" );
  
    am.addTarget( t );

    p.printAutoMakefile( am );
  
    
    // Write protocol file
    QString protocolFilename = protocol + ".protocol";

    QFileInfo fi( protocolFilename );
    protocolFilename = fi.absFilePath();

    KSaveFile::backupFile( protocolFilename, QString::null, ".backup" );
    
    QFile::remove( protocolFilename );
    
    KSimpleConfig protocolFile( protocolFilename );

    protocolFile.setGroup( "Protocol" );
    protocolFile.writeEntry( "exec", "kio_" + protocol );
    protocolFile.writeEntry( "protocol", protocol );
    protocolFile.writeEntry( "input", "none" );
    protocolFile.writeEntry( "output", "filesystem" );
    protocolFile.writeEntry( "reading", "true" );
    protocolFile.writeEntry( "DocPath", "kioslave/" + protocol + ".html" );

    protocolFile.sync();
  }

  return 0;
}

int main(int argc,char **argv)
{
  KAboutData aboutData( "kode", I18N_NOOP("KDE Code Generator"), "0.1" );
  aboutData.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );
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
