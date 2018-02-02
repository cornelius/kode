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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include <QFile>
#include <QTextStream>

#include <iostream>

int main( int argc, char **argv )
{
  QCoreApplication app( argc, argv);
  QCoreApplication::setApplicationName("testholidays");
  QCoreApplication::setApplicationVersion("0.1");

  QCommandLineParser cmdLine;
  cmdLine.setApplicationDescription("Dump holidays to stdout");
  cmdLine.addHelpOption();
  cmdLine.addVersionOption();

  QCommandLineOption holidayfile(
              "holidayfile",
              QCoreApplication::translate("main", "Name of holiday XML file"));
  cmdLine.addOption(holidayfile);

  QCommandLineOption output(
              "holidayfile",
              QCoreApplication::translate("main", "Name of output XML file"));
  cmdLine.addOption(output);

  if (!cmdLine.parse(QCoreApplication::arguments())) {
    qDebug() << cmdLine.errorText();
    return -1;
  }

  if ( cmdLine.positionalArguments().count() < 1 ||
       cmdLine.positionalArguments().count() > 2 ) {
    qDebug() << "Wrong number of arguments";
    cmdLine.showHelp();
    return -1;
  }

  if ( cmdLine.positionalArguments().count() != 1 ) {
    qDebug( "Wrong number of arguments." );
    cmdLine.showHelp(-1);
  }

  QString filename = cmdLine.positionalArguments().at( 0 );

  bool ok;
  HolidayCalendar holidays = HolidayCalendar::parseFile( filename, &ok );

  if ( !ok ) {
    qDebug() <<"Parse error";
  } else {
    // TODO: Print data to stdout
  }

  if ( cmdLine.isSet( output) ) {
    QString out = cmdLine.value( "output" );
    if ( !holidays.writeFile( out ) ) {
      qDebug() <<"Write error";
    }
  }
}
