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
    form->tag( "xf:input" )->attribute( "ref", "@" + r.target() )
      ->tag( "xf:label", r.target() );
  }

  QString currentChoice;

  XmlBuilder *list = 0;

  foreach( Schema::Relation r, element.elementRelations() ) {
    qDebug() << "CHILD" << r.target();
    qDebug() << "  CHOICE: " << r.choice();
    if ( r.isList() ) {
      if ( !list || r.choice().isEmpty() || currentChoice != r.choice() ) {
        list = form->tag( "list" );
        list->tag( "xf:label", r.target() );
      }
      list->tag( "itemclass" )->attribute( "ref", r.target() )
        ->tag( "itemlabel", r.target() );
      currentChoice = r.choice();
    } else {
      form->tag( "xf:input" )->attribute( "ref", "@" + r.target() )
        ->tag( "xf:label", r.target() );
    }
  }
}
