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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "compiler.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <klocale.h>

#include <qdir.h>
#include <qfile.h>
#include <qtimer.h>

static const KCmdLineOptions options[] =
{
  { "d", 0, 0 },
  { "directory <dir>", I18N_NOOP( "Directory to generate files in" ), "." },
  { "n", 0, 0 },
  { "namespace <namespace>", I18N_NOOP( "Namespace of the created classes" ), "." },
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
    qDebug( "Invalid arguments, try --help." );
    return 1;
  }

  KApplication app( false, false );

  QString outputDirectory = QFile::decodeName( args->getOption( "directory" ) );
  if ( outputDirectory.isEmpty() )
    outputDirectory = QDir::current().path();

  KWSDL::Compiler compiler;
  compiler.setWSDLUrl( args->url( 0 ).path() );
  compiler.setOutputDirectory( outputDirectory );
  if ( args->isSet( "namespace" ) )
    compiler.setNameSpace( args->getOption( "namespace" ) );

  QTimer::singleShot( 0, &compiler, SLOT( run() ) );

  return app.exec();
}
