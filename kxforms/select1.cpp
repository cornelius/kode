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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "select1.h"
#include "manager.h"

#include <kdebug.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QRadioButton>
#include <QListWidget>

using namespace KXForms;

Select1::Select1( Manager *m, const QString &label, QWidget *parent, Properties *p )
  : GuiElement( parent, m, p )
{
  mManager->dispatcher()->registerElement( this );
    mLabel = new QLabel( label, parent );
  if( mProperties->appearance == Full ) {
    mWidget = new QWidget( parent );
    mWidget->setLayout( new QVBoxLayout( parent ) );
  } else if( mProperties->appearance == Compact ) {
    mListWidget = new QListWidget( parent );
    mListWidget->setSelectionMode( QAbstractItemView::SingleSelection );
    mWidget = mListWidget;
  } else {
    mComboBox = new QComboBox( parent );
    mWidget = mComboBox;
  }

  applyProperties();
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
        mValues.append( value );
        if( mProperties->appearance == Full ) {
          QRadioButton *radio = new QRadioButton( label, mWidget );
          mWidget->layout()->addWidget( radio );
          mRadioButtons.append( radio );
        } else if( mProperties->appearance == Compact ) {
          mListWidget->addItem( label );
        } else {
          mComboBox->addItem( label );
        }
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
    if( mProperties->appearance == Full ) {
      QRadioButton *radio = mRadioButtons[count];
      if( radio )
        radio->setChecked( true );
    } else if( mProperties->appearance == Compact ) {
      mListWidget->setCurrentRow( count );
    } else {
      mComboBox->setCurrentIndex( count );
    }
  } else {
    kWarning() << "Select1::loadData() unknown value: " << txt << endl;
  }
}

void Select1::saveData()
{
  kDebug() << "Select1::saveData()" << endl;
  kDebug() << ref().toString() << endl;
  kDebug() << "Context: " << context().nodeName() << endl;
  Reference::Segment s = ref().segments().last();

  QString txt;
  if( mProperties->appearance == Full ) {
    for( int i = 0; i < mRadioButtons.size(); ++i ) {
      if( mRadioButtons[i]->isChecked() ) {
        txt = mValues[ i ];
        break;
      }
    }
  } else if( mProperties->appearance == Compact ) {
    txt = mValues[ mListWidget->currentRow() ];
  } else { 
    txt = mValues[ mComboBox->currentIndex() ];
  }

  if ( s.isAttribute() ) {
    context().setAttribute( s.name(), txt );
  } else {
    QDomElement e = ref().applyElement( context() );
    if ( e.isNull() ) {
      e = createElement( ref() );
    }
    QDomNode n = e.firstChild();
    if( n.isNull() ) {
      n = mManager->document().createElement( txt );
      e.appendChild( n );
    }
    else {
      if( n.isText() ) {
        QDomText t = n.toText();
        t.setData( txt );
      } else if( n.isElement() ){
        n.toElement().setTagName( txt );
      }
    }
  }
}
