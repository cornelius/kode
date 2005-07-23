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

#ifndef SCHEMA_QUALIFIEDNAME_H
#define SCHEMA_QUALIFIEDNAME_H

#include <qstring.h>
#include <qvaluelist.h>

namespace Schema {

class QualifiedName
{
  public:
    typedef QValueList<QualifiedName> List;

    QualifiedName();
    QualifiedName( const QString &name );

    void operator=( const QString &name );

    QString localName() const;
    QString prefix() const;

    void setNameSpace( const QString &nameSpace );
    QString nameSpace() const;

    bool operator==( const QualifiedName& ) const;

  private:
    void parse( const QString& );

    QString mNameSpace;
    QString mLocalName;
    QString mPrefix;
};

}

#endif
