/*
    This file is part of KDE.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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
#include "converter.h"
#include "creator.h"

#include <kode/code.h>
#include <kode/printer.h>
#include <kode/typedef.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <klocale.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>

static const KCmdLineOptions options[] =
{
  { "d", 0, 0 },
  { "directory <dir>", I18N_NOOP( "Directory to generate files in" ), "." },
  { "+wsdl", I18N_NOOP( "WSDL file" ), 0 },
  KCmdLineLastOption
};

int main( int argc, char **argv )
{
  KAboutData aboutData( "kwsdl_compiler", I18N_NOOP( "KDE wsdl compiler" ), "0.1",
                        I18N_NOOP( "KDE WSDL Compiler" ),
                        KAboutData::License_LGPL );

  aboutData.addAuthor( "Tobias Koenig", 0, "tokoe@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() < 1 || args->count() > 1 ) {
    kdError() << "Invalid arguments, try --help." << endl;
    return 1;
  }

  KInstance app( &aboutData );

  QString baseDir = QFile::decodeName( args->getOption( "directory" ) );
  if ( !baseDir.endsWith( "/" ) )
    baseDir.append( "/" );

  QString wsdlFilename = args->url( 0 ).path();
  QFile wsdlFile( wsdlFilename );

  if ( !wsdlFile.open( IO_ReadOnly ) ) {
    kdError() << "Unable to open '" << wsdlFilename << "'" << endl;
    return 1;
  }

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &wsdlFile, true, &errorMsg, &errorLine, &errorCol ) ) {
    kdError() << errorMsg << " at " << errorLine << "," << errorCol << endl;
    return 1;
  }

  kdDebug() << "Begin parsing" << endl;

  KWSDL::Parser parser;
  parser.parse( doc.documentElement() );

  KWSDL::Converter converter;
  converter.setBindings( parser.bindings() );
  converter.setMessages( parser.messages() );
  converter.setPorts( parser.ports() );
  converter.setService( parser.service() );
  converter.setTypes( parser.types() );
  converter.setParser( &parser.parser() );

  converter.convert();

  KWSDL::Creator creator;
  creator.setOutputDirectory( baseDir );
  creator.create( converter.classes() );
}
