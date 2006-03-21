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
