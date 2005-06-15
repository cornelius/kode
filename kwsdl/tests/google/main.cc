#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kmdcodec.h>

#include "googlesearch.hh"

int main( int argc, char **argv )
{
  KAboutData aboutData( "kgooglesearch", "KDE Google Search", "0.1", "", KAboutData::License_GPL );
  aboutData.addAuthor( "Tobias Koenig", 0, "tokoe@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );

  KApplication app( false, false );

  GoogleSearch search;

  search.googleSearch( "foobar", 0, 10, true, "", false, "", "latin1", "latin1" );
  search.spellingSuggestion( "guugel" );

  return app.exec();
}
