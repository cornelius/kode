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
#include "dispatcher.h"

#include <kdebug.h>

using namespace KXForms;

GuiElement::GuiElement( QWidget *parent, Manager *manager, Properties *p )
  : mParent( parent ), mLabel( 0 ), mWidget( 0 ), mManager( manager ),
    mProperties( p )
{
  mManager->dispatcher()->registerElement( this );
  if( !mProperties )
    mProperties = new Properties;
}

GuiElement::~GuiElement()
{
  delete mProperties;
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

void GuiElement::applyProperties()
{
  if( widget() )
    widget()->setEnabled( !mProperties->readonly );
}

void GuiElement::parseProperties( const QDomElement &element, Properties *properties )
{
  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "properties" ) {
      QDomNode n2;
      for( n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
        QDomElement e2 = n2.toElement();
        if ( e2.tagName() == "readonly" ) {
          properties->readonly = (e2.text() == "true");
        } else if ( e2.tagName() == "relevant" ) {
          QString elem = e2.attribute( "ref" );
          QString value = e2.text();
          if( !elem.isEmpty() && !value.isEmpty() )
            properties->relevance[elem] = value;
        } else if ( e2.tagName() == "layout" ) {
          QDomNode n3;
          bool ok;
          for( n3 = e2.firstChild(); !n3.isNull(); n3 = n3.nextSibling() ) {
            QDomElement e3 = n3.toElement();
            if ( e3.tagName() == "page" ) {
              properties->page = e3.text().toInt( &ok );
              if( !ok ) properties->page = -1;
            } else if ( e3.tagName() == "position" ) {
              properties->position = e3.text().toInt( &ok );
              if( !ok ) properties->position = -1;
            } else if ( e3.tagName() == "valign" ) {
              if( e3.text() == "bottom" )
                properties->valign = Qt::AlignBottom;
              else if( e3.text() == "center" )
                properties->valign = Qt::AlignVCenter;
              else
                properties->valign = Qt::AlignTop;
            } else if ( e3.tagName() == "halign" ) {
              if( e3.text() == "right" )
                properties->valign = Qt::AlignRight;
              else if( e3.text() == "center" )
                properties->valign = Qt::AlignHCenter;
              else if( e3.text() == "center" )
                properties->valign = Qt::AlignLeft;
              else
                properties->valign = 0;
            } else if ( e3.tagName() == "appearance" ) {
              if( e3.text() == "minimal" )
                properties->appearance = Minimal;
              else if( e3.text() == "compact" )
                properties->appearance = Compact;
              else if( e3.text() == "full" )
                properties->appearance = Full;
            }
          } 
        }
      }
    }
  }
}

#include "guielement.moc"
