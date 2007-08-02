/*
    This file is part of KXForms.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "remotefile.h"

#include <kdebug.h>
#include <kio/job.h>
#include <kio/jobuidelegate.h>

RemoteFile::RemoteFile( QWidget *parent )
  : mParent( parent ), mGetJob( 0 ), mPutJob( 0 ), mLoaded( false )
{
}

void RemoteFile::get( const KUrl &url )
{
  kDebug() <<"RemoteFile::get()" << url;

  mUrl = url;
  mLoaded = false;

  mGetJob = KIO::get( mUrl, false, false );
  mGetJob->ui()->setWindow( mParent );

  connect( mGetJob, SIGNAL( result( KJob * ) ),
           SLOT( slotJobResultGet( KJob * ) ) );
  connect( mGetJob, SIGNAL( data( KIO::Job *, const QByteArray & ) ),
           SLOT( slotJobDataGet( KIO::Job *, const QByteArray & ) ) );
}

void RemoteFile::put( const KUrl &url, const QString &data )
{
  mUrl = url;
  mData = data;

  mPutData = mData;
  mPutResult.clear();

  mPutJob = KIO::put( mUrl, -1, true, false, true );
  mPutJob->ui()->setWindow( mParent );
  connect( mPutJob, SIGNAL( dataReq( KIO::Job *, QByteArray & ) ),
    SLOT( slotDataReq( KIO::Job *, QByteArray & ) ) );
  connect( mPutJob, SIGNAL( result( KJob * ) ),
    SLOT( slotJobResultPut( KJob * ) ) );
  connect( mPutJob, SIGNAL( data( KIO::Job *, const QByteArray & ) ),
    SLOT( slotJobDataPut( KIO::Job *, const QByteArray & ) ) );
}

void RemoteFile::put( const QString &data )
{
  put( mUrl, data );
}

QString RemoteFile::data() const
{
  return mData;
}

QString RemoteFile::putResult() const
{
  return mPutResult;
}

void RemoteFile::setUrl( const KUrl &url )
{
  mUrl = url;
}

KUrl RemoteFile::url() const
{
  return mUrl;
}

void RemoteFile::slotJobResultGet( KJob *job )
{
  kDebug() <<"RemoteFile::slotJobResultGet()";

  if ( job->error() ) {
    static_cast<KIO::Job*>(job)->ui()->showErrorMessage();
    emit resultGet( false );
  } else {
    mLoaded = true;
    emit resultGet( true );
  }
  mGetJob = 0;
}

void RemoteFile::slotJobDataGet( KIO::Job *, const QByteArray &data )
{
  mData.append(
    QString::fromUtf8( QByteArray( data.data(), data.size() + 1 ).data() ) );
}

void RemoteFile::slotDataReq( KIO::Job *, QByteArray &data )
{
  if ( !mPutData.isEmpty() ) {
    data = mPutData.toUtf8();
    data.resize( data.size() - 1 );
    mPutData.clear();
  }
}

void RemoteFile::slotJobDataPut( KIO::Job *, const QByteArray &data )
{
  mPutResult.append( QString::fromUtf8(
    QByteArray( data.data(), data.size() + 1 ) ) );
}

void RemoteFile::slotJobResultPut( KJob *job )
{
  kDebug() <<"RemoteFile::slotJobResultPut()";

  if ( job->error() ) {
    static_cast<KIO::Job*>(job)->ui()->showErrorMessage();
    emit resultPut( false );
  } else {
    emit resultPut( true );
  }
  mPutJob = 0;
}

bool RemoteFile::isValid() const
{
  return mUrl.isValid();
}

bool RemoteFile::isLoading() const
{
  return mGetJob;
}

bool RemoteFile::isLoaded() const
{
  return mLoaded;
}

#include "remotefile.moc"
