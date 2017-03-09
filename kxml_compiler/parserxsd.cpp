/*
    This file is part of KDE.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "parserxsd.h"

#include <schema/parser.h>

#include <common/nsmanager.h>
#include <common/messagehandler.h>
#include <common/parsercontext.h>

#include <kdebug.h>

#include <QDebug>

using namespace RNG;

ParserXsd::ParserXsd() : mVerbose ( false )
{
}

void ParserXsd::setVerbose( bool verbose )
{
  mVerbose = verbose;
}

Schema::Document ParserXsd::parse( const QString &data )
{
  if ( mVerbose ) {
    qDebug() << "----ParserXsd::parse() string";
  }

  NSManager namespaceManager;
  MessageHandler messageHandler;
  ParserContext context;
  context.setNamespaceManager( &namespaceManager );
  context.setMessageHandler( &messageHandler );

  XSD::Parser parser;
  if ( !parser.parseString( &context, data ) ) {
    qDebug() << "Error parsing data.";
    return Schema::Document();
  } else {
    return parse( parser );
  }
}

Schema::Document ParserXsd::parse( QFile &file )
{
  if ( mVerbose ) {
    qDebug() << "----ParserXsd::parse() file";
  }

  NSManager namespaceManager;
  MessageHandler messageHandler;
  ParserContext context;
  context.setNamespaceManager( &namespaceManager );
  context.setMessageHandler( &messageHandler );

  XSD::Parser parser;
  if ( !parser.parseFile( &context, file ) ) {
    qDebug() << "Error parsing file " << file.fileName();

    return Schema::Document();
  } else {
    return parse( parser );
  }
}

Schema::Document ParserXsd::parse( const XSD::Parser &parser )
{
  XSD::Types types = parser.types();

  foreach ( XSD::Element element, types.elements() ) {
    if ( mVerbose ) {
      qDebug() << "Element: " << element.name() << element.type().qname();
//      qDebug() << "  Annotations: " << element.annotations().count();
//      qDebug() << "  " << element.minOccurs() << ","
//        << element.maxOccurs();
    }

    Schema::Element e;
    e.setIdentifier( element.name() );
    e.setName( element.name() );
    XSD::ComplexType complexType = types.complexType( element );
    if ( complexType.contentModel() == XSD::XSDType::MIXED ) {
      if ( mVerbose ) {
        qDebug() << "  Mixed";
      }
      e.setText( true );
    } else if ( complexType.contentModel() == XSD::XSDType::SIMPLE &&
            !complexType.baseTypeName().isEmpty() ) {
      e.setBaseType( Schema::Node::typeFromString(complexType.baseTypeName().qname()) );
    }

    e.setType( Schema::Node::typeFromString(element.type().qname()) );
    if ( e.type() == Schema::Node::None ) {
      bool found = false;
      XSD::SimpleType simpleType = types.simpleType( element.type().qname(), &found );

      // check if element having restrictions
      if ( !found )
        e.setType( Schema::Node::ComplexType );
      else {
        if ( simpleType.facetType() &  XSD::SimpleType::ENUM ) {
          setType( e, simpleType );
        }
      }
    }
    foreach( XSD::Element subElement, complexType.elements() ) {
      if ( mVerbose ) {
        qDebug() << "  Element: " << subElement.name();
        qDebug() << "    " << subElement.minOccurs() << ","
          << subElement.maxOccurs();
      }

      Schema::Relation eRelation( subElement.name() );
      eRelation.setMinOccurs( subElement.minOccurs() );
      if ( subElement.maxOccurs() == XSD::Parser::UNBOUNDED ) {
        eRelation.setMaxOccurs( Schema::Relation::Unbounded );
      } else {
        eRelation.setMaxOccurs( subElement.maxOccurs() );
      }
      XSD::Compositor compositor = subElement.compositor();
      if ( mVerbose ) {
        qDebug() << "  Compositor " << compositor.type();
      }
      if ( compositor.type() == XSD::Compositor::Choice ) {
        QString choice;
        foreach( QName qname, compositor.children() ) {
          if ( !choice.isEmpty() ) choice += '+';
          choice += qname.qname();
        }
        eRelation.setChoice( choice );
      }
      e.addElementRelation( eRelation );
    }

    foreach( XSD::Attribute attribute, complexType.attributes() ) {
      if ( mVerbose ) {
        qDebug() << "  Attribute: " << attribute.name();
      }

      Schema::Relation aRelation( attribute.name() );
      e.addAttributeRelation( aRelation );

      Schema::Attribute a;
      a.setIdentifier( attribute.name() );
      a.setName( attribute.name() );

      if ( !attribute.type().isEmpty() ) {
        a.setType( Schema::Node::typeFromString(attribute.type().qname()) );
        if ( a.type() == Schema::Node::None ) {
          XSD::SimpleType simpleType = types.simpleType( attribute.type() );
          setType( a, simpleType );
        }
      }

      a.setRequired(attribute.isUsed());
      a.setDefaultValue(attribute.defaultValue());

      mDocument.addAttribute( a );
    }

    setAnnotations( e, element.annotations() );

    mDocument.addElement( e );
    if ( mDocument.startElement().isEmpty() ) {
      mDocument.setStartElement( e );
    }
  }

  setAnnotations( mDocument, parser.annotations() );

  if ( mVerbose ) {
    qDebug() << "----ParserXsd::parse() done";
  }

  mDocument.setTargetNamespace(parser.targetNamespace());
  return mDocument;
}

void ParserXsd::setAnnotations( Schema::Annotatable &annotatable,
  XSD::Annotation::List annotations )
{
  QString documentation;
  QList<QDomElement> domElements;
  foreach ( XSD::Annotation a, annotations ) {
    if ( a.isDocumentation() ) documentation.append( a.documentation() );
    if ( a.isAppinfo() ) {
      domElements.append( a.domElement() );
    }
  }
  annotatable.setDocumentation( documentation );
  annotatable.setAnnotations( domElements );
}

void ParserXsd::setType( Schema::Node &node, const XSD::SimpleType &simpleType )
{
  if ( simpleType.subType() == XSD::SimpleType::TypeRestriction ) {
    if ( simpleType.facetType() == XSD::SimpleType::NONE ) {
      // Nothing to do.
    } else if ( simpleType.facetType() == XSD::SimpleType::ENUM ) {
      node.setType( Schema::Node::Enumeration );
      node.setEnumerationValues( simpleType.facetEnums() );
    } else {
      qDebug() << "SimpleType::facetType(): " << simpleType.facetType()
        << " not supported.";
    }
  } else {
    qDebug() << "SimpleType::subType: " << simpleType.subType()
      << " not supported.";
  }
}
