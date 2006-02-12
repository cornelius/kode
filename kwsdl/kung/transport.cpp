/*
    This file is part of Kung.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "transport.h"
#include <kdebug.h>

Transport::Transport( const QString &url )
{
  mUrl = url;
  qDebug( "url=%s", url.toLatin1() );
}

void Transport::query( const QString &xml )
{
  mData.truncate( 0 );
  
  QByteArray postData;
  QDataStream stream( &postData, QIODevice::WriteOnly );
  stream.writeRawBytes( xml.toUtf8(), xml.toUtf8().length() );
  
  KIO::TransferJob* job = KIO::http_post( KUrl( mUrl ), postData, false );
  if ( !job ) {
    kWarning() << "Unable to create KIO job for " << mUrl << endl;
    return;
  }
  
  job->addMetaData( "UserAgent", "KDE Kung" );
  job->addMetaData( "content-type", "Content-Type: text/xml; charset=utf-8" );
  
  connect( job, SIGNAL( data( KIO::Job*, const QByteArray& ) ), this, SLOT( slotData( KIO::Job*, const QByteArray& ) ) );
  connect( job, SIGNAL( result( KIO::Job* ) ), this, SLOT( slotResult( KIO::Job* ) ) );
}

void Transport::slotData( KIO::Job*, const QByteArray &data )
{
  unsigned int oldSize = mData.size();
  mData.resize( oldSize + data.size() );
  memcpy( mData.data() + oldSize, data.data(), data.size() );
}

void Transport::slotResult( KIO::Job* job )
{
  if ( job->error() != 0 )
    emit error( job->errorText() );
  else
    emit result( QString::fromUtf8( mData.data(), mData.size() ) );
}


#include "transport.moc"

