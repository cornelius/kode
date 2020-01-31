/*
    This file is part of Kode.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef KXFORMS_REMOTEFILE_H
#define KXFORMS_REMOTEFILE_H

#include "kurl.h"

#include <QObject>
#include <QString>

class QWidget;

class RemoteFile : public QObject
{
    Q_OBJECT
public:
    RemoteFile(QWidget *parent = 0);

    void get(const KUrl &);
    void put(const KUrl &, const QString &data);
    void put(const QString &data);

    QString data() const;

    QString putResult() const;

    void setUrl(const KUrl &);
    KUrl url() const;

    bool isValid() const;

    bool isLoading() const;
    bool isLoaded() const;

signals:
    void resultGet(bool ok);
    void resultPut(bool ok);

private:
    QWidget *mParent;

    KUrl mUrl;
    QString mData;

    QString mPutData;
    QString mPutResult;

    bool mLoaded;
};

#endif
