/*
    This file is part of kdepim.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "kde-holidays.h"
#include "kde-holidays_parser.h"

#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kdebug.h>

#include <QFile>
#include <qtextstream.h>

#include <iostream>

static const KCmdLineOptions options[] =
{
  { "+holidayfile", "Name of holiday XML file", 0 },
  { "output <file>", "Name of output file", 0 },
  KCmdLineLastOption
};

int main( int argc, char **argv )
{
  KApplication::disableAutoDcopRegistration();
  KAboutData aboutData( "testholidays", "Dump holidays to stdout",
                        "0.1" );
  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );

  KApplication app( false );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() != 1 ) {
    args->usage( "Wrong number of arguments." );
  }

  QString filename = QFile::decodeName( args->arg( 0 ) );

  HolidayCalendarParser parser;

  bool ok;
  HolidayCalendar holidays = parser.parseFile( filename, &ok );

  if ( !ok ) {
    kError() << "Parse error" << endl;
  } else {
    // TODO: Print data to stdout
  }

  if ( args->isSet( "output" ) ) {
    QString out = args->getOption( "output" );
    if ( !holidays.writeFile( out ) ) {
      kError() << "Write error" << endl;
    }
  }
}
