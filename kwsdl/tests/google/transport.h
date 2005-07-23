/*
    This file is part of KDE.

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
    
    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/
#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <qobject.h>
#include <kio/job.h>

class Transport : public QObject
{
  Q_OBJECT

  public:
    Transport( const QString &url );
    void query( const QString &xml );
  
  signals:
    void result( const QString &xml );
  
  private slots:
    void slotData( KIO::Job*, const QByteArray &data );
    void slotResult( KIO::Job* job );
  
  private:
    QString mUrl;
    QByteArray mData;
};

#endif
