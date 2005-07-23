/*
    This file is part of Kung.

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

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <klocale.h>

#include <qtimer.h>

#include "loader.h"

static const KCmdLineOptions options[] =
{
  { "+wsdl", I18N_NOOP( "Location of WSDL file" ), 0 },
  KCmdLineLastOption
};

int main( int argc, char **argv )
{
  KAboutData aboutData( "kung", I18N_NOOP( "KDE WSDL Interpreter" ), "0.1",
                        I18N_NOOP( "KDE WSDL Interpreter" ),
                        KAboutData::License_LGPL );

  aboutData.addAuthor( "Tobias Koenig", 0, "tokoe@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() == 0 ) {
    kdError() << "No WSDL file given." << endl;
    return 1;
  }

  KApplication app;

  Loader loader;
  loader.setWSDLUrl( args->url( 0 ).path() );

  QTimer::singleShot( 0, &loader, SLOT( run() ) );

  return app.exec();
}
