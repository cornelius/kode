/*
    This file is part of KDE Schema Parser

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

#include "annotation.h"

#include <common/qname.h>

namespace XSD {

Annotation::Annotation()
{
}

Annotation::Annotation( const QDomElement &e )
  : mDomElement( e )
{
}

void Annotation::setDomElement( const QDomElement &e )
{
  mDomElement = e;
}

QDomElement Annotation::domElement() const
{
  return mDomElement;
}

bool Annotation::isDocumentation() const
{
  return QName( mDomElement.tagName() ).localName() == "documentation";
}

bool Annotation::isAppinfo() const
{
  return QName( mDomElement.tagName() ).localName() == "appinfo";
}

QString Annotation::documentation() const
{
  QString result;

  if ( isDocumentation() ) {
    result = mDomElement.text().trimmed();
  }
  
  return result;
}


QString Annotation::List::documentation() const
{
  QString result;

  foreach( Annotation a, *this ) {
    result.append( a.documentation() );
  }
  
  return result;
}

}
