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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "parser.h"
#include "creator.h"

#include <kode/code.h>
#include <kode/printer.h>
#include <kode/typedef.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>

#include <iostream>

static const KCmdLineOptions options[] =
{
  { "d", 0, 0 },
  { "directory <dir>", I18N_NOOP("Directory to generate files in"), "." },
  { "+dtd", I18N_NOOP("DTD of XML file"), 0 },
  { "external-parser", I18N_NOOP("Generate parser in separate source file."),
    0 },
  { "custom-parser", I18N_NOOP("Generate parser customized for schema."), 0 },
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
    kdError() << "Too few arguments." << endl;
    return 1;
  }
  if ( args->count() > 1 ) {
    kdError() << "Too many arguments." << endl;
    return 1;
  }

  QString baseDir = QFile::decodeName( args->getOption( "directory" ) );
  if ( !baseDir.endsWith( "/" ) ) baseDir.append( "/" );

  QString dtdFilename = args->url( 0 ).path();

  QString baseName = args->url( 0 ).fileName();
  int pos = baseName.findRev( '.' );
  if ( pos > 0 ) baseName = baseName.left( pos );


  QFile dtdFile( dtdFilename );
  if ( !dtdFile.open( IO_ReadOnly ) ) {
    kdError() << "Unable to open '" << dtdFilename << "'" << endl;
    return 1;
  }

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &dtdFile, false, &errorMsg, &errorLine, &errorCol ) ) {
    kdError() << errorMsg << " at " << errorLine << "," << errorCol << endl;
    return 1;
  }

  kdDebug() << "Begin parsing" << endl;

  Parser p;
  Element *start = p.parse( doc.documentElement() );
  if ( !start ) {
    kdError() << "Could not find start element" << endl;
    return 1;
  }

  p.dumpDefinitionMap();
  
//  return 0;

  p.substituteReferences( start );

#if 0  
  std::cout << "--- TREE:" << std::endl;
  p.dumpTree( start );
#endif

  kdDebug() << "Begin creating code" << endl;

  Creator::XmlParserType pt;
  if ( args->isSet( "custom-parser" ) ) {
    pt = Creator::XmlParserCustomExternal;
  } else if ( args->isSet( "external-parser" ) ) {
    pt = Creator::XmlParserDomExternal;
  } else {
    pt = Creator::XmlParserDom;
  }

  Creator c( pt );

  kdDebug() << "Create classes" << endl;
  QValueList<Element *>::ConstIterator it;
  for( it = start->elements.begin(); it != start->elements.end(); ++it ) {
    c.createClass( *it );
  }
  kdDebug() << "Create parser" << endl;
  for( it = start->elements.begin(); it != start->elements.end(); ++it ) {
    c.setExternalClassPrefix( c.upperFirst( (*it)->name ) );
    c.createFileParser( *it );
    c.createFileWriter( *it, dtdFilename.replace( "rng", "dtd" ) );
  }

  c.createListTypedefs();

#if 0
  QValueList<Reference *>::ConstIterator it2;
  for( it2 = start->references.begin(); it2 != start->references.end();
       ++it2 ) {
    Element e;
    e.name = (*it2)->name;
    e.pattern = (*it2)->pattern;
    c.createClass( &e );
  }
#endif

  kdDebug() << "Begin printing code" << endl;

  KODE::File &f = c.file();
  
  f.setFilename( baseName );

  KODE::Printer printer;
  printer.setCreationWarning( true );
  printer.setGenerator( "kxml_compiler" );
  printer.setOutputDirectory( baseDir );
  printer.setSourceFile( args->url( 0 ).fileName() );

  c.printFiles( printer );

  kdDebug() << "Finished." << endl;
}
