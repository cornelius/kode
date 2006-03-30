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

#include "hints.h"

#include <QDebug>
#include <QDomDocument>
#include <QDomElement>

using namespace KXForms;

Hint::Hint()
{
}

Hint::Hint( const QString &ref )
  : mRef( ref )
{
}

bool Hint::isValid() const
{
  return !mRef.isEmpty();
}

void Hint::setRef( const QString &r )
{
  mRef = r;
}

QString Hint::ref() const
{
  return mRef;
}

void Hint::setLabel( const QString &l )
{
  mLabel = l;
}

QString Hint::label() const
{
  return mLabel;
}

void Hint::setEnumValue( const QString &value, const QString &replacement )
{
  mEnums.insert( value, replacement );
}

QString Hint::enumValue( const QString &value )
{
  if ( mEnums.contains( value ) ) return mEnums[ value ];
  else return QString();
}


Hints::Hints()
{
}

bool Hints::parseFile( QFile &file )
{
  qDebug() << "---Hints::parseFile() " << file.fileName();

  QDomDocument doc;
  doc.setContent( &file );
  
  return parse( doc );
}

bool Hints::parseString( const QString &data )
{
  qDebug() << "---Hints::parseString() ";

  QDomDocument doc;
  doc.setContent( data );

  return parse( doc );
}

bool Hints::parse( const QDomDocument &doc )
{  
  QDomElement documentElement = doc.documentElement();
  QDomNode n;
  for( n = documentElement.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    parseHint( e );
  }

  qDebug() << "---Hints::parse() done";

  return true;
}

void Hints::parseHint( const QDomElement &element )
{
  QString ref = element.attribute( "ref" );
  if ( ref.isEmpty() ) {
    qDebug() << "Error: No reference attribute in hint tag.";
    return;
  }
  Hint hint( ref );

  qDebug() << "Hints::parseHint()" << ref;

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "label" ) {
      hint.setLabel( e.text() );
    } else if ( e.tagName() == "enum" ) {
      hint.setEnumValue( e.attribute( "value" ), e.text() );
    }
  }

  mHints[ ref ] = hint;
}

Hint Hints::hint( const QString &ref ) const
{
  if ( mHints.contains( ref ) ) return mHints[ ref ];
  else return Hint();
}

Hint::List Hints::hints() const
{
  Hint::List result;

  foreach( Hint h, mHints ) {
    result.append( h );
  }
  
  return result;
}
