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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "remotefile.h"

#include <kdebug.h>
#include <kio/job.h>

RemoteFile::RemoteFile( QWidget *parent )
  : mParent( parent )
{
}

void RemoteFile::get( const KUrl &url )
{
  mUrl = url;

  mGetJob = KIO::get( mUrl, false, false );

  connect( mGetJob, SIGNAL( result( KIO::Job * ) ),
           SLOT( slotJobResultGet( KIO::Job * ) ) );
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
  connect( mPutJob, SIGNAL( dataReq( KIO::Job *, QByteArray & ) ),
    SLOT( slotDataReq( KIO::Job *, QByteArray & ) ) );
  connect( mPutJob, SIGNAL( result( KIO::Job * ) ),
    SLOT( slotJobResultPut( KIO::Job * ) ) );
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

void RemoteFile::slotJobResultGet( KIO::Job *job )
{
  kDebug() << "RemoteFile::slotJobResultGet()" << endl;

  if ( job->error() ) {
    job->showErrorDialog( mParent );
    emit resultGet( false );
  } else {
    emit resultGet( true );
  }
  mGetJob = 0;
}

void RemoteFile::slotJobDataGet( KIO::Job *, const QByteArray &data )
{
  mData.append( QByteArray( data.data(), data.size() + 1 ) );
}

void RemoteFile::slotDataReq( KIO::Job *, QByteArray &data )
{
  if ( !mPutData.isEmpty() ) {
    data = mPutData.utf8();
    data.resize( data.size() - 1 );
    mPutData.clear();
  }
}

void RemoteFile::slotJobDataPut( KIO::Job *, const QByteArray &data )
{
  mPutResult.append( QString::fromUtf8(
    QByteArray( data.data(), data.size() + 1 ) ) );
}

void RemoteFile::slotJobResultPut( KIO::Job *job )
{
  kDebug() << "RemoteFile::slotJobResultPut()" << endl;

  if ( job->error() ) {
    job->showErrorDialog( mParent );
    emit resultPut( false );
  } else {
    emit resultPut( true );
  }
  mPutJob = 0;
}

#include "remotefile.moc"
