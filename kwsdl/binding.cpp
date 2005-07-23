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

#include "binding.h"

using namespace KWSDL;

Binding::Operation::Operation()
{
}

Binding::Operation::Operation( const QString &name, const QString &action )
  : mName( name ), mAction( action )
{
}

Binding::Binding()
{
}

Binding::Binding( const QString &name, const QString &type )
  : mName( name ), mType( type )
{
}


void Binding::addOperation( const Operation &operation )
{
  mOperations.append( operation );
}

Binding::Operation Binding::operation( const QString &name ) const
{
  Operation::List::ConstIterator it;
  for ( it = mOperations.begin(); it != mOperations.end(); ++it )
    if ( (*it).name() == name )
      return *it;

  return Operation();
}

Binding::Operation::List Binding::operations() const
{
  return mOperations;
}
