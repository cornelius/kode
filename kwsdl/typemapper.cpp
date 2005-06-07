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

#include <qmap.h>

#include <schema/complextype.h>
#include <schema/simpletype.h>

#include "typemapper.h"

using namespace KWSDL;


TypeMapper::TypeMapper()
{
  mMap.insert( "any", TypeInfo( "any", "QString", "qstring.h" ) );
  mMap.insert( "anyURI", TypeInfo( "anyUri", "QString", "qstring.h" ) );
  mMap.insert( "base64Binary", TypeInfo( "base64Binary", "QByteArray", "qcstring.h" ) );
  mMap.insert( "binary", TypeInfo( "binary", "QByteArray", "qcstring.h" ) );
  mMap.insert( "boolean", TypeInfo( "boolean", "bool", "" ) );
  mMap.insert( "byte", TypeInfo( "byte", "char", "" ) );
  mMap.insert( "date", TypeInfo( "date", "QDate", "qdatetime.h" ) );
  mMap.insert( "dateTime", TypeInfo( "dateTime", "QDateTime", "qdatetime.h" ) );
  mMap.insert( "decimal", TypeInfo( "decimal", "float", "" ) );
  mMap.insert( "double", TypeInfo( "double", "double", "" ) );
  mMap.insert( "duration", TypeInfo( "duration", "QString", "qstring.h" ) ); // TODO: add duration class
  mMap.insert( "int", TypeInfo( "int", "int", "" ) );
  mMap.insert( "language", TypeInfo( "language", "QString", "qstring.h" ) );
  mMap.insert( "short", TypeInfo( "short", "short", "" ) );
  mMap.insert( "string", TypeInfo( "string", "QString", "qstring.h" ) );
  mMap.insert( "unsignedByte", TypeInfo( "unsignedByte", "unsigned char", "" ) );
  mMap.insert( "unsignedInt", TypeInfo( "unsignedInt", "unsigned int", "" ) );
}

void TypeMapper::setTypes( const Schema::XSDType::List &types )
{
  mTypes = types;
}

void TypeMapper::setElements( const Schema::Element::PtrList &elements )
{
  mElements = elements;
}

void TypeMapper::setTypeMap( const QMap<int, QString> &typeMap )
{
  mTypeMap = typeMap;
}

QString TypeMapper::type( const Schema::XSDType *type ) const
{
  QString typeName = type->name();
  typeName[ 0 ] = typeName[ 0 ].upper();

  return typeName;
}

QString TypeMapper::type( const Schema::Element *element ) const
{
  QString typeName = mTypeMap[ element->type() ];

  QString type;
  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() )
    type = it.data().type;

  if ( type.isEmpty() ) {
    type = typeName;
    type[ 0 ] = type[ 0 ].upper();
  }

  return type;
}

QString TypeMapper::type( const Schema::Attribute *attribute ) const
{
  QString typeName = mTypeMap[ attribute->type() ];

  QString type;
  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() )
    type = it.data().type;

  if ( type.isEmpty() ) {
    type = typeName;
    type[ 0 ] = type[ 0 ].upper();
  }

  return type;
}

QString TypeMapper::type( const QString &typeName ) const
{
  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() )
    return it.data().type;

  Schema::XSDType::List::ConstIterator typeIt;
  for ( typeIt = mTypes.begin(); typeIt != mTypes.end(); ++typeIt ) {
    if ( (*typeIt)->name() == typeName ) {
      if ( (*typeIt)->isSimple() ) {
        return type( static_cast<const Schema::SimpleType*>( *typeIt ) );
      } else {
        return type( static_cast<const Schema::ComplexType*>( *typeIt ) );
      }
    }
  }

  Schema::Element::PtrList::ConstIterator elemIt;
  for ( elemIt = mElements.begin(); elemIt != mElements.end(); ++elemIt ) {
    if ( (*elemIt)->name() == typeName ) {
      return type( *elemIt );
    }
  }

  return QString();
}

