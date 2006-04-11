#include "kapplication.h"

#include "kcmdlineargs.h"

KApplication::KApplication()
  : QApplication( *KCmdLineArgs::qt_argc(), *KCmdLineArgs::qt_argv(), true )
{
}
