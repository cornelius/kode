/*
    This file is part of KXForms.

    Copyright (c) 2005,2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "mainwindow.h"

#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>

static const char description[] =
    I18N_NOOP("XML Forms Editor");

static KCmdLineOptions options[] =
{
  { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
  { "kxform <URL>", I18N_NOOP( "Form description" ), 0 },
  { "schema <URL>", I18N_NOOP( "XML Schema" ), 0 },
  KCmdLineLastOption
};

int main(int argc, char **argv)
{
  KAboutData about("kxforms", I18N_NOOP("KXForms"), "0.1", description,
    KAboutData::License_GPL, "(C) 2005 Cornelius Schumacher", 0, 0,
    "schumacher@kde.org");
  about.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );
  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);
  KApplication app;

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  MainWindow *widget = new MainWindow;

  widget->show();

  if ( args->isSet( "kxform" ) ) {
    QString form = args->getOption( "kxform" );
    widget->loadForm( KUrl::fromPath( form ) );
  }

  if ( args->count() == 1 ) {
    widget->load( KUrl::fromPath( args->arg( 0 ) ) );
  }

  return app.exec();
}
