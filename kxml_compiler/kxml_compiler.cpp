/*
    This file is part of KDE.

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

#include "parserrelaxng.h"
#include "creator.h"
#include "schema.h"

#include <libkode/code.h>
#include <libkode/printer.h>
#include <libkode/typedef.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>
#include <kurl.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>
#include <QList>
#include <QFileInfo>

#include <iostream>

static const KCmdLineOptions options[] =
{
  { "d", 0, 0 },
  { "directory <dir>", I18N_NOOP("Directory to generate files in"), "." },
  { "+schema", I18N_NOOP("Schema of XML file"), 0 },
  { "external-parser", I18N_NOOP("Generate parser in separate source file"),
    0 },
  { "custom-parser", I18N_NOOP("Generate parser customized for schema"), 0 },
  { "xsd", I18N_NOOP("Schema is XML Schema"), 0 },
  { "rng", I18N_NOOP("Schema is RelaxNG"), 0 },
  KCmdLineLastOption
};

int main( int argc, char **argv )
{
  KApplication::disableAutoDcopRegistration();
  KAboutData aboutData( "kxml_compiler", I18N_NOOP("KDE xml compiler"), "0.1",
  	I18N_NOOP("KDE XML Compiler") , KAboutData::License_LGPL );
  aboutData.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData, KCmdLineArgs::CmdLineArgNone );
  KCmdLineArgs::addCmdLineOptions( options );

  KApplication app( false );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() < 1 ) {
    kError() << "Too few arguments." << endl;
    return 1;
  }
  if ( args->count() > 1 ) {
    kError() << "Too many arguments." << endl;
    return 1;
  }

  QString baseDir = QFile::decodeName( args->getOption( "directory" ) );
  if ( !baseDir.endsWith( "/" ) ) baseDir.append( "/" );

  QString schemaFilename = args->url( 0 ).path();

  QString baseName = args->url( 0 ).fileName();
  int pos = baseName.lastIndexOf( '.' );
  if ( pos > 0 ) baseName = baseName.left( pos );


  QFile schemaFile( schemaFilename );
  if ( !schemaFile.open( QIODevice::ReadOnly ) ) {
    kError() << "Unable to open '" << schemaFilename << "'" << endl;
    return 1;
  }

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &schemaFile, false, &errorMsg, &errorLine, &errorCol ) ) {
    kError() << errorMsg << " at " << errorLine << "," << errorCol << endl;
    return 1;
  }

  kDebug() << "Begin parsing" << endl;

  Schema::Document schemaDocument;

  QFileInfo fi( schemaFile );
  if ( args->isSet( "xsd" ) || fi.suffix() == "xsd" ) {
    kError() << "XSD support not implemented yet." << endl;
    return 1;
  } else if ( args->isSet( "rng" ) || fi.suffix() == "rng" ) {
    RNG::ParserRelaxng p;
    RNG::Element *start = p.parse( doc.documentElement() );
    if ( !start ) {
      kError() << "Could not find start element" << endl;
      return 1;
    }

    p.dumpDefinitionMap();

  //  return 0;

    p.substituteReferences( start );

  #if 1
    std::cout << "--- TREE:" << std::endl;
    p.dumpTree( start );
  #endif

    schemaDocument = p.convertToSchema( start );
  } else {
    kError() << "Unable to determine schema type." << endl;
    return 1;
  }

  std::cout << "--- SCHEMA:" << std::endl;
  schemaDocument.dump();

  kDebug() << "Begin creating code" << endl;

  Creator::XmlParserType pt;
  if ( args->isSet( "custom-parser" ) ) {
    pt = Creator::XmlParserCustomExternal;
  } else if ( args->isSet( "external-parser" ) ) {
    pt = Creator::XmlParserDomExternal;
  } else {
    pt = Creator::XmlParserDom;
  }

  Creator c( schemaDocument, pt );

  kDebug() << "Create classes" << endl;
  foreach( Schema::Element e, schemaDocument.usedElements() ) {
    c.createClass( e );
  }
  kDebug() << "Create parser" << endl;
  Schema::Element startElement = schemaDocument.startElement();
  c.setExternalClassPrefix( c.upperFirst( startElement.name() ) );
  c.createFileParser( startElement );
//  c.setDtd( schemaFilename.replace( "rng", "dtd" ) );
  c.createFileWriter( startElement );

  c.createListTypedefs();

  kDebug() << "Begin printing code" << endl;

  KODE::File &f = c.file();

  f.setFilename( baseName );

  KODE::Printer printer;
  printer.setCreationWarning( true );
  printer.setGenerator( aboutData.appName() );
  printer.setOutputDirectory( baseDir );
  printer.setSourceFile( args->url( 0 ).fileName() );

  c.printFiles( printer );

  kDebug() << "Finished." << endl;
}
