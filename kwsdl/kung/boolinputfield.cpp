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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include <qcheckbox.h>

#include "boolinputfield.h"

BoolInputField::BoolInputField( const QString &name, const Schema::SimpleType *type )
  : QObject( 0, "BoolInputField" ), SimpleInputField( name, type ),
    mValue( false )
{
}

void BoolInputField::setXMLData( const QDomElement &element )
{
  if ( mName != element.tagName() ) {
    qDebug( "BoolInputField: Wrong dom element passed: expected %s, got %s", mName.latin1(), element.tagName().latin1() );
    return;
  }

  mValue = ( element.text().lower() == "true" );
}

QDomElement BoolInputField::xmlData( QDomDocument &document )
{
  QDomElement element = document.createElement( mName );
  element.setAttribute( "xsi:type", "xsd:boolean" );
  QDomText text = document.createTextNode( mValue ? "true" : "false" );
  element.appendChild( text );

  return element;
}

QWidget *BoolInputField::createWidget( QWidget *parent )
{
  mInputWidget = new QCheckBox( parent );

  mInputWidget->setChecked( mValue );

  connect( mInputWidget, SIGNAL( toggled( bool ) ),
           this, SLOT( inputChanged( bool ) ) );

  return mInputWidget;
}

void BoolInputField::inputChanged( bool value )
{
  mValue = value;
}

#include "boolinputfield.moc"
