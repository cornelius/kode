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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "class.h"

#include <kdebug.h>

using namespace KODE;

Class::Class()
{
  mBaseClasses.setAutoDelete( true );
}

Class::Class( const QString &name )
  : mName( name )
{
}

Class::Class( const Class &c )
{
  *this = c;
}

Class &Class::operator=( const Class &c )
{
  if ( this == &c ) return *this;

  mName = c.mName;
  mFunctions = c.mFunctions;
  mMemberVariables = c.mMemberVariables;
  mIncludes = c.mIncludes;

  QPtrListIterator<Class> it( c.mBaseClasses );
  while( it.current() ) {
    mBaseClasses.append( new Class( *( it.current() ) ) );
    ++it;
  }

  mTypedefs = c.mTypedefs;
  
  return *this;
}

void Class::setName( const QString &name )
{
  mName = name;
}

void Class::addInclude( const QString &include )
{
  if ( mIncludes.find( include ) == mIncludes.end() ) {
    mIncludes.append( include );
  }
}

void Class::addBaseClass( const Class &c )
{
  mBaseClasses.append( new Class( c ) );
}

void Class::addFunction( const Function &function )
{
  mFunctions.append( function );
}

void Class::addMemberVariable( const MemberVariable &v )
{
  mMemberVariables.append( v );
}

Class::List Class::baseClasses() const
{
  Class::List b;
  
  QPtrListIterator<Class> it( mBaseClasses );
  while( it.current() ) {
    b.append( Class( *( it.current() ) ) );
    ++it;
  }

  return b;
}

void Class::addTypedef( const Typedef &t )
{
  mTypedefs.append( t );
}

bool Class::isValid() const
{
  return !mName.isEmpty();
}

bool Class::hasFunction( const QString &functionName ) const
{
  Function::List::ConstIterator it;
  for( it = mFunctions.begin(); it != mFunctions.end(); ++it ) {
    if ( (*it).name() == functionName ) return true;
  }

  return false;
}
