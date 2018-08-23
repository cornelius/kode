/*
    This file is part of KDE.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "schema.h"

#include <QDebug>

using namespace Schema;

Document::Document()
{
}


void Document::setStartElement( const Element &e )
{
  mStartElement = e;
  mStartElement.setIsRootElement( true );
}

Element Document::startElement() const
{
  return mStartElement;
}


void Document::addElement( const Element &e )
{
  mElements.append( e );
}

Element::List Document::elements() const
{
  return mElements;
}

bool Document::hasElement( const Element &element )
{
  foreach( Element e, mElements ) {
    if ( e.identifier() == element.identifier() ) return true;
  }
  return false;
}

Element Document::element( const QString &identifier ) const
{
  foreach( Element e, mElements ) {
    if ( e.identifier() == identifier ) return e;
  }
  return Element();
}

Element Document::element( const Relation &relation ) const
{
  return element( relation.target() );
}

Element::List Document::usedElements() const
{
  mUsedElements.clear();
  findUsedElements( mStartElement );
  return mUsedElements;
}

void Document::findUsedElements( const Element &e ) const
{
  addUsedElement( e );

  foreach( Relation r, e.elementRelations() ) {
    Element e2 = element( r );
    if ( (!e2.mixed() || r.isList() ) && addUsedElement( e2 )  ) {
      findUsedElements( e2 );
    }
  }
}

bool Document::addUsedElement( const Element &element ) const
{
  bool found = false;
  foreach( Element usedElement, mUsedElements ) {
    if ( usedElement.identifier() == element.identifier() ) {
      found = true;
      break;
    }
  }
  if ( !found ) {
    mUsedElements.prepend( element );
    return true;
  } else {
    return false;
  }
}
QString Document::targetNamespace() const
{
    return mTargetNamespace;
}

void Document::setTargetNamespace(const QString &targetNamespace)
{
    mTargetNamespace = targetNamespace;
}

void Document::addAttribute( const Attribute &a )
{
    mAttributes.append( a );
}

Attribute::List Document::attributes() const
{
    return mAttributes;
}

bool Document::hasAttribute( const Attribute &attribute )
{
  foreach( Attribute a, mAttributes ) {
    if ( a.identifier() == attribute.identifier() ) return true;
  }
  return false;
}

Attribute Document::attribute( const QString &identifier ) const
{
  foreach( Attribute a, mAttributes ) {
    if ( a.identifier() == identifier ) return a;
  }
  return Attribute();
}

Attribute Document::attribute( const Relation &relation ) const
{
  return attribute( relation.target() );
}

bool Document::isEmpty() const
{
  return mElements.isEmpty();
}

void Document::dump() const
{
  foreach( Element e, mElements ) {
    qDebug() << "ELEMENT" << e.identifier() << ":" << e.name() << e.type();
    if ( e.text() ) qDebug() << " TEXT";
    if ( e.mixed() ) qDebug() << " MIXED";
    foreach( Relation r, e.elementRelations() ) {
      qDebug() << r.asString( "ELEMENT" );
    }
    foreach( Relation r, e.attributeRelations() ) {
      qDebug() << r.asString( "ATTRIBUTE" );
    }
  }
  foreach( Attribute a, mAttributes ) {
    qDebug() << "ATTRIBUTE " << a.identifier() << ": " << a.name() << a.type();
  }
}

Relation::Relation()
  : mMinOccurs( 1 ), mMaxOccurs( 1 )
{
}

Relation::Relation( const QString &target )
  : mTarget( target ), mMinOccurs( 1 ), mMaxOccurs( 1 )
{
}

void Relation::setMinOccurs( int v )
{
  mMinOccurs = v;
}

void Relation::setMaxOccurs( int v )
{
  mMaxOccurs = v;
}

int Relation::minOccurs() const
{
  return mMinOccurs;
}

int Relation::maxOccurs() const
{
  return mMaxOccurs;
}

bool Relation::isOptional() const
{
  return mMinOccurs == 0 && mMaxOccurs == 1;
}

bool Relation::isRequired() const
{
  return mMinOccurs == 1 && mMaxOccurs == 1;
}

bool Relation::isList() const
{
  return mMaxOccurs > 1 || mMaxOccurs == Unbounded;
}

void Relation::setTarget( const QString &identifier )
{
  mTarget = identifier;
}

QString Relation::target() const
{
  return mTarget;
}

void Relation::setChoice( const QString &choice )
{
  mChoice = choice;
}

QString Relation::choice() const
{
  return mChoice;
}

QString Relation::asString( const QString &type ) const
{
  QString out = "  R " + type + ": " + target() + " (" +
    QString::number( minOccurs() ) + ',';
  if ( maxOccurs() == Relation::Unbounded ) {
    out += "UNBOUNDED";
  } else {
    out += QString::number( maxOccurs() );
  }
  out += ')';

  return out;
}


Node::Node()
  : mType( String ), mBaseType( None )
{
}

Node::~Node()
{
}

void Node::setType( Type t )
{
  mType = t;
}

Node::Type Node::type() const
{
  return mType;
}

void Node::setIdentifier( const QString &i )
{
  mIdentifier = i;
}

QString Node::identifier() const
{
  return mIdentifier;
}

void Node::setName( const QString &n )
{
  mName = n;
}

QString Node::name() const
{
  return mName;
}

void Node::setBaseType( Type type )
{
  mBaseType = type;
}

Node::Type Node::baseType() const
{
  return mBaseType;
}

bool Node::isValid() const
{
  return !mIdentifier.isEmpty() && !mName.isEmpty();
}

void Node::setEnumerationValues( const QStringList &v )
{
  mEnumerationValues = v;
}

QStringList Node::enumerationValues() const
{
  return mEnumerationValues;
}

Node::Type Node::typeFromString(QString xsType)
{
  if ( xsType == "xs:string" ) {
    return Schema::Node::String;
  } else if ( xsType == "xs:decimal" || xsType == "xs:double" ) {
    return Schema::Node::Decimal;
  } else if ( xsType == "xs:int")  {
    return Schema::Node::Int;
  } else if ( xsType == "xs:integer")  {
    return Schema::Node::Integer;
  } else if ( xsType == "xs:date" ) {
    return Schema::Node::Date;
  } else if ( xsType == "xs:datetime" ) {
    return Schema::Node::DateTime;
  } else if ( xsType == "xs:normalizedString" ) {
    return Schema::Node::NormalizedString;
  } else if ( xsType == "xs:token" ) {
    return Schema::Node::Token;
  } else if ( xsType == "xs:boolean" ) {
    return Schema::Node::Boolean;
  }
  return Schema::Node::None;
}


void Annotatable::setDocumentation( const QString &str )
{
  mDocumentation = str;
}

QString Annotatable::documentation() const
{
  return mDocumentation;
}

void Annotatable::setAnnotations( const QList<QDomElement> &a )
{
  mAnnotations = a;
}

QList<QDomElement> Annotatable::annotations() const
{
  return mAnnotations;
}


Element::Element()
  : mText( false ), mIsRootElement( false )
{
}

QString Element::ref() const
{
  return name();
}

bool Element::hasRelation( const QString &identifier ) const
{
  return hasElementRelation( identifier ) || hasAttributeRelation( identifier );
}

void Element::addElementRelation( const Relation &r )
{
  mElementRelations.append( r );
}

bool Element::hasElementRelation( const Element &element ) const
{
  Relation::List::ConstIterator it;
  for( it = mElementRelations.begin(); it != mElementRelations.end(); ++it ) {
    if ( (*it).target() == element.identifier() ) return true;
  }
  return false;
}

bool Element::hasElementRelation( const QString &identifier ) const
{
  Relation::List::ConstIterator it;
  for( it = mElementRelations.begin(); it != mElementRelations.end(); ++it ) {
    if ( (*it).target() == identifier ) return true;
  }
  return false;
}

Relation &Element::elementRelation( const Element &element )
{
  Relation::List::Iterator it;
  for( it = mElementRelations.begin(); it != mElementRelations.end(); ++it ) {
    if ( (*it).target() == element.identifier() ) return *it;
  }
  return mNullRelation;
}

Relation::List Element::elementRelations() const
{
  return mElementRelations;
}

bool Element::hasElementRelations() const
{
  return !mElementRelations.isEmpty();
}

void Element::addAttributeRelation( const Relation &r )
{
  mAttributeRelations.append( r );
}

bool Element::hasAttributeRelation( const Attribute &attribute ) const
{
  Relation::List::ConstIterator it;
  for( it = mAttributeRelations.begin(); it != mAttributeRelations.end(); ++it ) {
    if ( (*it).target() == attribute.identifier() ) return true;
  }
  return false;
}

bool Element::hasAttributeRelation( const QString &identifier ) const
{
  Relation::List::ConstIterator it;
  for( it = mAttributeRelations.begin(); it != mAttributeRelations.end(); ++it ) {
    if ( (*it).target() == identifier ) return true;
  }
  return false;
}

Relation::List Element::attributeRelations() const
{
  return mAttributeRelations;
}

bool Element::hasAttributeRelations() const
{
  return !mAttributeRelations.isEmpty();
}

bool Element::mixed() const
{
  return mText && !mElementRelations.isEmpty();
}

void Element::setText( bool text )
{
  mText = text;
}

bool Element::text() const
{
  return mText;
}

bool Element::isEmpty() const
{
  return !mText && mElementRelations.isEmpty();
}
bool Element::isRootElement() const
{
    return mIsRootElement;
}

void Element::setIsRootElement(bool IsRootElement)
{
    mIsRootElement = IsRootElement;
}


Attribute::Attribute()
{
}

bool Attribute::required() const
{
    return mRequired;
}

void Attribute::setRequired(bool required)
{
    mRequired = required;
}

QString Attribute::defaultValue() const
{
  return mDefVal;
}

void Attribute::setDefaultValue(const QString defVal)
{
  mDefVal = defVal;
}

QString Attribute::ref() const
{
  return '@' + name();
}
