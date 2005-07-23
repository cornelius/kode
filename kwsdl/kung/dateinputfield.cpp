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

#include <kdatepicker.h>

#include "dateinputfield.h"

DateInputField::DateInputField( const QString &name, const Schema::SimpleType *type )
  : SimpleInputField( name, type ),
    mValue( QDate::currentDate() )
{
}

void DateInputField::setXMLData( const QDomElement &element )
{
  if ( mName != element.tagName() ) {
    qDebug( "DateInputField: Wrong dom element passed: expected %s, got %s", mName.latin1(), element.tagName().latin1() );
    return;
  }

  setData( element.text() );
}

void DateInputField::xmlData( QDomDocument &document, QDomElement &parent )
{
  QDomElement element = document.createElement( mName );
  element.setAttribute( "xsi:type", "xsd:date" );
  QDomText text = document.createTextNode( data() );
  element.appendChild( text );

  parent.appendChild( element );
}

void DateInputField::setData( const QString &data )
{
  mValue = QDate::fromString( data, Qt::ISODate );
}

QString DateInputField::data() const
{
  return mValue.toString( Qt::ISODate );
}

QWidget *DateInputField::createWidget( QWidget *parent )
{
  mInputWidget = new KDatePicker( parent );

  mInputWidget->setDate( mValue );

  connect( mInputWidget, SIGNAL( dateChanged( QDate ) ),
           this, SLOT( inputChanged( QDate ) ) );

  return mInputWidget;
}

void DateInputField::inputChanged( QDate date )
{
  mValue = date;

  emit modified();
}

#include "dateinputfield.moc"
