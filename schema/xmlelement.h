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

#ifndef XML_ELEMENT_H
#define XML_ELEMENT_H

#include <qstring.h>

#include <common/qname.h>

class XmlElement
{
  public:
    XmlElement();
    XmlElement( const QString &nameSpace );
    ~XmlElement();

    void setName( const QString &name );
    QString name() const;

    void setNameSpace( const QString &nameSpace );
    QString nameSpace() const;

    QName qualifiedName() const;

  private:
    QString mName;
    QString mNameSpace;
};

#endif
