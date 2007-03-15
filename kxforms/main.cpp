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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "mainwindow.h"

#include "manager.h"
#include "guihandler.h"
#include "guihandlerdialogs.h"
#include "guihandlerflat.h"
#include "prefs.h"

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>

#include <QDir>
#include <QByteArray>

#include <unistd.h>
#include <limits.h>

static const char description[] =
    I18N_NOOP("XML Forms Editor");

static KCmdLineOptions options[] =
{
  { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
  { "kxform <URL>", I18N_NOOP( "KXForms description" ), 0 },
  { "schema <URL>", I18N_NOOP( "XML Schema" ), 0 },
  { "ugh <URL>", I18N_NOOP( "UI Generation Hints" ), 0 },
  { "xml <URL>", I18N_NOOP( "XML file" ), 0 },
  { "dialogs", I18N_NOOP( "Use dialogs" ), 0 },
  { "developer", I18N_NOOP( "Use developer mode of user interface" ), 0 },
  { "vertical-list-buttons", I18N_NOOP( "Use vertical list buttons" ), 0 },
  { "layout-style <layout-style>", I18N_NOOP( "One of grid, vertical. Arrange elements vertically or in a grid." ), 0 },
  KCmdLineLastOption
};

KUrl makeURL( const QString &arg )
{
   if (!QDir::isRelativePath( arg ))
   {
      KUrl result;
      result.setPath( arg );
      return result; // Absolute path.
   }

   if ( !KUrl::isRelativeUrl( arg ) )
     return KUrl( arg ); // Argument is a URL

  KUrl result;
  char cwd[ PATH_MAX + 1 ];
  getcwd( cwd, PATH_MAX );
  result.setPath( QFile::decodeName( QByteArray( cwd ) ) + '/' + arg );
  result.cleanPath();
  return result;  // Relative path
}

int main(int argc, char **argv)
{
  KAboutData about("kxforms", I18N_NOOP("KXForms"), "0.1", description,
    KAboutData::License_GPL, "(C) 2005 Cornelius Schumacher", 0, 0,
    "schumacher@kde.org");
  about.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );
  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);
#if 1
  KApplication app;
#else
  QApplication app( argc, argv );
#endif

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  Prefs::setDeveloperMode( args->isSet( "developer" ) );
  Prefs::setVerticalListButtons( args->isSet( "vertical-list-buttons" ) );

#if 1
  // Hack: force a decent font
  QFont f( "Arial", 11 );
  QApplication::setFont( f );
#endif

  MainWindow *mainWindow = new MainWindow;
  KXForms::GuiHandler *guiHandler;
  if ( args->isSet( "dialogs" ) ) {
    guiHandler = new KXForms::GuiHandlerDialogs( mainWindow->formsManager() );
  } else {
    guiHandler = new KXForms::GuiHandlerFlat( mainWindow->formsManager() );
  }

  if ( args->isSet( "layout-style" ) ) {
    if( args->getOption( "layout-style" ).toLower() == "grid" ) {
      guiHandler->setLayoutStyle( KXForms::GuiHandler::Grid );
    }
    else {
      guiHandler->setLayoutStyle( KXForms::GuiHandler::Vertical );
    }
  }

  mainWindow->show();

  if ( args->isSet( "schema" ) && args->isSet( "kxform" ) ) {
    kWarning() << "KXForm will be generated from schema. Ignoring --kxform "
      << "option" << endl;
  }

  if ( args->isSet( "ugh" ) ) {
    QString ugh = args->getOption( "ugh" );
    mainWindow->loadHints( makeURL( ugh ) );
  }

  if ( args->isSet( "schema" ) ) {
    QString schema = args->getOption( "schema" );
    mainWindow->loadSchema( makeURL( schema ) );
  } else {
    if ( args->isSet( "kxform" ) ) {
      QString form = args->getOption( "kxform" );
      mainWindow->loadForm( makeURL( form ) );
    }
  }

  if ( args->isSet( "xml" ) ) {
    QString xml = args->getOption( "xml" );
    mainWindow->load( xml );
  }

  if ( args->count() == 1 ) {
    mainWindow->load( args->url( 0 ) );
  }

  return app.exec();
}
