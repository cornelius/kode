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

#include <ktimewidget.h>

#include "timeinputfield.h"

TimeInputField::TimeInputField( const QString &name, const Schema::SimpleType *type )
  : SimpleInputField( name, type ),
    mValue( QTime::currentTime() )
{
}

void TimeInputField::setXMLData( const QDomElement &element )
{
  if ( mName != element.tagName() ) {
    qDebug( "TimeInputField: Wrong dom element passed: expected %s, got %s", mName.latin1(), element.tagName().latin1() );
    return;
  }

  setData( element.text() );
}

void TimeInputField::xmlData( QDomDocument &document, QDomElement &parent )
{
  QDomElement element = document.createElement( mName );
  element.setAttribute( "xsi:type", "xsd:time" );
  QDomText text = document.createTextNode( data() );
  element.appendChild( text );

  parent.appendChild( element );
}

void TimeInputField::setData( const QString &data )
{
  mValue = QTime::fromString( data, Qt::ISODate );
}

QString TimeInputField::data() const
{
  return mValue.toString( Qt::ISODate );
}

QWidget *TimeInputField::createWidget( QWidget *parent )
{
  mInputWidget = new KTimeWidget( parent );

  mInputWidget->setTime( mValue );

  connect( mInputWidget, SIGNAL( valueChanged( const QTime& ) ),
           this, SLOT( inputChanged( const QTime& ) ) );

  return mInputWidget;
}

void TimeInputField::inputChanged( const QTime &time )
{
  mValue = time;

  emit modified();
}

#include "timeinputfield.moc"
