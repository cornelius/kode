/*
    This file is part of KDE.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#include "enum.h"

using namespace KODE;

Enum::Enum()
  : mCombinable( false )
{
}

Enum::Enum( const QString &name, const QStringList &enums, bool combinable )
  : mName( name ), mEnums( enums ), mCombinable( combinable )
{
}
    
QString Enum::declaration() const
{
  QString retval( "enum " + mName + " {" );
  uint value = 0;
  QStringList::ConstIterator it;
  for ( it = mEnums.begin(); it != mEnums.end(); ++it, ++value ) {
    if ( mCombinable ) {
      if ( it == mEnums.begin() )
        retval += QString( " %1 = %2" ).arg( *it ).arg( 1 << value );
      else
        retval += QString( ", %1 = %2" ).arg( *it ).arg( 1 << value );
    } else {
      if ( it == mEnums.begin() )
        retval += " " + *it;
      else
        retval += ", " + *it;
    }
  }

  retval += " };";

  return retval;
}
