#include "klocale.h"

QString i18n( const QString &str )
{
  return str;
}

QString i18n( const QString &str, const QString &arg )
{
  return str.arg( arg );
}

QString i18n( const QString &str, const QString &arg, int, int )
{
  return str.arg( arg );
}
