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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "parser.h"

#include <kode/code.h>
#include <kode/printer.h>
#include <kode/typedef.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>

#include <iostream>

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
  str += ")";
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

Parser::Parser()
{
}

Element *Parser::parse( const QDomElement &docElement )
{
  Element *start = 0;

  QDomNode n1;
  for( n1 = docElement.firstChild(); !n1.isNull(); n1 = n1.nextSibling() ) {
    QDomElement e1 = n1.toElement();
    kdDebug() << "TOP LEVEL element " << e1.tagName() << endl;
    if ( e1.tagName() == "define" ) {
      Element *d = new Element;
      d->name = e1.attribute( "name" );
      parseElement( e1, d, Pattern() );
      Element::List definitions;
      QMap<QString,Element::List >::ConstIterator it; 
      it = mDefinitionMap.find( d->name );
      if ( it != mDefinitionMap.end() ) definitions = *it;
      definitions.append( d );
      mDefinitionMap.replace( d->name, definitions );
    } else if ( e1.tagName() == "start" ) {
      start = new Element;
      parseElement( e1, start, Pattern() );
    } else {
      kdDebug() << "parseGrammar: Unrecognized tag: " << e1.tagName() << endl;
    }
  }

  return start;
}

Reference *Parser::parseReference( const QDomElement &referenceElement )
{
  Reference *r = new Reference;
  r->name = referenceElement.attribute( "name" );
  return r;
}

bool Parser::parseAttribute( const QDomElement &attributeElement,
                           Attribute *a )
{
  a->name = attributeElement.attribute( "name" );

  return true;
}

bool Parser::parseElement( const QDomElement &elementElement, Element *e,
                   Pattern pattern )
{
  kdDebug() << "parseElement " << e->name << endl;

  QDomNode n1;
  for( n1 = elementElement.firstChild(); !n1.isNull(); n1 = n1.nextSibling() ) {
    QDomElement e1 = n1.toElement();
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
      kdDebug() << "ATTRIBUTE: " << a->name << " " << a->pattern.asString()
                << endl;
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
        kdDebug() << "Unsupported pattern '" << e1.tagName() << "'" << endl;
      }
      parseElement( e1, e, p );
    }
  }

  return true;
}

void Parser::substituteReferences( Element *s )
{
  kdDebug() << "substituteReferences for '" << s->name << "'" << endl;
  Reference::List::Iterator it = s->references.begin();
  while( it != s->references.end() ) {
    Reference *r = *it;
    kdDebug() << "REF " << r->name << endl;
    if ( r->name == s->name ) {
      kdDebug() << "Don't resolve self reference" << endl;
      return;
    }
    if ( r->substituted ) {
      kdDebug() << "Already substituted." << endl;
      ++it;
      continue;
    } else {
      r->substituted = true;
    }
    QMap<QString,Element::List >::ConstIterator it1;
    it1 = mDefinitionMap.find( r->name );
    if ( it1 != mDefinitionMap.end() ) {
      Element::List elements = *it1;
      Element::List::ConstIterator it4;
      for( it4 = elements.begin(); it4 != elements.end(); ++it4 ) {
        Element *d = *it4;
        substituteReferences( d );
        Element::List::ConstIterator it2;
        for( it2 = d->elements.begin(); it2 != d->elements.end(); ++it2 ) {
          Element *e = *it2;
          e->pattern.merge( r->pattern );
          substituteReferences( e );
          s->elements.append( e );
        }
        Attribute::List::ConstIterator it3;
        for( it3 = d->attributes.begin(); it3 != d->attributes.end();
             ++it3 ) {
          Attribute *a = *it3;
          a->pattern.merge( r->pattern );
          s->attributes.append( a );
        }
      }
      it = s->references.erase( it );
    } else {
      kdDebug() << "Reference not found" << endl;
      ++it;
    }
  }
}

void Parser::doIndent( int cols )
{
  for( int i = 0; i < cols; ++i ) std::cout << " ";
}

void Parser::dumpPattern( Pattern pattern )
{
  std::cout << pattern.asString().utf8();
}

void Parser::dumpReferences( const Reference::List &references, int indent )
{
  Reference::List::ConstIterator it;
  for( it = references.begin(); it != references.end(); ++it ) {
    Reference *r = *it;
    doIndent( indent );
    std::cout << "REFERENCE " << r->name.utf8();
    dumpPattern( r->pattern );
    std::cout << std::endl;
  }
}

void Parser::dumpAttributes( const Attribute::List &attributes, int indent )
{
  Attribute::List::ConstIterator it;
  for( it = attributes.begin(); it != attributes.end(); ++it ) {
    Attribute *a = *it;
    doIndent( indent );
    std::cout << "ATTRIBUTE " << a->name.utf8();
    dumpPattern( a->pattern );
    std::cout << std::endl;
  }
}

void Parser::dumpElements( const Element::List &elements, int indent )
{
  Element::List::ConstIterator it;
  for( it = elements.begin(); it != elements.end(); ++it ) {
    Element *e = *it;
    dumpElement( e, indent );
  }
}

void Parser::dumpElement( Element *e, int indent )
{
  doIndent( indent );
  std::cout << "ELEMENT " << e->name.utf8();
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

void Parser::dumpTree( Element *s )
{
  std::cout << "START " << s->name.utf8() << std::endl;
  dumpElements( s->elements, 2 );
  dumpReferences( s->references, 2 );
}

void Parser::dumpDefinitionMap()
{
  std::cout << "DEFINITION MAP" << std::endl;
  QMap<QString,Element::List >::ConstIterator it;
  for( it = mDefinitionMap.begin(); it != mDefinitionMap.end(); ++it ) {
    dumpElements( *it, 2 );
  }
}
