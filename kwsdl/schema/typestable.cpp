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
 
#include "typestable.h"

#include <kdebug.h>

using namespace Schema;

TypesTable::TypesTable()
{
    mCurrentId = XSDType::ANYURI + 1;

//map of simple types
    mBasicTypes["string"] = XSDType::STRING;
    mBasicTypes["integer"] = XSDType::INTEGER;
    mBasicTypes["int"] = XSDType::INT;
    mBasicTypes["byte"] = XSDType::BYTE;
    mBasicTypes["unsignedByte"] = XSDType::UBYTE;
    mBasicTypes["positiveInteger"] = XSDType::POSINT;
    mBasicTypes["unsignedInt"] = XSDType::UINT;
    mBasicTypes["long"] = XSDType::LONG;
    mBasicTypes["unsignedLong"] = XSDType::ULONG;
    mBasicTypes["short"] = XSDType::SHORT;
    mBasicTypes["unsignedShort"] = XSDType::USHORT;
    mBasicTypes["decimal"] = XSDType::DECIMAL;
    mBasicTypes["float"] = XSDType::FLOAT;
    mBasicTypes["double"] = XSDType::DOUBLE;
    mBasicTypes["boolean"] = XSDType::BOOLEAN;
    mBasicTypes["time"] = XSDType::TIME;
    mBasicTypes["dateTime"] = XSDType::DATETIME;
    mBasicTypes["date"] = XSDType::DATE;
    mBasicTypes["token"] = XSDType::TOKEN;
    mBasicTypes["QName"] = XSDType::QNAME;
    mBasicTypes["NCName"] = XSDType::NCNAME;
    mBasicTypes["NMTOKEN"] = XSDType::NMTOKEN;
    mBasicTypes["NMTOKENS"] = XSDType::NMTOKENS;
    mBasicTypes["base64Binary"] = XSDType::BASE64BIN;
    mBasicTypes["hexBinary"] = XSDType::HEXBIN;
    mBasicTypes["anyType"] = XSDType::ANYTYPE;
    mBasicTypes["any"] = XSDType::ANY;
    mBasicTypes["anyURI"] = XSDType::ANYURI;
}

TypesTable::~TypesTable()
{
  clear();
}

void TypesTable::clear()
{
  QMap<QString, int>::Iterator it;
  for ( it = mUserTypes.begin(); it != mUserTypes.end(); ++it )
    delete typePtr( it.data() );

  mUserTypes.clear();
  mTypes.clear();
}

int TypesTable::numExtRefs() const
{
  return mExternRefs.count();
}

QualifiedName TypesTable::extRefName( int index ) const
{
  return mExternRefs[ index ].qname;
}

int TypesTable::extRefType( int index ) const
{
  return mExternRefs[ index ].localTypeId;
}

int TypesTable::addType( XSDType *type )
{
  QualifiedName qn = type->qualifiedName();
  QString type_name( qn.localName() );

  int i = 0;

  if ( type_name.isEmpty() ) {
    // create an anonymous type name
    type_name.sprintf( "type%d", mTypes.count() );
    type->setName( type_name );
    type->setAnonymous( true );  //auto generated name
  }

  // add the typename and its id  to the map
  if ( (i = mUserTypes[type_name]) != 0 ) {
    // this type was refernced earlier.
    mTypes[i - (XSDType::ANYURI + 1)] = type;
    type->setType( i );
    return i;
  } else {
    mUserTypes[ type_name ] = mCurrentId;
    type->setType( mCurrentId );
    mTypes.append( type );
    mCurrentId++;
    return mCurrentId - 1;
  }
}

int TypesTable::typeId( const QualifiedName &name, bool create )
{
  int typeId;

  if ( name.nameSpace() == SchemaUri ) { // this is one of the basic types
    typeId = mBasicTypes[ name.localName() ];
    if ( typeId == 0 ) // if this is a basic type which is not mapped, treat as string
      typeId = XSDType::STRING;
  } else if ( name.nameSpace() == mNameSpace )
    typeId = mUserTypes[ name.localName() ];
  else { // the type does not belong to this schema
    return 0;
  }

  if ( typeId == 0 && create ) {
    // handle forward reference
    // create an id and return its value
    mUserTypes[name.localName()] = mCurrentId;
    mTypes.append( 0 );
    mCurrentId++;
    typeId = mCurrentId - 1;
  }

  return typeId;
}

QString TypesTable::typeName( int id ) const
{
  if ( id < 0 )
    return QString();

  QMap<QString, int>::ConstIterator it;

  if ( id >= 0 && id <= XSDType::ANYURI ) {
    for ( it = mBasicTypes.begin(); it != mBasicTypes.end(); ++it )
      if ( id == it.data() )
        return it.key();
  }

  for ( it = mUserTypes.begin(); it != mUserTypes.end(); ++it )
    if ( id == it.data() )
      return it.key();

  return "<unknown type>";
}

int TypesTable::addExternalTypeId( const QualifiedName &type, XSDType *pType )
{
  for ( int i = 0; i < (int)mExternRefs.count(); i++ )
    if ( mExternRefs[i].qname == type )
      return mExternRefs[i].localTypeId;

  struct ExternRef ref;
  ref.qname = (pType) ? pType->qualifiedName() : type;
  ref.localTypeId = mCurrentId;
  mExternRefs.append( ref );

  mTypes.append( pType );
  mCurrentId++;

  return ref.localTypeId;
}

// adds a type into a type table for a given type id
// used for types present in imported schemas but referenced in current schema
int TypesTable::addExtType( XSDType *type, int localId )
{
  int index = localId - XSDType::ANYURI - 1;
  if ( index >= (int)mTypes.count() )
    return 0;

  mTypes[ index ] = type;
  return localId;
}

bool TypesTable::detectUndefinedTypes()
{
  for ( int i = 0; i < (int)mTypes.count(); i++ )
    if ( mTypes[i] == 0 )
      return true;

  return false;
}

void TypesTable::resolveForwardElementRefs( const QString &name, Element &element )
{
  for ( int i = 0; i < (int)mTypes.count(); i++ )
    if ( mTypes[i] != 0 ) {
      if ( !mTypes[i]->isSimple() ) {
        ComplexType *ct = (ComplexType*)mTypes[i];
        ct->matchElementRef( name, element );
      }
    }
}

void TypesTable::resolveForwardAttributeRefs( const QString &name, Attribute &attribute )
{
  for ( int i = 0; i < (int)mTypes.count(); i++ )
    if ( mTypes[i] != 0 ) {
      if ( !mTypes[i]->isSimple() ) {
        ComplexType *ct = (ComplexType*)mTypes[i];
        ct->matchAttributeRef( name, attribute );
      }
    }
}

XSDType *TypesTable::typePtr( int id ) const
{
  // this is a basic XSD type
  if ( id < XSDType::ANYURI + 1 || id > XSDType::ANYURI + (int)mTypes.count() )
    return 0;

  return mTypes[ id - (SimpleType::ANYURI + 1) ];
}

int TypesTable::numTypes() const
{
  return mTypes.count();
}

void TypesTable::setTargetNamespace( const QString &nameSpace )
{
  mNameSpace = nameSpace;
}

QString TypesTable::targetNamespace() const
{
  return mNameSpace;
}
