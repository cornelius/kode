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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kdebug.h>

#include <QFile>
#include <QTextStream>

#include <iostream>

int main( int argc, char **argv )
{
  KAboutData aboutData( "testholidays", 0, ki18n("Dump holidays to stdout"),
                        "0.1" );
  KCmdLineArgs::init( argc, argv, &aboutData );

  KCmdLineOptions options;
  options.add("+holidayfile", ki18n("Name of holiday XML file"));
  options.add("output <file>", ki18n("Name of output file"));
  KCmdLineArgs::addCmdLineOptions( options );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() != 1 ) {
    args->usage( "Wrong number of arguments." );
  }

  QString filename = args->arg( 0 );

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
