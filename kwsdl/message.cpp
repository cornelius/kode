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

#include "message.h"

using namespace KWSDL;

Message::Part::Part()
{
}

Message::Part::Part( const QString &name, const QString &type )
 : mName( name ), mType( type )
{
}

Message::Message()
{
}

Message::Message( const QString &name )
  : mName( name )
{
}

void Message::setName( const QString &name )
{
  mName = name;
}

QString Message::name() const
{
  return mName;
}

void Message::addPart( const Part &part )
{
  mParts.append( part );
}

Message::Part Message::part( const QString &name ) const
{
  Part::List::ConstIterator it;
  for ( it = mParts.begin(); it != mParts.end(); ++it )
    if ( (*it).name() == name )
      return *it;

  return Part();
}

Message::Part::List Message::parts() const
{
  return mParts;
}
