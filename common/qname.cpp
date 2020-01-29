/*
    This file is part of KDE Schema Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>
                       based on wsdlpull parser by Vivek Krishna

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

#include "qname.h"

QName::QName() {}

QName::QName(const QString &name)
{
    parse(name);
}

QName::QName(const QString &nameSpace, const QString &localName)
    : mNameSpace(nameSpace), mLocalName(localName)
{
}

void QName::operator=(const QString &name)
{
    parse(name);
}

QString QName::localName() const
{
    return mLocalName;
}

QString QName::prefix() const
{
    return mPrefix;
}

QString QName::qname() const
{
    if (mPrefix.isEmpty())
        return mLocalName;
    else
        return mPrefix + ':' + mLocalName;
}

void QName::setNameSpace(const QString &nameSpace)
{
    mNameSpace = nameSpace;
}

QString QName::nameSpace() const
{
    return mNameSpace;
}

bool QName::operator==(const QName &qname) const
{
    return (qname.nameSpace() == mNameSpace && qname.localName() == mLocalName);
}

bool QName::operator!=(const QName &qname) const
{
    return !operator==(qname);
}

bool QName::isEmpty() const
{
    return (mNameSpace.isEmpty() && mLocalName.isEmpty());
}

void QName::parse(const QString &str)
{
    int pos = str.indexOf(':');
    if (pos != -1) {
        mPrefix = str.left(pos);
        mLocalName = str.mid(pos + 1);
    } else
        mLocalName = str;
}
