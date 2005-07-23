/*
    This file is part of kdepim.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>

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

#include "membervariable.h"

#include <kdebug.h>

using namespace KODE;

MemberVariable::MemberVariable()
  : Variable()
{
}

MemberVariable::MemberVariable( const QString &name, const QString &type,
                                bool isStatic )
  : Variable( name, type, isStatic )
{
  QString n;

  if ( name.isEmpty() ) {
    n = "mUndefined";
  } else if ( name.length() >= 2  && name[ 0 ] == "m" &&
              ( name[ 1 ].upper() == name[ 1 ] ) ) {
    n = name;
  } else {
    n = "m";
    n += name[ 0 ].upper();
    n += name.mid( 1 );
  }

  setName( n );
}
