/*
    This file is part of KDE.

    Copyright (c) 2005 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "input.h"

#include <klineedit.h>
#include <kdebug.h>

#include <qlabel.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3HBoxLayout>

using namespace KXForms;

Input::Input( Manager *m, const QString &label, QWidget *parent )
  : GuiElement( parent ), mManager( m )
{
  Q3BoxLayout *topLayout = new Q3HBoxLayout( this );

  QLabel *l = new QLabel( label, this );
  topLayout->addWidget( l );

  mLineEdit = new KLineEdit( this );
  topLayout->addWidget( mLineEdit );
}

void Input::loadData()
{
  kdDebug() << "Input::loadData() " << ref().toString() << "  context: "
    << context().tagName() << endl;

  Reference::Segment s = ref().segments().last();

  QString txt;
  if ( s.isAttribute() ) {
    kdDebug() << "S.NAME: " << s.name() << endl;
    txt = context().attribute( s.name() );
  } else {
    kdDebug() << "S.ELE" << endl;
    txt = refElement().text();
  }

  mLineEdit->setText( txt );
}

void Input::saveData()
{
  kdDebug() << "Input::saveData()" << endl;

  Reference::Segment s = ref().segments().last();

  QString txt = mLineEdit->text();
  if ( s.isAttribute() ) {
    context().setAttribute( s.name(), txt );
  } else {
    QDomElement e = refElement();
    QDomText t = e.firstChild().toText();
    t.setData( txt );
  }
}
