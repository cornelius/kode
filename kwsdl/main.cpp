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
#include "settings.h"

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <klocale.h>
#include <kurl.h>
#include <kcomponentdata.h>

#include <QDir>
#include <QFile>
#include <QTimer>
#include <QCoreApplication>

int main( int argc, char **argv )
{
  KAboutData aboutData( "kwsdl_compiler", 0, ki18n( "KDE wsdl compiler" ), "0.1",
                        ki18n( "KDE WSDL Compiler" ),
                        KAboutData::License_LGPL );

  aboutData.addAuthor( ki18n("Tobias Koenig"), KLocalizedString(), "tokoe@kde.org" );

  KComponentData wsdlInstance( &aboutData );

  KCmdLineArgs::init( argc, argv, &aboutData );

  KCmdLineOptions options;
  options.add("c");
  options.add("configfile <file>", ki18n( "Configuration file" ), "kwsdl.cfg");
  options.add("d");
  options.add("outputDirectory <dir>", ki18n( "Directory to generate files in" ), ".");
  options.add("+wsdl", ki18n( "WSDL file" ));
  KCmdLineArgs::addCmdLineOptions( options );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  QCoreApplication app( argc, argv );

  if ( args->isSet( "configfile" ) ) {
    if ( !Settings::self()->load( args->getOption( "configfile" ) ) )
      return 1;
  } else {
    if ( args->count() != 1 ) {
      KCmdLineArgs::usageError( i18n( "Neither a config file nor a WSDL url is given." ) );
    }
  }

  if ( args->isSet( "outputDirectory" ) )
    Settings::self()->setOutputDirectory( args->getOption( "outputDirectory" ) );

  if ( args->count() == 1 )
    Settings::self()->setWsdlUrl( args->url( 0 ).path() );

  KWSDL::Compiler compiler;

  QTimer::singleShot( 0, &compiler, SLOT( run() ) );

  return app.exec();
}
