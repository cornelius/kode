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

#include "typemapper.h"

using namespace KWSDL;


TypeMapper::TypeMapper()
{
  mMap.insert( "string", TypeInfo( "string", "QString", "qstring.h", true ) );
  mMap.insert( "byte", TypeInfo( "byte", "char", "", false ) );
  mMap.insert( "unsignedByte", TypeInfo( "unsignedByte", "unsigned char", "", false ) );
  mMap.insert( "binary", TypeInfo( "binary", "QByteArray", "qcstring.h", true ) );
  mMap.insert( "base64Binary", TypeInfo( "base64Binary", "QByteArray", "qcstring.h", true ) );
  mMap.insert( "boolean", TypeInfo( "boolean", "bool", "", false ) );
  mMap.insert( "int", TypeInfo( "int", "int", "", false ) );
  mMap.insert( "short", TypeInfo( "short", "short", "", false ) );
  mMap.insert( "unsignedInt", TypeInfo( "unsignedInt", "unsigned int", "", false ) );
  mMap.insert( "double", TypeInfo( "double", "double", "", false ) );
  mMap.insert( "date", TypeInfo( "date", "QDate", "qdatetime.h", true ) );
  mMap.insert( "dateTime", TypeInfo( "dateTime", "QDateTime", "qdatetime.h", true ) );
  mMap.insert( "anyURI", TypeInfo( "anyUri", "QString", "qstring.h", true ) );
  mMap.insert( "decimal", TypeInfo( "decimal", "float", "", false ) );
  mMap.insert( "language", TypeInfo( "language", "QString", "qstring.h", true ) );
  mMap.insert( "duration", TypeInfo( "duration", "QString", "qstring.h", true ) ); // TODO: add duration class
  mMap.insert( "any", TypeInfo( "any", "QString", "qstring.h", true ) );
}

void TypeMapper::setTypes( const Schema::XSDType::List &types )
{
  mTypes = types;
}

void TypeMapper::setElements( const Schema::Element::PtrList &elements )
{
  mElements = elements;
}

void TypeMapper::setParser( const Schema::Parser *parser )
{
  mParser = parser;
}

QString TypeMapper::type( const Schema::XSDType *type ) const
{
  bool isComplex;
  return this->type( type, isComplex );
}

QString TypeMapper::type( const Schema::XSDType *type, bool &isComplex ) const
{
  isComplex = true;

  QString typeName = type->name();
  typeName[ 0 ] = typeName[ 0 ].upper();

  return typeName;
}

QString TypeMapper::type( const Schema::Element *element ) const
{
  bool isComplex;
  return type( element, isComplex );
}

QString TypeMapper::type( const Schema::Element *element, bool &isComplex ) const
{
  QString typeName = mParser->typeName( element->type() );

  QString type;
  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() )
    type = it.data().type;

  if ( type.isEmpty() ) {
    isComplex = true;
    type = typeName;
    type[ 0 ] = type[ 0 ].upper();
  } else
    isComplex = it.data().isComplex;

  if ( element->maxOccurs() > 1 )
    isComplex = true;

  return type;
}

QString TypeMapper::type( const Schema::Attribute *attribute ) const
{
  bool isComplex;
  return type( attribute, isComplex );
}

QString TypeMapper::type( const Schema::Attribute *attribute, bool &isComplex ) const
{
  QString typeName = mParser->typeName( attribute->type() );

  QString type;
  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() )
    type = it.data().type;

  if ( type.isEmpty() ) {
    isComplex = true;
    type = typeName;
    type[ 0 ] = type[ 0 ].upper();
  } else
    isComplex = it.data().isComplex;

  return type;
}

QString TypeMapper::type( const QString &typeName ) const
{
  bool isComplex;
  return type( typeName, isComplex );
}

QString TypeMapper::type( const QString &typeName, bool &isComplex ) const
{
  // check basic types
  QMap<QString, TypeInfo>::ConstIterator it = mMap.find( typeName );
  if ( it != mMap.end() ) {
    isComplex = it.data().isComplex;
    return it.data().type;
  }

  Schema::XSDType::List::ConstIterator typeIt;
  for ( typeIt = mTypes.begin(); typeIt != mTypes.end(); ++typeIt ) {
    if ( (*typeIt)->name() == typeName ) {
      if ( (*typeIt)->isSimple() ) {
        return type( static_cast<const Schema::SimpleType*>( *typeIt ), isComplex );
      } else {
        return type( static_cast<const Schema::ComplexType*>( *typeIt ), isComplex );
      }
    }
  }

  Schema::Element::PtrList::ConstIterator elemIt;
  for ( elemIt = mElements.begin(); elemIt != mElements.end(); ++elemIt ) {
    if ( (*elemIt)->name() == typeName ) {
      return type( *elemIt, isComplex );
    }
  }

  isComplex = false;
  return QString();
}

QStringList TypeMapper::header( const Schema::XSDType *type ) const
{
  return type->name().lower() + ".h";
}

QStringList TypeMapper::header( const Schema::Element *element ) const
{
  QString typeName = mParser->typeName( element->type() );

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
  QString typeName = mParser->typeName( element->type() );

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
  QString typeName = mParser->typeName( attribute->type() );

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
  QString typeName = mParser->typeName( attribute->type() );

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
  bool isComplex = false;

  QString typeName = type( element, isComplex );

  if ( element->maxOccurs() > 1 )
    return "QValueList<" + type( element ) + ">* " + name;
  else
    return type( element ) + "* " + name;
}

QString TypeMapper::argument( const QString &name, const Schema::Attribute *attribute ) const
{
  bool isComplex = false;

  QString typeName = type( attribute, isComplex );

  return type( attribute ) + "* " + name;
}

QString TypeMapper::argument( const QString &name, const QString &typeName ) const
{
  QString type = this->type( typeName );
  return type + "* " + name;
}

QString TypeMapper::argument( const QString &name, const QString &typeName, bool isList ) const
{
  QString type = this->type( typeName );
  if ( isList ) {
    return "QValueList<" + type + ">* " + name;
  } else {
    return type + "* " + name;
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
