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

#include <qlabel.h>
#include <qlayout.h>

#include "inputfieldfactory.h"

#include "pageinputfield.h"

PageInputField::PageInputField( const QString &name, const KWSDL::Message &message )
 : InputField( name ),
   mMessage( message )
{
  KWSDL::Message::Part::List parts = message.parts();
  KWSDL::Message::Part::List::ConstIterator it;
  for ( it = parts.begin(); it != parts.end(); ++it ) {
    InputField *field = InputFieldFactory::self()->createField( (*it).name(), (*it).type() );
    if ( !field ) {
      qDebug( "PageInputField: Unable to create input field for %s (%s)", (*it).name().latin1(), (*it).type().latin1() );
    } else {
      appendChild( field );
    }
  }
}

void PageInputField::setXMLData( const QDomElement &element )
{
  if ( mName != element.tagName() ) {
    qDebug( "PageInputField: Wrong dom element passed: expected %s, got %s", mName.latin1(), element.tagName().latin1() );
    return;
  }

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement child = node.toElement();
    if ( !child.isNull() ) {
      InputField *field = childField( child.tagName() );
      if ( !field ) {
        qDebug( "PageInputField: Child field %s does not exists", child.tagName().latin1() );
      } else {
        field->setXMLData( child );
      }
    }

    node = node.nextSibling();
  }
}

void PageInputField::xmlData( QDomDocument &document, QDomElement &parent )
{
  QDomElement element = document.createElement( "ns1:" + mName );

  InputField::List::Iterator it;
  for ( it = mFields.begin(); it != mFields.end(); ++it )
    (*it)->xmlData( document, element );

  parent.appendChild( element );
}

void PageInputField::setData( const QString& )
{
}

QString PageInputField::data() const
{
  return QString();
}

QWidget *PageInputField::createWidget( QWidget *parent )
{
  QWidget *mInputWidget = new QWidget( parent, "PageInputWidget" );
  QGridLayout *layout = new QGridLayout( mInputWidget, 2, 2, 11, 6 );

  InputField::List::Iterator it;
  int row = 0;
  for ( it = mFields.begin(); it != mFields.end(); ++it, ++row ) {
    QLabel *label = new QLabel( (*it)->name(), mInputWidget );
    label->setAlignment( Qt::AlignTop );
    layout->addWidget( label, row, 0 );
    layout->addWidget( (*it)->createWidget( mInputWidget ), row, 1 );
  }

  layout->setRowStretch( ++row, 1 );

  return mInputWidget;
}

#include "pageinputfield.moc"
