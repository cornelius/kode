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

#ifndef SCHEMA_TYPESTABLE_H
#define SCHEMA_TYPESTABLE_H

#include <qmap.h>
#include <qstring.h>

#include "complextype.h"
#include "simpletype.h"

namespace Schema {

class TypesTable
{
  public:
    TypesTable();
    ~TypesTable();

    void clear();

    int addType( XSDType *type );
    int addExtType( XSDType *type, int id );

    int typeId( const QualifiedName &name, bool create = false );

    QString typeName( int id ) const;

    int addExternalTypeId( const QualifiedName &type, XSDType *type );

    int numExtRefs() const;
    QualifiedName extRefName( int index ) const;
    int extRefType( int index ) const;

    void resolveForwardElementRefs( const QString &name, Element &element );
    void resolveForwardAttributeRefs( const QString &name, Attribute &attribute );

    XSDType *typePtr( int id ) const;

    int numTypes() const;

    bool detectUndefinedTypes();

    void setTargetNamespace( const QString &nameSpace );
    QString targetNamespace() const;

  private:
    QValueList<XSDType*> mTypes;

    //maintains a map of all user defined type names and their ids
    QMap<QString, int> mUserTypes;
    QMap<QString, int> mBasicTypes;

    int mCurrentId;

    QString mNameSpace;

    struct ExternRef
    {
      int localTypeId;
      QualifiedName qname;
    };

    QValueList<struct ExternRef> mExternRefs;
};

}

#endif
