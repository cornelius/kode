#include "mainwindow.h"

#include "reference.h"

#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>

using namespace KXForms;

static const char description[] =
    I18N_NOOP("References Test Program");

static KCmdLineOptions options[] =
{
  { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
  { "kxform <URL>", I18N_NOOP( "Form description" ), 0 },
  { "schema <URL>", I18N_NOOP( "XML Schema" ), 0 },
  KCmdLineLastOption
};

void checkReference( const QString &str )
{
  Reference ref( str );

  QString s = ref.toString();

  if ( s == str ) {
    kdDebug() << "OK  REF: " << str << endl;
  } else {
    kdDebug() << "ERR REF: " << str << " GOT: " << s << endl;
  }
}

int main(int argc, char **argv)
{
  KAboutData about("testreferences", I18N_NOOP("TestReferences"), "0.1",
    description,
    KAboutData::License_GPL, "(C) 2005 Cornelius Schumacher", 0, 0,
    "schumacher@kde.org");
  about.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );
  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app( false, false );

  checkReference( "/one/two/three" );
  checkReference( "relative" );
  checkReference( "/one/two[2]" );
  checkReference( "/one/two[2]/three[2]" );
  checkReference( "/one/@attribute" );
  checkReference( "relative/more/more" );

  Reference r1 = "one";
  kdDebug() << "R1: " << r1.toString() << endl;
  Reference r2 = "two";
  Reference r3 = r1 + r2;
  kdDebug() << "R3: " << r3.toString() << endl;
  
  Reference r4 = "one/twox";
  
  if ( r3 == r4 ) kdDebug() << "YES" << endl;
  else kdDebug() << "NO" << endl;
}

