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

#include <iostream>

static const KCmdLineOptions options[] =
{
  { "d", 0, 0 },
  { "directory <dir>", I18N_NOOP("Directory to generate files in"), "." },
  { "+relaxng", I18N_NOOP("RelaxNG schema of XML file"), 0 },
  { "external-parser", I18N_NOOP("Generate parser in separate source file"),
    0 },
  { "custom-parser", I18N_NOOP("Generate parser customized for schema"), 0 },
  KCmdLineLastOption
};

int main( int argc, char **argv )
{
  KAboutData aboutData( "kxml_compiler", I18N_NOOP("KDE xml compiler"), "0.1",
  	I18N_NOOP("KDE XML Compiler") , KAboutData::License_LGPL );
  aboutData.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );

  KInstance app( &aboutData );

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

  QString dtdFilename = args->url( 0 ).path();

  QString baseName = args->url( 0 ).fileName();
  int pos = baseName.lastIndexOf( '.' );
  if ( pos > 0 ) baseName = baseName.left( pos );


  QFile dtdFile( dtdFilename );
  if ( !dtdFile.open( QIODevice::ReadOnly ) ) {
    kError() << "Unable to open '" << dtdFilename << "'" << endl;
    return 1;
  }

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &dtdFile, false, &errorMsg, &errorLine, &errorCol ) ) {
    kError() << errorMsg << " at " << errorLine << "," << errorCol << endl;
    return 1;
  }

  kDebug() << "Begin parsing" << endl;

  ParserRelaxng p;
  Element *start = p.parse( doc.documentElement() );
  if ( !start ) {
    kError() << "Could not find start element" << endl;
    return 1;
  }

  p.dumpDefinitionMap();
  
//  return 0;

  p.substituteReferences( start );

#if 0  
  std::cout << "--- TREE:" << std::endl;
  p.dumpTree( start );
#endif

  kDebug() << "Begin creating code" << endl;

  Creator::XmlParserType pt;
  if ( args->isSet( "custom-parser" ) ) {
    pt = Creator::XmlParserCustomExternal;
  } else if ( args->isSet( "external-parser" ) ) {
    pt = Creator::XmlParserDomExternal;
  } else {
    pt = Creator::XmlParserDom;
  }

  Creator c( pt );

  kDebug() << "Create classes" << endl;
  QList<Element *>::ConstIterator it;
  for( it = start->elements.begin(); it != start->elements.end(); ++it ) {
    c.createClass( *it );
  }
  kDebug() << "Create parser" << endl;
  for( it = start->elements.begin(); it != start->elements.end(); ++it ) {
    c.setExternalClassPrefix( c.upperFirst( (*it)->name ) );
    c.createFileParser( *it );
    c.createFileWriter( *it, dtdFilename.replace( "rng", "dtd" ) );
  }

  c.createListTypedefs();

#if 0
  QList<Reference *>::ConstIterator it2;
  for( it2 = start->references.begin(); it2 != start->references.end();
       ++it2 ) {
    Element e;
    e.name = (*it2)->name;
    e.pattern = (*it2)->pattern;
    c.createClass( &e );
  }
#endif

  kDebug() << "Begin printing code" << endl;

  KODE::File &f = c.file();
  
  f.setFilename( baseName );

  KODE::Printer printer;
  printer.setCreationWarning( true );
  printer.setGenerator( KCmdLineArgs::cwd() + KCmdLineArgs::appName() );
  printer.setOutputDirectory( baseDir );
  printer.setSourceFile( args->url( 0 ).fileName() );

  c.printFiles( printer );

  kDebug() << "Finished." << endl;
}
