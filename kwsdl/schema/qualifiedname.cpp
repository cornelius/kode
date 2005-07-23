/* 
    This file is part of KDE Schema Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>
                       based on wsdlpull parser by Vivek Krishna

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

#include "qualifiedname.h"

using namespace Schema;

QualifiedName::QualifiedName( const QString &name )
{
  parse( name );
}

QualifiedName::QualifiedName()
{
}

void QualifiedName::operator=( const QString &name )
{
  parse( name );
}

QString QualifiedName::localName() const
{
  return mLocalName;
}

QString QualifiedName::prefix() const
{
  return mPrefix;
}

void QualifiedName::setNameSpace( const QString &nameSpace )
{
  mNameSpace = nameSpace;
}

QString QualifiedName::nameSpace() const
{
  return mNameSpace;
}

bool QualifiedName::operator==( const QualifiedName &qn ) const
{
  return (qn.nameSpace() == mNameSpace && qn.localName() == mLocalName );
}

void QualifiedName::parse( const QString &str )
{
  int pos = str.find( ':' );
  if ( pos != -1 ) {
    mPrefix = str.left( pos );
    mLocalName = str.mid( pos + 1 );
  } else
    mLocalName = str;

  if ( mLocalName.endsWith( "[]" ) )
    mLocalName.truncate( mLocalName.length() - 2 );
}

