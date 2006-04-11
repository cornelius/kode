#include "kurl.h"

KUrl::KUrl()
{
}

KUrl::KUrl( const QString &str )
  : QUrl( str )
{
}

bool KUrl::isRelativeURL( const QString & )
{
  return false;
}

void KUrl::cleanPath()
{
}

QString KUrl::prettyURL() const
{
  return QString();
}
