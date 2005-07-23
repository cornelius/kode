/*
    This file is part of Kung.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>

#include <schema/complextype.h>

#include "inputfieldfactory.h"

#include "complexbaseinputfield.h"

ComplexBaseInputField::ComplexBaseInputField( const QString &name, const Schema::ComplexType *type )
  : ComplexInputField( name, type )
{
  Schema::Element::List elements = type->elements();
  Schema::Element::List::ConstIterator elemIt;
  for ( elemIt = elements.begin(); elemIt != elements.end(); ++elemIt ) {
    bool isList = ((*elemIt).maxOccurs() == UNBOUNDED);
    InputField *field = InputFieldFactory::self()->createField( (*elemIt).name(), (*elemIt).typeName(), isList );
    if ( !field ) {
      qDebug( "ComplexBaseInputField: Unable to create field of type %s", type->baseTypeName().latin1() );
    } else {
      appendChild( field );
    }
  }

  Schema::Attribute::List attributes = type->attributes();
  Schema::Attribute::List::ConstIterator attrIt;
  for ( attrIt = attributes.begin(); attrIt != attributes.end(); ++attrIt ) {
    InputField *field = InputFieldFactory::self()->createField( (*attrIt).name(), (*attrIt).typeName() );
    if ( !field ) {
      qDebug( "ComplexBaseInputField: Unable to create field of type %s", type->baseTypeName().latin1() );
    } else {
      appendChild( field );
    }
  }
}

void ComplexBaseInputField::setXMLData( const QDomElement &element )
{
  if ( mName != element.tagName() ) {
    qDebug( "ComplexBaseInputField: Wrong dom element passed: expected %s, got %s", mName.latin1(), element.tagName().latin1() );
    return;
  }

  // elements
  if ( mType->isArray() ) {
    InputField *field = childField( "item" );
    field->setXMLData( element );
  } else {
    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
      QDomElement child = node.toElement();
      if ( !child.isNull() ) {
        InputField *field = childField( child.tagName() );
        if ( !field ) {
          qDebug( "ComplexBaseInputField: Child field %s does not exists", child.tagName().latin1() );
        } else {
          field->setXMLData( child );
        }
      }

      node = node.nextSibling();
    }
  }

  // attributes
  QDomNamedNodeMap nodes = element.attributes();
  for ( uint i = 0; i < nodes.count(); ++i ) {
    QDomNode node = nodes.item( i );
    QDomAttr attr = node.toAttr();

    InputField *field = childField( attr.name() );
    if ( !field ) {
      qDebug( "ComplexBaseInputField: Child field %s does not exists", attr.name().latin1() );
    } else {
      field->setData( attr.value() );
    }
  }
}

void ComplexBaseInputField::xmlData( QDomDocument &document, QDomElement &parent )
{
  QDomElement element = document.createElement( mName );

  Schema::Element::List elements = mType->elements();
  Schema::Element::List::ConstIterator elemIt;
  for ( elemIt = elements.begin(); elemIt != elements.end(); ++elemIt ) {
    InputField *field = childField( (*elemIt).name() );
    if ( !field ) {
      qDebug( "ComplexBaseInputField: No child found" );
    } else {
      field->xmlData( document, element );
    }
  }

  Schema::Attribute::List attributes = mType->attributes();
  Schema::Attribute::List::ConstIterator attrIt;
  for ( attrIt = attributes.begin(); attrIt != attributes.end(); ++attrIt ) {
    InputField *field = childField( (*attrIt).name() );
    if ( !field ) {
      qDebug( "ComplexBaseInputField: No child found" );
    } else {
      element.setAttribute( field->name(), field->data() );
    }
  }

  parent.appendChild( element );
}

void ComplexBaseInputField::setData( const QString& )
{
}

QString ComplexBaseInputField::data() const
{
  return QString();
}

QWidget *ComplexBaseInputField::createWidget( QWidget *parent )
{
  QGroupBox *inputWidget = new QGroupBox( mName, parent );
  inputWidget->setColumnLayout( 0, Qt::Horizontal );
  QGridLayout *layout = new QGridLayout( inputWidget->layout(), 2, 2, 6 );

  InputField::List::Iterator it;
  int row = 0;
  for ( it = mFields.begin(); it != mFields.end(); ++it, ++row ) {
    QLabel *label = new QLabel( (*it)->name(), inputWidget );
    layout->addWidget( label, row, 0 );
    layout->addWidget( (*it)->createWidget( inputWidget ), row, 1 );
  }

  return inputWidget;
}
