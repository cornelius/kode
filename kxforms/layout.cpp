/*
    This file is part of KXForms.

    Copyright (c) 2007 Andre Duffeck <aduffeck@suse.de>

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

#include "layout.h"
#include "guielement.h"

#include <kdebug.h>

using namespace KXForms;

int Layout::Element::space() const
{
  //TODO:Implement some metric here
  int space = 20;

  foreach( GuiElement *e, element()->attributeElements() )
    space += 20;

  return 20;
}

int Layout::Element::width() const
{
  int width = 1;

  Layout::Element *it = rightElement();
  while( it ) {
    width++;
    it = it->rightElement();
  }

  return width;
}

int Layout::Element::height() const
{
  int height = 1;

  const Layout::Element *it = this;
  while( it ) {
    int h = 0;
    const Layout::Element *it2 = it;
    while( it2 ) {
      h++;
      foreach( GuiElement *e, it2->element()->attributeElements() )
        h++;
      it2 = it2->belowElement();
    }
    height = qMax( height, h );
    it = it->rightElement();
  }

  return height;
}

Layout::~Layout()
{
  foreach( Element *e, mOrderedList )
    delete e;
}

void Layout::addElement( GuiElement *guiElem )
{
  mElements.append( guiElem );
  mOrdered = false;
}

void Layout::order()
{
  if( mOrdered )
    return;

  foreach( Element *e, mOrderedList )
    delete e;
  mOrderedList.clear();

  foreach( GuiElement *e, mElements )
  {
    mOrderedList.append( new Element( e ) );
  }

  foreach( Element *e, mOrderedList ) {
    foreach( QDomElement pos, e->element()->properties()->positions ) {
      kDebug() << e->element()->ref().toString() << " : " << pos.text() << endl;
      Element *refEntry = findElement( Reference( pos.text() ) );
      if( refEntry ) {
        if( pos.tagName() == "rightOf" ) {
          e->setLeft( refEntry );
          refEntry->setRight( e );
          mOrderedList.removeAt( mOrderedList.indexOf( e ) );
        } else if( pos.tagName() == "leftOf" ) {
          e->setRight( refEntry );
          refEntry->setLeft( e );
          mOrderedList.removeAt( mOrderedList.indexOf( refEntry ) );
        } else if( pos.tagName() == "above" ) {
          e->setBelow( refEntry );
          refEntry->setAbove( e );
          mOrderedList.move( mOrderedList.indexOf( e ), mOrderedList.indexOf( refEntry ) );
        } else if( pos.tagName() == "below" ) {
          e->setAbove( refEntry );
          refEntry->setBelow( e );
          if( mOrderedList.contains( refEntry ) )
            mOrderedList.move( mOrderedList.indexOf( refEntry ), mOrderedList.indexOf( e ) );
          else
            mOrderedList.removeAt( mOrderedList.indexOf( e ) );
        }
      }
    }
  }
  mOrdered = true;
}

Layout::Element *Layout::findElement( const Reference &ref ) const
{
  Element *found = 0;
  foreach( Element *e, mOrderedList ) {
    kDebug() << e->element()->ref().toString() << " : " << ref.toString() << endl;
    found = findElement( e, ref );
    if( found )
      return found;
  }

  return 0;
}

Layout::Element *Layout::findElement( Layout::Element *e, const Reference &ref ) const
{
  if( e->element()->ref().matches( ref, false ) )
      return e;
  foreach( Element *e, e->children() ) {
    kDebug() << e->element()->ref().toString() << " : " << ref.toString() << endl;
    if( e->element()->ref().matches( ref, false ) )
      return e;
  }

  return 0;
}

QList< Layout::Element * > Layout::orderedList() const
{
  return mOrderedList;
}

int Layout::space() const
{
  int space = 0;
  foreach( Element *e, mOrderedList ) {
    space += e->space();
  }
  return space;
}

int Layout::width() const
{
  int width = 0;
  foreach( Element *e, mOrderedList ) {
    width = qMax( width, e->width() );
  }
  return width;
}

int Layout::height() const
{
  int height = 0;
  foreach( Element *e, mOrderedList ) {
    height = qMax( height, e->height() );
  }
  return height;
}

