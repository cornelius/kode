/*
    This file is part of KXForms.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "xmlbuilder.h"

#include <QDebug>

XmlBuilder::Node::Node( const QString &text )
  : mText( text ), mXml( 0 )
{
}

XmlBuilder::Node::Node( XmlBuilder *xml )
  : mXml( xml )
{
}

bool XmlBuilder::Node::isText() const
{
  return !mXml;
}

bool XmlBuilder::Node::isXml() const
{
  return mXml;
}

QString XmlBuilder::Node::text() const
{
  return mText;
}

XmlBuilder *XmlBuilder::Node::xml() const
{
  return mXml;
}


XmlBuilder::XmlBuilder( const QString &tagName )
  : mTagName( tagName ), mHasText( false ), mHasChildElements( false )
{
}

XmlBuilder::~XmlBuilder()
{
  foreach( Node n, mChildren ) {
    if ( n.xml() ) delete n.xml();
  }
}

XmlBuilder *XmlBuilder::tag( const QString &tagName, const QString &text )
{
  XmlBuilder *builder = new XmlBuilder( tagName );
  builder->text( text );

  mChildren.append( Node( builder ) );
  mHasChildElements = true;

  return builder;
}

XmlBuilder *XmlBuilder::attribute( const QString &name, const QString &value )
{
  mAttributes[ name ] = value;
  
  return this;
}

XmlBuilder *XmlBuilder::text( const QString &text )
{
  if ( !text.isEmpty() ) {
    mChildren.append( Node( text ) );
    mHasText = true; 
  }
   
  return this;
}

QString XmlBuilder::print( int indentation, bool newLine ) const
{
  QString out = indent( indentation ) + "<" + mTagName;
  
  QMap<QString,QString>::ConstIterator it;
  for( it = mAttributes.begin(); it != mAttributes.end(); ++it ) {
    out += " " + it.key() + "=\"" + it.value() + "\"";
  }

  if ( isEmpty() ) {
    out += "/>\n";
  } else {
    out += ">";
    if ( !hasText() ) out += "\n";
  
    foreach( Node child, mChildren ) {
      if ( child.isText() ) {
        out += child.text();
      } else {
        if ( hasText() ) {
          out += child.xml()->print( 0, false );
        } else {
          out += child.xml()->print( indentation + 2 );
        }
      }
    }
    
    if ( !hasText() ) out += indent( indentation );
    out += "</" + mTagName + ">";
    if ( newLine ) out += "\n";
  }

  return out;
}

QString XmlBuilder::indent( int indent ) const
{
  QString txt;
  for( int i = 0; i < indent; ++i ) {
    txt += " ";
  }
  return txt;
}

bool XmlBuilder::isEmpty() const
{
  return mChildren.isEmpty();
}

bool XmlBuilder::hasText() const
{
  return mHasText;
}

bool XmlBuilder::hasChildElements() const
{
  return mHasChildElements;
}
