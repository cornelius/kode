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

#include "guielement.h"
#include "manager.h"

using namespace KXForms;

GuiElement::GuiElement( QWidget *parent, Manager *manager )
  : mParent( parent ), mLabel( 0 ), mWidget( 0 ), mManager( manager )
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

QDomElement GuiElement::createElement( const Reference &ref )
{
  QDomElement e;
  QDomElement itElement = context();
  foreach( Reference::Segment s, ref.segments() ) {
    if( s.isEmpty() )
      continue;
    e = itElement.firstChildElement( s.name() );
    int cnt = 1;
    while( !e.isNull() && cnt++ < s.count() ) {
      e = e.nextSiblingElement( s.name() );
    }
    if( e.isNull() ) {
      e = mManager->document().createElement( s.name() );
      itElement.appendChild( e );
    }
    itElement = e;
  }
  return e;
}

void GuiElement::loadData( const QDomElement &element )
{
  mContext = element;
  
  loadData();
}
