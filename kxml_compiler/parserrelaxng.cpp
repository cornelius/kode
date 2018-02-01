/*
    This file is part of KDE.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>

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

#include "parserrelaxng.h"

#include <QDebug>

#include <iostream>

using namespace RNG;

Pattern::Pattern()
  : optional( false ), zeroOrMore( false ), oneOrMore( false ),
    choice( false )
{
}

bool Pattern::isEmpty()
{
  return !optional && !zeroOrMore && !oneOrMore && !choice;
}

QString Pattern::asString()
{
  if ( isEmpty() ) return "";
  QString str = "( ";
  if ( optional ) str += "optional ";
  if ( zeroOrMore ) str += "zeroOrMore ";
  if ( oneOrMore ) str += "oneOrMore ";
  if ( choice ) str += "choice ";
  str += ')';
  return str;
}

void Pattern::merge( Pattern p )
{
  if ( p.optional ) optional = true;
  if ( p.zeroOrMore ) zeroOrMore = true;
  if ( p.oneOrMore ) oneOrMore = true;
  if ( p.choice ) choice = true;
}

Element::Element()
  : hasText( false ), isEmpty( false )
{
}

ParserRelaxng::ParserRelaxng()
{
}

void ParserRelaxng::setVerbose( bool verbose )
{
  mVerbose = verbose; 
}

Element *ParserRelaxng::parse( const QDomElement &docElement )
{
  Element *start = 0;

  QDomNode n1;
  QDomElement e1;
  for( e1 = docElement.firstChildElement(); !e1.isNull(); e1 = e1.nextSiblingElement() ) {
    if ( mVerbose ) {
      qDebug() <<"TOP LEVEL element" << e1.tagName();
    }
    if ( e1.tagName() == "define" ) {
      Element *d = new Element;
      d->name = e1.attribute( "name" );
      parseElement( e1, d, Pattern() );
      Element::List definitions;
      QMap<QString,Element::List >::ConstIterator it;
      it = mDefinitionMap.constFind( d->name );
      if ( it != mDefinitionMap.constEnd() ) definitions = *it;
      definitions.append( d );
      mDefinitionMap.insert( d->name, definitions );
    } else if ( e1.tagName() == "start" ) {
      start = new Element;
      parseElement( e1, start, Pattern() );
    } else if ( e1.isComment()  ) {
      // Ignore all comments for now!
    } else {
      qDebug() <<"parseGrammar: Unrecognized tag:" << e1.tagName();
    }
  }

  return start;
}

Reference *ParserRelaxng::parseReference( const QDomElement &referenceElement )
{
  Reference *r = new Reference;
  r->name = referenceElement.attribute( "name" );
  return r;
}

bool ParserRelaxng::parseAttribute( const QDomElement &attributeElement,
                           Attribute *a )
{
  a->name = attributeElement.attribute( "name" );

  return true;
}

bool ParserRelaxng::parseElement( const QDomElement &elementElement, Element *e,
                   Pattern pattern )
{
  if ( mVerbose ) {
    qDebug() <<"parseElement" << e->name;
  }

  QDomNode n1;
  QDomElement e1;
  for( e1 = elementElement.firstChildElement(); !e1.isNull(); e1 = e1.nextSiblingElement() ) {
    if ( e1.tagName() == "element" ) {
      Element *element = new Element;
      element->name = e1.attribute( "name" );
      element->pattern = pattern;
      parseElement( e1, element, Pattern() );
      e->elements.append( element );
    } else if ( e1.tagName() == "attribute" ) {
      Attribute *a = new Attribute;
      a->name = e1.attribute( "name" );
      a->pattern = pattern;
      if ( mVerbose ) {
        qDebug() <<"ATTRIBUTE:" << a->name << a->pattern.asString();
      }
      parseAttribute( e1, a );
      e->attributes.append( a );
    } else if ( e1.tagName() == "ref" ) {
      Reference *r = parseReference( e1 );
      r->pattern = pattern;
      e->references.append( r );
    } else if ( e1.tagName() == "text" ) {
      e->hasText = true;
    } else if ( e1.tagName() == "empty" ) {
      e->isEmpty = true;
    } else {
      Pattern p = pattern;
      if ( e1.tagName() == "optional" ) p.optional = true;
      else if ( e1.tagName() == "zeroOrMore" ) p.zeroOrMore = true;
      else if ( e1.tagName() == "oneOrMore" ) p.oneOrMore = true;
      else if ( e1.tagName() == "choice" ) p.choice = true;
      else {
        qDebug() <<"Unsupported pattern '" << e1.tagName() <<"'";
      }
      parseElement( e1, e, p );
    }
  }

  return true;
}

void ParserRelaxng::substituteReferences( Element *s )
{
  if ( mVerbose ) {
    qDebug() <<"substituteReferences for '" << s->name <<"'";
  }
  Reference::List::Iterator it = s->references.begin();
  while( it != s->references.end() ) {
    Reference *r = *it;
    if ( mVerbose ) {
      qDebug() <<"REF" << r->name;
    }
    if ( r->name == s->name ) {
      if ( mVerbose ) {
        qDebug() <<"Don't resolve self reference";
      }
      return;
    }
    if ( r->substituted ) {
      if ( mVerbose ) {
        qDebug() <<"Already substituted.";
      }
      ++it;
      continue;
    } else {
      r->substituted = true;
    }
    QMap<QString,Element::List >::ConstIterator it1;
    it1 = mDefinitionMap.constFind( r->name );
    if ( it1 != mDefinitionMap.constEnd() ) {
      Element::List elements = *it1;
      Element::List::ConstIterator it4;
      for( it4 = elements.constBegin(); it4 != elements.constEnd(); ++it4 ) {
        Element *d = *it4;
        substituteReferences( d );
        Element::List::ConstIterator it2;
        for( it2 = d->elements.constBegin(); it2 != d->elements.constEnd(); ++it2 ) {
          Element *e = *it2;
          e->pattern.merge( r->pattern );
          substituteReferences( e );
          s->elements.append( e );
        }
        Attribute::List::ConstIterator it3;
        for( it3 = d->attributes.constBegin(); it3 != d->attributes.constEnd();
             ++it3 ) {
          Attribute *a = *it3;
          a->pattern.merge( r->pattern );
          s->attributes.append( a );
        }
      }
      it = s->references.erase( it );
    } else {
      qDebug() <<"Reference not found";
      ++it;
    }
  }
}

void ParserRelaxng::doIndent( int cols )
{
  for( int i = 0; i < cols; ++i ) std::cout << " ";
}

void ParserRelaxng::dumpPattern( Pattern pattern )
{
  std::cout << pattern.asString().toLocal8Bit().data();
}

void ParserRelaxng::dumpReferences( const Reference::List &references, int indent )
{
  Reference::List::ConstIterator it;
  for( it = references.constBegin(); it != references.constEnd(); ++it ) {
    Reference *r = *it;
    doIndent( indent );
    std::cout << "REFERENCE " << r->name.toLocal8Bit().data();
    dumpPattern( r->pattern );
    std::cout << std::endl;
  }
}

void ParserRelaxng::dumpAttributes( const Attribute::List &attributes, int indent )
{
  Attribute::List::ConstIterator it;
  for( it = attributes.constBegin(); it != attributes.constEnd(); ++it ) {
    Attribute *a = *it;
    doIndent( indent );
    std::cout << "ATTRIBUTE " << a->name.toLocal8Bit().data();
    dumpPattern( a->pattern );
    std::cout << std::endl;
  }
}

void ParserRelaxng::dumpElements( const Element::List &elements, int indent )
{
  Element::List::ConstIterator it;
  for( it = elements.constBegin(); it != elements.constEnd(); ++it ) {
    Element *e = *it;
    dumpElement( e, indent );
  }
}

void ParserRelaxng::dumpElement( Element *e, int indent )
{
  doIndent( indent );
  std::cout << "ELEMENT " << e->name.toLocal8Bit().data();
  dumpPattern( e->pattern );
  std::cout << std::endl;

  if ( e->hasText ) {
    doIndent( indent + 2 );
    std::cout << "TEXT" << std::endl;
  }

  dumpAttributes( e->attributes, indent + 2 );
  dumpElements( e->elements, indent + 2 );
  dumpReferences( e->references, indent + 2 );
}

void ParserRelaxng::dumpTree( Element *s )
{
  std::cout << "START " << s->name.toLocal8Bit().data() << std::endl;
  dumpElements( s->elements, 2 );
  dumpReferences( s->references, 2 );
}

void ParserRelaxng::dumpDefinitionMap()
{
  std::cout << "DEFINITION MAP" << std::endl;
  QMap<QString,Element::List >::ConstIterator it;
  for( it = mDefinitionMap.constBegin(); it != mDefinitionMap.constEnd(); ++it ) {
    dumpElements( *it, 2 );
  }
}

Schema::Document ParserRelaxng::convertToSchema( Element *start )
{
  Element::List elements = start->elements;
  if ( !elements.isEmpty() ) {
    Schema::Element element = convertToSchemaElement( elements.first() );
    mDocument.setStartElement( element );
  }

  return mDocument;
}

Schema::Element ParserRelaxng::convertToSchemaElement( Element *e )
{
  Schema::Element schemaElement;
  schemaElement.setName( e->name );
  schemaElement.setIdentifier( e->name );

  if ( e->hasText ) schemaElement.setText( true );

  foreach( Element *element, e->elements ) {
    QString id = element->name;
    if ( !mDocument.element( id ).isValid() ) {
      Schema::Element relatedElement = convertToSchemaElement( element );
    }
    Schema::Relation relation = convertToRelation( element->pattern, id );
    schemaElement.addElementRelation( relation );
  }

  foreach( Attribute *attribute, e->attributes ) {
    QString id = schemaElement.identifier() + '/' + attribute->name;
    if ( !mDocument.attribute( id ).isValid() ) {
      Schema::Attribute relatedAttribute = convertToSchemaAttribute(
        schemaElement.identifier(), attribute );
    }
    Schema::Relation relation = convertToRelation( attribute->pattern, id );
    schemaElement.addAttributeRelation( relation );
  }

  foreach( Reference *reference, e->references ) {
    QString id = reference->name;
    Schema::Relation relation = convertToRelation( reference->pattern, id );
    schemaElement.addElementRelation( relation );
  }

  mDocument.addElement( schemaElement );

  return schemaElement;
}

Schema::Relation ParserRelaxng::convertToRelation( const Pattern &pattern,
  const QString &id )
{
  Schema::Relation relation( id );
  if ( pattern.optional ) {
    relation.setMinOccurs( 0 );
    relation.setMaxOccurs( 1 );
  } else if ( pattern.zeroOrMore ) {
    relation.setMinOccurs( 0 );
    relation.setMaxOccurs( Schema::Relation::Unbounded );
  } else if ( pattern.oneOrMore ) {
    relation.setMinOccurs( 1 );
    relation.setMaxOccurs( Schema::Relation::Unbounded );
  }

  return relation;
}

Schema::Attribute ParserRelaxng::convertToSchemaAttribute( const QString &path,
  Attribute *a )
{
  Schema::Attribute schemaAttribute;
  schemaAttribute.setName( a->name );
  schemaAttribute.setIdentifier( path + '/' + a->name );

  mDocument.addAttribute( schemaAttribute );

  return schemaAttribute;
}
