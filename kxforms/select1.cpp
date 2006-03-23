/*
    This file is part of KXForms.

    Copyright (c) 2005,2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "select1.h"

#include <kdebug.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>

using namespace KXForms;

Select1::Select1( Manager *m, const QString &label, QWidget *parent )
  : GuiElement( parent ), mManager( m )
{
  QBoxLayout *topLayout = new QHBoxLayout( this );

  QLabel *l = new QLabel( label, this );
  topLayout->addWidget( l );

  mComboBox = new QComboBox( this );
  topLayout->addWidget( mComboBox );
}

void Select1::parseElement( const QDomElement &formElement )
{
  QDomNode n;
  for( n = formElement.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "xf:item" ) {
      QString label;
      QString value;
      QDomNode n2;
      for( n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
        QDomElement e2 = n2.toElement();
        if ( e2.tagName() == "xf:label" ) {
          label = e2.text();
        } else if ( e2.tagName() == "xf:value" ) {
          value = e2.text();
        }
      }
      if ( !label.isEmpty() && !value.isEmpty() ) {
        mComboBox->insertItem( label );
        mValues.append( value );
      }
    }
  }
}

void Select1::loadData()
{
  kDebug() << "Select1::loadData() " << ref().toString() << "  context: "
    << context().tagName() << endl;

  Reference::Segment s = ref().segments().last();

  QString txt = ref().applyString( context() );

  int count = 0;
  QStringList::ConstIterator it;
  for( it = mValues.begin(); it != mValues.end(); ++it, ++count ) {
    if ( *it == txt ) break;
  }
  if ( it != mValues.end() ) {
    mComboBox->setCurrentItem( count );
  } else {
    kWarning() << "Select1::loadData() unknown value: " << txt << endl;
  }
}

void Select1::saveData()
{
  kDebug() << "Select1::saveData()" << endl;

  Reference::Segment s = ref().segments().last();

  QString txt = mValues[ mComboBox->currentItem() ];
  if ( s.isAttribute() ) {
    context().setAttribute( s.name(), txt );
  } else {
    QDomElement e = ref().apply( context() );
    QDomText t = e.firstChild().toText();
    t.setData( txt );
  }
}
