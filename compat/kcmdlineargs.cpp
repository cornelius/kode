#include "kcmdlineargs.h"

KCmdLineArgs *KCmdLineArgs::mParsedArgs = 0;
int KCmdLineArgs::argc = 0;
char **KCmdLineArgs::argv = 0;

void KCmdLineArgs::addCmdLineOptions( const KCmdLineOptions *options )
{
}

bool KCmdLineArgs::isSet( const QString & ) const
{
  return false;
}

QString KCmdLineArgs::getOption( const QString & )
{
  return QString();
}

int KCmdLineArgs::count() const
{
  return 0;
}

KUrl KCmdLineArgs::url( int index ) const
{
  return KUrl();
}

void KCmdLineArgs::init(int _argc,
               char **_argv,
               const KAboutData *about )
{
  argc = _argc;
  argv = _argv;
}

KCmdLineArgs *KCmdLineArgs::parsedArgs()
{
  return mParsedArgs;
}

int *KCmdLineArgs::qt_argc()
{
  return &argc;
}

char ***KCmdLineArgs::qt_argv()
{
  return &argv;
}
