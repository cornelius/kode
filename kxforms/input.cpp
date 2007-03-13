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

using namespace KXForms;

Input::Input( Manager *m, const QString &label, QWidget *parent )
  : GuiElement( parent, m )
{
  mLabel = new QLabel( label, mParent );
  mLineEdit = new KLineEdit( mParent );
  mWidget = mLineEdit;

  connect( mLineEdit, SIGNAL( returnPressed() ), SIGNAL( returnPressed() ) );
}

void Input::loadData()
{
  kDebug() << "Input::loadData() " << ref().toString() << "  context: "
    << context().tagName() << endl;

  QString txt = ref().applyString( context() );

  mLineEdit->setText( txt );
}

void Input::saveData()
{
  kDebug() << "Input::saveData()" << endl;

  Reference::Segment s = ref().segments().last();

  QString txt = mLineEdit->text();
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

#include "input.moc"
