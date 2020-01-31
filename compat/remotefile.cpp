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

#include "remotefile.h"

#include "kdebug.h"

#include <QWidget>

RemoteFile::RemoteFile(QWidget *parent) : QObject(parent) {}

void RemoteFile::get(const KUrl &)
{
    kError() << "RemoteFile::get() not implemented";
}

void RemoteFile::put(const KUrl &, const QString &data)
{
    kError() << "RemoteFile::put() not implemented";
}

void RemoteFile::put(const QString &data)
{
    put(mUrl, data);
}

QString RemoteFile::data() const
{
    return mData;
}

QString RemoteFile::putResult() const
{
    return mPutResult;
}

void RemoteFile::setUrl(const KUrl &url)
{
    mUrl = url;
}

KUrl RemoteFile::url() const
{
    return mUrl;
}

bool RemoteFile::isValid() const
{
    return mUrl.isValid();
}

bool RemoteFile::isLoading() const
{
    return false;
}

bool RemoteFile::isLoaded() const
{
    return false;
}
