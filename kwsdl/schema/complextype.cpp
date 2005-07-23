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

#include "complextype.h"

using namespace Schema;

ComplexType::ComplexType( const QString &nameSpace )
  : mNameSpace( nameSpace ), mType ( 0 ), mContentModel( COMPLEX ),
    mMixed( false ), mAnonymous( false ), mIsArray( false ), mContentType( 0 ),
    mTopLevelGroup( ALL ), mCurrentGroup( 0 ), mPreviousGroup( 0 ),
    mForwardElementRef( false ), mForwardAttributeRef( false )
{
  mBaseType.typeId = XSDType::ANYTYPE;
  mBaseType.derivation = Extension;
  mBaseType.type = 0;
}

ComplexType::ComplexType()
  : mType ( 0 ), mContentModel( COMPLEX ),
    mMixed( false ), mAnonymous( false ), mIsArray( false ), mContentType( 0 ),
    mTopLevelGroup( ALL ), mCurrentGroup( 0 ), mPreviousGroup( 0 ),
    mForwardElementRef( false ), mForwardAttributeRef( false )
{
  mBaseType.typeId = XSDType::ANYTYPE;
  mBaseType.derivation = Extension;
  mBaseType.type = 0;
}

ComplexType::~ComplexType()
{
}

QString ComplexType::name() const
{
  return mName;
}

QualifiedName ComplexType::qualifiedName() const
{
  QualifiedName qn( mName );
  qn.setNameSpace( mNameSpace );
  return qn;
}

void ComplexType::setDocumentation( const QString &documentation )
{
  mDocumentation = documentation;
}

QString ComplexType::documentation() const
{
  return mDocumentation;
}

int ComplexType::type() const
{
  return mType;
}

int ComplexType::contentModel() const
{
  return mContentModel;
}

int ComplexType::contentType() const
{
  return mContentType;
}

bool ComplexType::isSimple() const
{
  return false;
}

int ComplexType::attributeType( int index )
{
  return attribute( index )->type();
}

QString ComplexType::attributeName( int index )
{
  return attribute( index )->name();
}

int ComplexType::elementType( int index )
{
  return element( index )->type();
}

QString ComplexType::elementName( int index )
{
  return element( index )->name();
}

int ComplexType::numElements() const
{
  return mElements.count();
}

int ComplexType::numAttributes() const
{
  return mAttributes.count();
}

bool ComplexType::isAnonymous() const
{
  return mAnonymous;
}

void ComplexType::setBaseTypeName( const QString &baseTypeName )
{
  mBaseType.name = baseTypeName;
}

QString ComplexType::baseTypeName() const
{
  return mBaseType.name;
}

int ComplexType::baseType() const
{
  return mBaseType.typeId;
}

int ComplexType::baseDerivation() const
{
  return mBaseType.derivation;
}

ComplexType::Compositor ComplexType::topLevelGroup() const
{
  return mTopLevelGroup;
}

ComplexType::Compositor ComplexType::groupType( int groupId ) const
{
  return mGroups[ groupId ].type;
}

const Element *ComplexType::element( const QString &name )
{
  Element::List::ConstIterator it;
  for ( it = mElements.begin(); it != mElements.end(); ++it ) {
    if ( (*it).name() == name )
      return &(*it);
  }

  return 0;
}

Element *ComplexType::element( int id )
{
  if ( id < 0 || id >= (int)mElements.count() ) {
    qDebug( "tried to access non existent element" );
    return 0;
  }

  Element::List::Iterator it = mElements.begin();
  for ( int i = 0; i < (int)mElements.count(); ++i, ++it )
    if ( id == i )
      return &(*it);

  return 0;
}

const Attribute *ComplexType::attribute( const QString &name )
{
  Attribute::List::ConstIterator it;
  for ( it = mAttributes.begin(); it != mAttributes.end(); ++it ) {
    if ( (*it).name() == name )
      return &(*it);
  }

  return 0;
}

Attribute *ComplexType::attribute( int id )
{
  if ( id < 0 || id >= (int)mAttributes.count() ) {
    qDebug( "tried to access non existent attributes" );
    return 0;
  }

  Attribute::List::Iterator it = mAttributes.begin();
  for ( int i = 0; i < (int)mAttributes.count(); ++i, ++it )
    if ( id == i )
      return &(*it);

  return 0;
}

void ComplexType::setElements( const Element::List &elements )
{
  mElements = elements;
}

Element::List ComplexType::elements() const
{
  return mElements;
}

void ComplexType::setAttributes( const Attribute::List &attributes )
{
  mAttributes = attributes;
}

Attribute::List ComplexType::attributes() const
{
  return mAttributes;
}

bool ComplexType::isArray() const
{
  return mIsArray;
}

