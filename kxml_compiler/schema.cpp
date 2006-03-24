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
    if ( !e2.mixed() && addUsedElement( e2 ) ) {
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

void Document::addAttribute( const Attribute &a )
{
  mAttributes.append( a );
}

Attribute::List Document::attributes() const
{
  return mAttributes;
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
    qDebug() << "ELEMENT " << e.identifier() << ": " << e.name();
    if ( e.text() ) qDebug() << "  TEXT";
    if ( e.mixed() ) qDebug() << "  MIXED";
    foreach( Relation r, e.elementRelations() ) {
      qDebug() << r.asString( "ELEMENT" );
    }
    foreach( Relation r, e.attributeRelations() ) {
      qDebug() << r.asString( "ATTRIBUTE" );
    }
  }
  foreach( Attribute a, mAttributes ) {
    qDebug() << "ATTRIBUTE " << a.identifier() << ": " << a.name();
  }
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

QString Relation::asString( const QString &type ) const
{
  QString out = "  R " + type + ": " + target() + " (" +
    QString::number( minOccurs() ) + ",";
  if ( maxOccurs() == Relation::Unbounded ) {
    out += "UNBOUNDED";
  } else {
    out += QString::number( maxOccurs() );
  }
  out += ")";

  return out;
}


Node::Node()
{
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

bool Node::isValid() const
{
  return !mIdentifier.isEmpty() && !mName.isEmpty();
}


Element::Element()
  : mText( false )
{
}

void Element::addElementRelation( const Relation &r )
{
  mElementRelations.append( r );
}

Relation::List Element::elementRelations() const
{
  return mElementRelations;
}

void Element::addAttributeRelation( const Relation &r )
{
  mAttributeRelations.append( r );
}

Relation::List Element::attributeRelations() const
{
  return mAttributeRelations;
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

Attribute::Attribute()
{
}
