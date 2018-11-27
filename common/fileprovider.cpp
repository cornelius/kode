/*
    This file is part of KDE Schema Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
 */

#include <unistd.h>

#include <QCoreApplication>
#include <QEventLoop>
#include <QFile>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTemporaryFile>

#include "fileprovider.h"

FileProvider::FileProvider(QObject *parent)
  : QObject( parent )
{
    connect(&mManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(downloadFinished(QNetworkReply*)));
}

bool FileProvider::get( const QString &url, QString &target )
{
  if ( !mFileName.isEmpty() )
    cleanUp();

  if ( target.isEmpty() ) {
    QTemporaryFile tmpFile;
    tmpFile.setAutoRemove(false);
    tmpFile.open();
    target = tmpFile.fileName();
    mFileName = target;
  }

  qDebug( "Downloading external schema '%s'", qPrintable( url ) );

  QNetworkRequest request = QNetworkRequest(QUrl(url));
  QNetworkReply *reply = mManager.get(request);

  while ( !reply->isFinished() ) {
    QCoreApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
    usleep( 500 );
  }

  return (reply->error() == QNetworkReply::NoError);
}

void FileProvider::cleanUp()
{
  ::unlink( QFile::encodeName( mFileName ) );
  mFileName = QString();
}


void FileProvider::downloadFinished(QNetworkReply *reply )
{
  if ( reply->error() != QNetworkReply::NoError) {
    qDebug( "%s", qPrintable( reply->errorString() ) );
    return;
  }

  QFile file( mFileName );
  if ( !file.open( QIODevice::WriteOnly ) ) {
    qDebug( "Unable to create temporary file" );
    return;
  }

  qDebug( "Download successful" );
  file.write( reply->readAll() );
  file.close();
}

