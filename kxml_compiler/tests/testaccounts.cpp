#include "account.h"

#include <QFileInfo>

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

  if ( args->count() < 1 || args->count() > 2 ) {
    args->usage( "Wrong number of arguments." );
  }

  QString filename = args->arg( 0 );
  QString referenceFile;
  if ( args->count() == 2 ) {
    referenceFile = args->arg( 1 );
  } else {
    referenceFile = filename;
  }

  QString outputFile = "out_" + QFileInfo( filename ).fileName();

  kDebug() << "Comparing" << referenceFile << "and" << outputFile;

  bool ok;
  Account account = Account::parseFile( filename, &ok );
  
  if ( !ok ) {
    kError() <<"Parse error";
    return 1;
  } else {
    kDebug() << "DISPLAY NAME" << account.displayname();
    
    QDate date = account.creationDate().value();
    kDebug() << date;
    QDateTime dateTime = account.email().updatedAt();
    kDebug() << dateTime;
    if ( account.resources().resourceList().size() != 2 ) exit( 1 );
    if ( account.resources3().resource3List().size() != 3 ) exit( 1 );
    account.resources().resourceList().first().isValid();
    
    account.writeFile( outputFile );
    int exitCode = system( QString( "diff %1 %2" ).arg( referenceFile )
      .arg( outputFile ).toUtf8() );
    if ( exitCode != 0 ) exit( 1 );
  }
  
  return 0;
}
