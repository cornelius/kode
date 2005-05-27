/*
    This file is part of KDE.

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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef KWSDL_TYPEMAPPER_H
#define KWSDL_TYPEMAPPER_H

#include <schema/parser.h>
#include <qstringlist.h>

namespace KWSDL {

class TypeInfo
{
  public:
    TypeInfo()
    {}

    TypeInfo( const QString &_xsdType, const QString &_type, const QString &_header, bool _complex )
      : xsdType( _xsdType ), type( _type ), header( _header ), isComplex( _complex )
    {}

    QString xsdType;
    QString type;
    QString header;
    bool isComplex;
};

class TypeMapper
{
  public:
    TypeMapper();

    void setTypes( const Schema::XSDType::List &types );
    void setElements( const Schema::Element::PtrList &elements );
    void setParser( const Schema::Parser *parser );

    QString type( const Schema::XSDType *type ) const;
    QString type( const Schema::XSDType *type, bool &isComplex ) const;
    QString type( const Schema::Element *element ) const;
    QString type( const Schema::Element *element, bool &isComplex ) const;
    QString type( const Schema::Attribute *attribute ) const;
    QString type( const Schema::Attribute *attribute, bool &isComplex ) const;
    QString type( const QString &typeName ) const;
    QString type( const QString &typeName, bool &isComplex ) const;

    QStringList header( const Schema::XSDType *type ) const;
    QStringList header( const Schema::Element *element ) const;
    QMap<QString,QString> headerDec( const Schema::Element *element ) const;
    QStringList header( const Schema::Attribute *attribute ) const;
    QMap<QString,QString> headerDec( const Schema::Attribute *attribute ) const;
    QStringList header( const QString &typeName ) const;
    QMap<QString,QString> headerDec( const QString &typeName ) const;

    QString argument( const QString &name, const Schema::Element *element ) const;
    QString argument( const QString &name, const Schema::Attribute *attribute ) const;
    QString argument( const QString &name, const QString &typeName ) const;

  private:
    bool isBaseType( const QString& ) const;

    const Schema::Parser *mParser;
    Schema::XSDType::List mTypes;
    Schema::Element::PtrList mElements;
    QMap<QString, TypeInfo> mMap;
};

}

#endif