void ComplexType::setType( int type )
{
  mType = type;
}
void ComplexType::setIsArray( bool isArray )
{
  mIsArray = isArray;
}
void ComplexType::setBaseType( int type, Derivation derivation, const XSDType *ptr )
{
  mBaseType.typeId = type;
  mBaseType.derivation = derivation;
  mBaseType.type = ptr;
}

void ComplexType::setAnonymous( bool anonymous )
{
  mAnonymous = anonymous;
}

void ComplexType::setName( const QString &name )
{
  mName = name;
}

void ComplexType::setContentType( int contentType )
{
  mContentType = contentType;
}

void ComplexType::setContentModel( int model )
{
  mContentModel = model;
  if ( mContentModel == MIXED ) {
    mMixed = true;
    qDebug( "Mixed content not supported" );
  } else
    mMixed = false;
}

void ComplexType::addAttribute( const QString &name, int type_id, bool qualified,
                                const QString &defaultValue, const QString &fixedValue,
                                bool use )
{
  if ( type_id == 0 ) {
    qDebug( "ComplexType:addAttribute(): No type given for attribute" );
    return;
  }

  Attribute attribute( name, type_id, qualified, defaultValue, fixedValue, use );

  Attribute *attributePtr = (Attribute*)this->attribute( name );
  if ( attributePtr )
    *attributePtr = attribute;
  else
    mAttributes.append( attribute );
}


void ComplexType::addAttributeRef( const QualifiedName &name, bool qualified, bool use )
{
  addAttribute( name.localName(), XSDType::ANYTYPE, qualified, QString(), QString(), use );
  mForwardAttributeRef = true;
}


void ComplexType::addElement( const QString &name, int type_id, int minOccurs,
                              int maxOccurs, bool qualified,
                              const QString &defaultValue, const QString &fixedValue,
                              const QString &documentation )
{
  if ( type_id == 0 ) {
    qDebug( "ComplexType:addElement() :No type given for element " );
    return;
  }

  if ( mTopLevelGroup == ALL && maxOccurs > 1 && mIsArray == false ) {
    qDebug( "Inside an <all> group elements can occur only once" );
    return;
  }

  Element element( name, type_id, minOccurs, maxOccurs, qualified, defaultValue, fixedValue );
  element.setGroupId( mCurrentGroup );
  element.setDocumentation( documentation );
  Element *elementPtr = (Element*)this->element( name );

  if ( elementPtr )
    *elementPtr = element;
  else
    mElements.append( element );
}

void ComplexType::addElementRef( const QualifiedName &name, int minOccurs, int maxOccurs )
{
  addElement( name.localName(), XSDType::ANYTYPE, minOccurs, maxOccurs );
  mForwardElementRef = true;
}

void ComplexType::matchElementRef( const QString &name, Element &element )
{
  if ( mForwardElementRef ) {
    Element *elementPtr = (Element*)this->element( name );
    if ( elementPtr ) {
      // these values are specific to the occurrence of the element inside another XML type
      // so we shouldnt delete them
      int min = elementPtr->minOccurs(); 
      int max = elementPtr->maxOccurs();
      int gId = elementPtr->groupId();
      *elementPtr = element;
      elementPtr->setMinOccurs( min );
      elementPtr->setMaxOccurs( max );
      elementPtr->setGroupId( gId );
    }
  }
}

void ComplexType::matchAttributeRef( const QString &name, Attribute &attribute )
{
  if ( mForwardAttributeRef ) {
    Attribute *attributePtr = (Attribute*)this->attribute( name );
    if ( attributePtr )
      *attributePtr = attribute;
  }
}

void ComplexType::setCompositor( Compositor type, bool open, int minOccurs, int maxOccurs )
{
  if ( open ) {
    mPreviousGroup = mCurrentGroup++;
    if ( mPreviousGroup == 0 )
      mTopLevelGroup = type;
    else if ( mTopLevelGroup == this->ALL ) {
        qDebug( "This cannot occur inside a top level <all> compositor" );
        return;
    }

    if ( type == this->ALL && mPreviousGroup != 0 ) {
      qDebug( "<all> can occur only at the top level" );
      return;
    }

    if ( type == this->ALL && (minOccurs != 1 || maxOccurs != 1) ) {
      qDebug( "<all> can have min/max of only 1 " );
      return;
    }

    mGroups.append( CompositorStruct( type, minOccurs, maxOccurs ) );
  } else
    mCurrentGroup = mPreviousGroup;
}

bool ComplexType::checkOccurrences()
{
  Element::List::ConstIterator it;
  for ( it = mElements.begin(); it != mElements.end(); ++it )
    if ( (*it).occurrence() < (*it).minOccurs() )
      return false;

  return true;
}

void ComplexType::resetCounters()
{
  Element::List::Iterator it;
  for ( it = mElements.begin(); it != mElements.end(); ++it )
    (*it).setOccurrence( 0 );
}
