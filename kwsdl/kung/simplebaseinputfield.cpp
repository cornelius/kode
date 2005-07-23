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

#include <schema/simpletype.h>

#include "inputfieldfactory.h"

#include "simplebaseinputfield.h"

SimpleBaseInputField::SimpleBaseInputField( const QString &name, const Schema::SimpleType *type )
  : SimpleInputField( name, type )
{
  if ( type->subType() == Schema::SimpleType::TypeRestriction ) {
    InputField *field = InputFieldFactory::self()->createBasicField( name, type->baseTypeName(), type );
    if ( !field ) {
      qDebug( "SimpleBaseInputField: Unable to create field of type %s", type->baseTypeName().latin1() );
    } else {
      appendChild( field );
    }
  } else {
    qDebug( "SimpleBaseInputField: Unsupported subtype" );
  }
}

void SimpleBaseInputField::setXMLData( const QDomElement &element )
{
  if ( mName != element.tagName() ) {
    qDebug( "SimpleBaseInputField: Wrong dom element passed: expected %s, got %s", mName.latin1(), element.tagName().latin1() );
    return;
  }

  if ( mType->subType() == Schema::SimpleType::TypeRestriction ) {
    InputField *field = childField( element.tagName() );
    if ( !field ) {
      qDebug( "SimpleBaseInputField: Child field %s does not exists", element.tagName().latin1() );
    } else {
      field->setXMLData( element );
    }
  } else {
    qDebug( "SimpleBaseInputField: Unsupported subtype" );
  }
}

void SimpleBaseInputField::xmlData( QDomDocument &document, QDomElement &parent )
{
  if ( mType->subType() == Schema::SimpleType::TypeRestriction ) {
    InputField *field = mFields.first();
    if ( !field ) {
      qDebug( "SimpleBaseInputField: No child found" );
    } else {
      field->xmlData( document, parent );
    }
  } else {
    qDebug( "SimpleBaseInputField: Unsupported subtype" );
  }
}

void SimpleBaseInputField::setData( const QString &data )
{
  if ( mType->subType() == Schema::SimpleType::TypeRestriction ) {
    InputField *field = mFields.first();
    if ( !field ) {
      qDebug( "SimpleBaseInputField: No child found" );
    } else {
      field->setData( data );
    }
  } else {
    qDebug( "SimpleBaseInputField: Unsupported subtype" );
  }
}

QString SimpleBaseInputField::data() const
{
  if ( mType->subType() == Schema::SimpleType::TypeRestriction ) {
    InputField *field = mFields.first();
    if ( !field ) {
      qDebug( "SimpleBaseInputField: No child found" );
    } else {
      field->data();
    }
  } else {
    qDebug( "SimpleBaseInputField: Unsupported subtype" );
  }

  return QString();
}

QWidget *SimpleBaseInputField::createWidget( QWidget *parent )
{
  if ( mType->subType() == Schema::SimpleType::TypeRestriction ) {
    InputField *field = mFields.first();
    if ( !field ) {
      qDebug( "SimpleBaseInputField: No child found" );
      return 0;
    } else {
      return field->createWidget( parent );
    }
  } else {
    qDebug( "SimpleBaseInputField: Unsupported subtype" );
    return 0;
  }
}