QStringList TypeMapper::header( const Schema::XSDType *type ) const
{
  return type->name().lower() + ".h";
}

QStringList TypeMapper::header( const Schema::Element *element ) const
{
  QString typeName = mTypeMap[ element->type() ];

  QStringList headers;

  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() ) {
    if ( !it.data().header.isEmpty() )
      headers.append( it.data().header );
  } else
    headers.append( typeName.lower() + ".h" );
  
  if ( element->maxOccurs() > 1 )
    headers.append( "qvaluelist.h" );

  return headers;
}

QMap<QString, QString> TypeMapper::headerDec( const Schema::Element *element ) const
{
  QString typeName = mTypeMap[ element->type() ];

  QMap<QString, QString> headers;

  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() ) {
    if ( !it.data().header.isEmpty() ) {
      if ( it.data().type == "QByteArray" )
        headers.insert( it.data().header, QString() );
      else
        headers.insert( it.data().header, it.data().type );
    }
  } else {
    typeName[ 0 ] = typeName[ 0 ].upper();
    headers.insert( typeName.lower() + ".h", typeName );
  }

  if ( element->maxOccurs() > 1 )
    headers.insert( "qvaluelist.h", QString() );

  return headers;
}

QStringList TypeMapper::header( const Schema::Attribute *attribute ) const
{
  QString typeName = mTypeMap[ attribute->type() ];

  QStringList headers;

  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() ) {
    if ( !it.data().header.isEmpty() )
      headers.append( it.data().header );
  } else
    headers.append( typeName.lower() + ".h" );
  
  return headers;
}

QMap<QString, QString> TypeMapper::headerDec( const Schema::Attribute *attribute ) const
{
  QString typeName = mTypeMap[ attribute->type() ];

  QMap<QString, QString> headers;

  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() ) {
    if ( !it.data().header.isEmpty() ) {
      if ( it.data().type == "QByteArray" )
        headers.insert( it.data().header, QString() );
      else
        headers.insert( it.data().header, it.data().type );
    }
  } else {
    typeName[ 0 ] = typeName[ 0 ].upper();
    headers.insert( typeName.lower() + ".h", typeName );
  }

  return headers;
}

QStringList TypeMapper::header( const QString &typeName ) const
{
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() )
    return it.data().header;

  const QString convertedType = type( typeName );
  if ( isBaseType( convertedType ) ) {
    for ( it = mMap.begin(); it != mMap.end(); ++it )
      if ( it.data().type == convertedType )
        return it.data().header;
  } else
    return typeName.lower() + ".h";

  return QStringList();
}

QMap<QString, QString> TypeMapper::headerDec( const QString &typeName ) const
{
  QString type( typeName );
  QMap<QString, QString> headers;

  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() ) {
    if ( !it.data().header.isEmpty() ) {
      if ( it.data().type == "QByteArray" )
        headers.insert( it.data().header, QString() );
      else
        headers.insert( it.data().header, it.data().type );
    }
  } else {
    type[ 0 ] = type[ 0 ].upper();
    headers.insert( typeName.lower() + ".h", type );
  }

  return headers;
}

QString TypeMapper::argument( const QString &name, const Schema::Element *element ) const
{
  QString typeName = type( element );

  if ( element->maxOccurs() > 1 )
    return "QValueList<" + typeName + ">* " + name;
  else
    return typeName + "* " + name;
}

QString TypeMapper::argument( const QString &name, const Schema::Attribute *attribute ) const
{
  return type( attribute ) + "* " + name;
}

QString TypeMapper::argument( const QString &name, const QString &typeName, bool isList ) const
{
  if ( isList ) {
    return "QValueList<" + type( typeName ) + ">* " + name;
  } else {
    return type( typeName ) + "* " + name;
  }
}

bool TypeMapper::isBaseType( const QString &type ) const
{
  QMap<QString, TypeInfo>::ConstIterator it;
  for ( it = mMap.begin(); it != mMap.end(); ++it )
    if ( it.data().type == type )
      return true;

  return false;
}
