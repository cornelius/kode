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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "code.h"
#include "printer.h"
#include "license.h"

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

#include <qfile.h>
#include <qtextstream.h>

#include <iostream>

static const KCmdLineOptions options[] =
{
  { "c", 0, 0 },
  { "create-class", I18N_NOOP("Create class"), 0 },
  { "d", 0, 0 },
  { "create-dialog", I18N_NOOP("Create dialog"), 0 },
  { "y", 0, 0 },
  { "codify", I18N_NOOP("Create generator code for given source"), 0 },
  { "author-email <name>", I18N_NOOP("Add author with given email address"), 0 },
  { "project <name>", I18N_NOOP("Name of project"), 0 },
  { "gpl", I18N_NOOP("Use GPL as license"), 0 },
  { "lgpl", I18N_NOOP("Use LGPL as license"), 0 },
  { "classname <name>", I18N_NOOP("Name of class"), 0 },
  { "namespace <name>", I18N_NOOP("Namespace"), 0 },
  { "warning", I18N_NOOP("Create warning about code generation"), 0 },
  { "qt-exception", I18N_NOOP("Add Qt excpetion to GPL"), 0 },
  { "singleton", I18N_NOOP("Create a singleton class"), 0 },
  { "+[filename]", I18N_NOOP("Source code file name"), 0 },
  KCmdLineLastOption
};

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
  if ( !args->isSet( "classname" ) ) {
    std::cerr << "Error: No class name given." << std::endl;
    return 1;
  }

  KODE::File file;

  file.setProject( args->getOption( "project" ) );

  QString authorEmail = args->getOption( "author-email" );
  if ( !authorEmail.isEmpty() ) {
    KABC::Addressee::List a =
        KABC::StdAddressBook::self()->findByEmail( authorEmail );
    QString authorName;
    if ( a.isEmpty() ) {
      kdDebug() << "Unable to find '" << authorEmail << "' in address book."
                << endl;
    } else {
      authorName = a.first().realName();
    }
    file.addCopyright( QDate::currentDate().year(), authorName, authorEmail );
  }

  KODE::License l;
  if ( args->isSet( "gpl" ) ) l = KODE::License( KODE::License::GPL );
  if ( args->isSet( "lgpl" ) ) l = KODE::License( KODE::License::LGPL );
  l.setQtException( args->isSet( "qt-exception" ) );
  file.setLicense( l );

  file.setNameSpace( args->getOption( "namespace" ) );

  QString className = args->getOption( "classname" );

  KODE::Class c( className );

  if ( args->isSet( "create-dialog" ) ) {
    c.addBaseClass( KODE::Class( "KDialogBase" ) );
    c.addInclude( "kdialogbase.h" );
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

  c.addFunction( constructor );

  file.insertClass( c );

  KODE::Printer p;
  if ( args->isSet( "warning" ) ) p.setCreationWarning( true );
  p.printHeader( file );
  p.printImplementation( file );

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

  if ( args->isSet( "create-class" ) || args->isSet( "create-dialog" ) ) {
    return create( args );
  } else if ( args->isSet( "codify" ) ) {
    return codify( args );
  } else {
    std::cerr << "Error: No command given." << std::endl;
    return 1;
  }
}
