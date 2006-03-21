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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
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

QDomElement GuiElement::refElement() const
{
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
