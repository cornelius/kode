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

#include <qlineedit.h>
#include <qvalidator.h>

#include <schema/simpletype.h>

#include "stringinputfield.h"

StringInputField::StringInputField( const QString &name, const QString &typeName, const Schema::SimpleType *type )
  : SimpleInputField( name, type ),
    mTypeName( typeName )
{
}

void StringInputField::setXMLData( const QDomElement &element )
{
  if ( mName != element.tagName() ) {
    qDebug( "StringInputField: Wrong dom element passed: expected %s, got %s", mName.latin1(), element.tagName().latin1() );
    return;
  }

  setData( element.text() );
}

void StringInputField::xmlData( QDomDocument &document, QDomElement &parent )
{
  QDomElement element = document.createElement( mName );
  element.setAttribute( "xsi:type", "xsd:" + mTypeName );
  QDomText text = document.createTextNode( data() );
  element.appendChild( text );

  parent.appendChild( element );
}

void StringInputField::setData( const QString &data )
{
  mValue = data;
}

QString StringInputField::data() const
{
  return mValue;
}

QWidget *StringInputField::createWidget( QWidget *parent )
{
  mInputWidget = new QLineEdit( parent );

  if ( mType ) {
    if ( mType->facetType() & Schema::SimpleType::LENGTH ) // TODO: using QValidator here?
      mInputWidget->setMaxLength( mType->facetLength() );

    if ( mType->facetType() & Schema::SimpleType::MINLEN ) {
      // TODO: using QValidator here?
      // mInputWidget->setMaxLength( type->facetMinimumLength() );
    }

    if ( mType->facetType() & Schema::SimpleType::MAXLEN )
      mInputWidget->setMaxLength( mType->facetMaximumLength() );

    if ( mType->facetType() & Schema::SimpleType::PATTERN )
      mInputWidget->setValidator( new QRegExpValidator( mType->facetPattern(), mInputWidget ) );
  }

  mInputWidget->setText( mValue );

  connect( mInputWidget, SIGNAL( textChanged( const QString& ) ),
           this, SLOT( inputChanged( const QString& ) ) );

  return mInputWidget;
}

void StringInputField::inputChanged( const QString &text )
{
  mValue = text;

  emit modified();
}

#include "stringinputfield.moc"
