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

#include "guielement.h"

using namespace KXForms;

GuiElement::GuiElement( QWidget *parent )
  : QWidget( parent )
{
}

GuiElement::~GuiElement()
{
}

void GuiElement::setRef( const Reference &ref )
{
  mRef = ref;
}

Reference GuiElement::ref() const
{
  return mRef;
}

QDomElement GuiElement::context() const
{
  return mContext;
}

// TODO: Move to Reference
QDomElement GuiElement::refElement() const
{
  // TODO: Make real check.
  Reference::Segment s = ref().segments().last();

  QDomNode n;
  for( n = context().firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == s.name() ) {
      return e;
    }
  }
  
  return QDomElement();
}

void GuiElement::loadData( const QDomElement &element )
{
  mContext = element;
  
  loadData();
}
