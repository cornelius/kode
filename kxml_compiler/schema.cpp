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

Element Document::element( const QString &identifier )
{
  foreach( Element e, mElements ) {
    if ( e.identifier() == identifier ) return e;
  }
  return Element();
}


void Document::addAttribute( const Attribute &a )
{
  mAttributes.append( a );
}

Attribute::List Document::attributes() const
{
  return mAttributes;
}

Attribute Document::attribute( const QString &identifier )
{
  foreach( Attribute a, mAttributes ) {
    if ( a.identifier() == identifier ) return a;
  }
  return Attribute();
}

void Document::dump() const
{
  foreach( Element e, mElements ) {
    qDebug() << "ELEMENT " << e.identifier() << ": " << e.name();
    if ( e.mixed() ) qDebug() << "  MIXED";
    foreach( Relation r, e.elementRelations() ) {
      dumpRelation( r, "ELEMENT" );
    }
    foreach( Relation r, e.attributeRelations() ) {
      dumpRelation( r, "ATTRIBUTE" );
    }
  }
  foreach( Attribute a, mAttributes ) {
    qDebug() << "ATTRIBUTE " << a.identifier() << ": " << a.name();
  }
}

void Document::dumpRelation( const Relation &r, const QString &type ) const
{
  if ( r.maxOccurs() == Relation::Unbounded ) {
    qDebug() << "  R" << type << ":" << r.target() << " (" << r.minOccurs() <<
      ", UNBOUNDED )";
  } else {
    qDebug() << "  R" << type << ":" << r.target() << " (" << r.minOccurs() <<
      "," << r.maxOccurs() << ")";
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
  return mMaxOccurs > 1;
}

void Relation::setTarget( const QString &identifier )
{
  mTarget = identifier;
}

QString Relation::target() const
{
  return mTarget;
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
  : mMixed( false )
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

void Element::setMixed( bool mixed )
{
  mMixed = mixed;
}

bool Element::mixed() const
{
  return mMixed;
}


Attribute::Attribute()
{
}
