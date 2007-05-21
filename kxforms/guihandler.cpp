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

#include "guihandler.h"

#include "manager.h"

#include <kdebug.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>

using namespace KXForms;

GuiHandler::GuiHandler( Manager *m )
  : mManager( m )
{
  mManager->setGuiHandler( this );
}

GuiHandler::~GuiHandler()
{
}

Manager *GuiHandler::manager() const
{
  return mManager;
}


QLayout *GuiHandler::getTopLayout() const
{
  return new QGridLayout();
}

void GuiHandler::addWidget( QLayout *l, QWidget *w ) const
{
  QGridLayout *gl = dynamic_cast< QGridLayout *>( l );
  if( !gl )
    return;

  gl->addWidget( w, gl->rowCount(), 0, 1, -1 );
}

void GuiHandler::addElement( QLayout *l, QWidget *label, QWidget *widget, int x, int y, int width, int height, 
            GuiElement::Properties *prop, bool indented ) const
{
  if( prop->layoutStyle == GuiElement::Horizontal ) {
    QGridLayout *gl = dynamic_cast< QGridLayout *>( l );
    if( !gl )
      return;

//     gl->setColumnStretch( (2*x)+1, 1 );
    if( label ) {
//       kDebug() << k_funcinfo << "Adding label at: (" << y << "," << 2*x << ")" << endl;
      gl->addWidget( label, y, (2*x), height, 1, Qt::AlignRight | Qt::AlignTop );
//       kDebug() << k_funcinfo << "Adding widget at: (" << y << "," << (2*x)+1 << ") (" << height << "," << (2*width)-1  << ")" << endl;
      if( indented ) {
        QHBoxLayout *hbl = new QHBoxLayout();
        hbl->addSpacing( 40 );
        hbl->addWidget( widget );
        gl->addLayout( hbl, y, (2*x)+1, 2*height, (2*width)-1 );
      }
      else
        gl->addWidget( widget, y, (2*x)+1, 2*height, (2*width)-1 );
    }
    else {
      if( indented ) {
        QHBoxLayout *hbl = new QHBoxLayout();
        hbl->addSpacing( 40 );
        hbl->addWidget( widget );
        gl->addLayout( hbl, y, (2*x), 2*height, 2*width );
      }
        gl->addWidget( widget, y, (2*x), 2*height, 2*width );
    }
  }
  else {
    QGridLayout *gl = dynamic_cast< QGridLayout *>( l );
    if( !gl )
      return;

//     gl->setColumnStretch( x, 1 );
    if( label ) {
//       kDebug() << k_funcinfo << "Adding label at: (" << 2*y << "," << x << ")" << endl;
      gl->addWidget( label, 2*y, 2*x, 1, 2*width, Qt::AlignLeft | Qt::AlignTop );
//       kDebug() << k_funcinfo << "Adding widget at: (" << (2*y)+1 << "," << x << ") (" << (2*height)-1 << "," << width  << ")" << endl;
      if( indented ) {
        QHBoxLayout *hbl = new QHBoxLayout();
        hbl->addSpacing( 40 );
        hbl->addWidget( widget );
        gl->addLayout( hbl, (2*y)+1, 2*x, (2*height)-1, 2*width );
      }
      else
        gl->addWidget( widget, (2*y)+1, 2*x, (2*height)-1, 2*width );
    }
    else {
      if( indented ) {
        QHBoxLayout *hbl = new QHBoxLayout();
        hbl->addSpacing( 40 );
        hbl->addWidget( widget );
        gl->addLayout( hbl, 2*y, 2*x, 2*height, 2*width );
      }
        gl->addWidget( widget, 2*y, 2*x, 2*height, 2*width );
    }
  }
}
