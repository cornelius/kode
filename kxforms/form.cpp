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

#include "form.h"

#include "formgui.h"

#include <kdebug.h>

using namespace KXForms;

Form::Form( Manager *m )
  : mManager( m )
{
}

void Form::setRef( const QString &ref )
{
  mRef = ref;
}

QString Form::ref() const
{
  return mRef;
}

void Form::setElement( const QDomElement &element )
{
  mElement = element;
}

QDomElement Form::element() const
{
  return mElement;
}

bool Form::isValid() const
{
  return !mRef.isEmpty();
}

QString Form::label() const
{
  QDomNode n;
  for ( n = mElement.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    QString tag = e.tagName();
    if ( tag == "xf:label" ) {
      return e.text();
    }
  }
  return QString();
}
