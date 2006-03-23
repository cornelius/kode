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

#include "input.h"

#include <klineedit.h>
#include <kdebug.h>

#include <QLabel>
#include <QHBoxLayout>

using namespace KXForms;

Input::Input( Manager *m, const QString &label, QWidget *parent )
  : GuiElement( parent ), mManager( m )
{
  QBoxLayout *topLayout = new QHBoxLayout( this );

  QLabel *l = new QLabel( label, this );
  topLayout->addWidget( l );

  mLineEdit = new KLineEdit( this );
  topLayout->addWidget( mLineEdit );
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
    context().setAttribute( s.name(), txt );
  } else {
    QDomElement e = ref().apply( context() );
    QDomText t = e.firstChild().toText();
    t.setData( txt );
  }
}
