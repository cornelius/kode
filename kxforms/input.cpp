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

#include "input.h"

#include "manager.h"

#include <kdebug.h>
#include <klineedit.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QCheckBox>

using namespace KXForms;

Input::Input( Manager *m, const QString &label, QWidget *parent, Properties *p )
  : GuiElement( parent, m, p )
{
  mManager->dispatcher()->registerElement( this );
  mLabel = new QLabel( label, mParent );
  if( mProperties->type == "xs:integer" ) {
    mSpinBox = new QSpinBox( mParent );
    mWidget = mSpinBox;
    connect( mSpinBox, SIGNAL( valueChanged(int) ), SLOT( emitValueChanged() ) );
  } else if( mProperties->type == "xs:boolean" ) {
    mCheckBox = new QCheckBox( mParent );
    mWidget = mCheckBox;
    connect( mCheckBox, SIGNAL( stateChanged(int) ), SLOT( emitValueChanged() ) );
  } else {
    mLineEdit = new KLineEdit( mParent );
    mWidget = mLineEdit;
    connect( mLineEdit, SIGNAL( textChanged(QString) ), SLOT( emitValueChanged() ) );
    connect( mLineEdit, SIGNAL( returnPressed() ), SIGNAL( returnPressed() ) );
  }


  applyProperties();
}

void Input::loadData()
{
  kDebug() << "Input::loadData() " << ref().toString() << "  context: "
    << context().tagName() << endl;
  if( mProperties->type == "xs:integer" ) {
    QString txt = ref().applyString( context() );
    bool ok;
    int value = txt.toInt( &ok );
    if( ok )
      mSpinBox->setValue( value );
  } else if( mProperties->type == "xs:boolean" ) {
    QString txt = ref().applyString( context() );
    mCheckBox->setChecked( txt == "true" );
  } else {
    QString txt = ref().applyString( context() );
    mLineEdit->setText( txt );
  }
}

void Input::saveData()
{
  kDebug() << "Input::saveData()" << endl;

  Reference::Segment s = ref().segments().last();

  QString txt;
  if( mProperties->type == "xs:integer" ) {
    txt = QString::number( mSpinBox->value() );
  } else if( mProperties->type == "xs:boolean" ) {
    txt = mCheckBox->isChecked() ? "true" : false;
  } else {
    txt = mLineEdit->text();
  }

  if ( s.isAttribute() ) {
    ref().applyAttributeContext( context() ).setAttribute( s.name(), txt );
  } else {
    QDomElement e = ref().applyElement( context() );
    if ( e.isNull() ) {
      e = createElement( ref() );
    }
    QDomText t = e.firstChild().toText();
    if ( t.isNull() ) {
      t = mManager->document().createTextNode( txt );
      e.appendChild( t ); 
    } else {
      t.setData( txt );
    }
  }
}

void Input::emitValueChanged()
{
  QString value;

  if( mProperties->type == "xs:integer" ) {
    value = QString::number( mSpinBox->value() );
  } else if( mProperties->type == "xs:boolean" ) {
    value = mCheckBox->isChecked() ? "true" : "false";
  } else {
    value = mLineEdit->text();
  }
  emit valueChanged( ref().toString(), value );
}

#include "input.moc"
