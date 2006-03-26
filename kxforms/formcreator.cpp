/*
    This file is part of KXForms.

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

#include "formcreator.h"

#include "xmlbuilder.h"

#include <QDebug>

using namespace KXForms;

FormCreator::FormCreator()
{
}

QString FormCreator::create( const Schema::Document &schemaDocument )
{
  mDocument = schemaDocument;

  XmlBuilder xml( "kxforms" );

  createForm( &xml, schemaDocument.startElement() );

  foreach ( Schema::Element element, schemaDocument.usedElements() ) {
    if ( element.identifier() != schemaDocument.startElement().identifier() ) {
      createForm( &xml, element );
    }
  }

  return xml.print();
}

void FormCreator::createForm( XmlBuilder *xml, const Schema::Element &element )
{
  qDebug() << "ELEMENT" << element.name(); 
  XmlBuilder *form = xml->tag( "form" )->attribute( "ref", element.name() );

  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );
    if ( a.type() == Schema::Attribute::String ) {
      form->tag( "xf:input" )->attribute( "ref", "@" + a.name() )
        ->tag( "xf:label", humanizeString( r.target() ) );
    } else if ( a.type() == Schema::Attribute::Enumeration ) {
      XmlBuilder *select1 = form->tag( "xf:select1" );
      select1->attribute( "ref", "@" + a.name() );
      select1->tag( "xf:label", humanizeString( a.name() ) );
      foreach( QString value, a.enumerationValues() ) {
        XmlBuilder *item = select1->tag( "xf:item" );
        item->tag( "xf:label", humanizeString( value ) );
        item->tag( "xf:value", value );  
      }
    } else {
      qDebug() << "Unsupported type: " << a.type();
    }
  }

  QString currentChoice;

  XmlBuilder *list = 0;

  foreach( Schema::Relation r, element.elementRelations() ) {
    qDebug() << "CHILD" << r.target();
    qDebug() << "  CHOICE: " << r.choice();
    if ( r.isList() ) {
      bool isMixedList = r.choice().contains( "+" );
      
      if ( !list || r.choice().isEmpty() || currentChoice != r.choice() ) {
        list = form->tag( "list" );
        QString label;
        if ( isMixedList ) {
          label = "Item";
        } else {
          label = humanizeString( r.target(), true );
        }
        list->tag( "xf:label", label );
      }
      XmlBuilder *item = list->tag( "itemclass" );
      item->attribute( "ref", r.target() );

      // Try to guess a suitable item label.
      QString itemLabel;
      Schema::Element itemElement = mDocument.element( r );
      foreach( Schema::Relation r2, itemElement.attributeRelations() ) {
        if ( r2.target() == "name" ) {
          if ( isMixedList ) {
            itemLabel = humanizeString( itemElement.name() ) + ": ";
          }
          itemLabel += "<arg ref=\"@name\"/>";
          break;
        }
      }

      if ( itemLabel.isEmpty() ) itemLabel = humanizeString( r.target() );
      item->tag( "itemlabel", itemLabel );

      currentChoice = r.choice();
    } else {
      form->tag( "xf:textarea" )->attribute( "ref", r.target() )
        ->tag( "xf:label", humanizeString( r.target() ) );
    }
  }
}

QString FormCreator::humanizeString( const QString &str, bool pluralize )
{
  if ( str.isEmpty() ) return str;
  QString result = str[0].toUpper() + str.mid( 1 );
  if ( pluralize ) {
    if ( result.endsWith( "y" ) ) {
      result = result.left( str.length() - 1 ) + "ies";
    } else {
      result += "s";
    }
  }

  return result;
}
