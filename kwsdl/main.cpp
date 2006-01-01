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

#include <QDir>
#include <QFile>
#include <QTimer>

static const KCmdLineOptions options[] =
{
  { "c", 0, 0 },
  { "configfile <file>", I18N_NOOP( "Configuration file" ), "kwsdl.cfg" },
  { "d", 0, 0 },
  { "outputDirectory <dir>", I18N_NOOP( "Directory to generate files in" ), "." },
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

  if ( args->isSet( "configfile" ) ) {
    if ( !Settings::self()->load( args->getOption( "configfile" ) ) )
      return 1;
  } else {
    if ( args->count() != 1 ) {
      KCmdLineArgs::usage( i18n("Neither a config file nor a WSDL url is given.") );
    }
  }

  if ( args->isSet( "outputDirectory" ) )
    Settings::self()->setOutputDirectory( args->getOption( "outputDirectory" ) );

  if ( args->count() == 1 )
    Settings::self()->setWsdlUrl( args->url( 0 ).path() );

  KApplication app( false );

  KWSDL::Compiler compiler;

  QTimer::singleShot( 0, &compiler, SLOT( run() ) );

  return app.exec();
}
