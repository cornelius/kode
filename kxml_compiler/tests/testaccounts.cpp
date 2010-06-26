#include "account.h"

#include <KAboutData>
#include <KCmdLineArgs>
#include <KDebug>

int main( int argc, char **argv )
{
  KAboutData aboutData( "testaccounts", 0, ki18n("Test program"),
                        "0.1" );
  KCmdLineArgs::init( argc, argv, &aboutData );

  KCmdLineOptions options;
  options.add("+xmlfile", ki18n("Name of XML file"));
  KCmdLineArgs::addCmdLineOptions( options );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() != 1 ) {
    args->usage( "Wrong number of arguments." );
  }

  QString filename = args->arg( 0 );

  bool ok;
  Account account = Account::parseFile( filename, &ok );
  
  if ( !ok ) {
    kError() <<"Parse error";
    return 1;
  } else {
    kDebug() << "DISPLAY NAME" << account.displayname();
    account.writeFile( "account.out.xml" );
    int exitCode = system( QString( "diff %1 account.out.xml" ).arg( filename ).toUtf8() );
    if ( exitCode != 0 ) exit( 1 );
  }
  
  return 0;
}
