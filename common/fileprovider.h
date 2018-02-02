/*
    This file is part of KDE

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

#ifndef FILEPROVIDER_H
#define FILEPROVIDER_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

#include "kode_export.h"

namespace KIO {
class Job;
}
class KJob;
class KXMLCOMMON_EXPORT FileProvider : QObject
{
  Q_OBJECT

  public:
    FileProvider(QObject *parent = NULL);

    bool get( const QString &url, QString &target );
    void cleanUp();

  private slots:
    void downloadFinished( QNetworkReply *reply );

  private:
    QString mFileName;
    QByteArray mData;
    QNetworkAccessManager mManager;
};

#endif
