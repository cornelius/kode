#include "kurl.h"

KUrl::KUrl()
{
}

KUrl::KUrl( const QString &str )
  : QUrl( str )
{
}

bool KUrl::isRelativeUrl( const QString & )
{
  return false;
}

void KUrl::cleanPath()
{
}

QString KUrl::prettyUrl() const
{
  return QString();
}
