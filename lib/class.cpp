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

#include "class.h"

#include <kdebug.h>

using namespace KODE;

Class::Class()
  : mUseDPointer( false )
{
}

Class::Class( const QString &name, const QString &nameSpace )
  : mName( name ), mNameSpace( nameSpace ), mUseDPointer( false )
{
}

void Class::setName( const QString &name )
{
  mName = name;
}

void Class::setNameSpace( const QString &nameSpace )
{
  mNameSpace = nameSpace;
}

void Class::setUseDPointer( const bool &useDPointer )
{
  mUseDPointer = useDPointer;
}

void Class::addInclude( const QString &include,
  const QString &forwardDeclaration )
{
  if ( !include.isEmpty() && !mIncludes.contains( include ) )
      mIncludes.append( include );

  if ( !forwardDeclaration.isEmpty() &&
       !mForwardDeclarations.contains( forwardDeclaration ) )
    mForwardDeclarations.append( forwardDeclaration );
}

void Class::addIncludes( const QStringList &files,
                         const QStringList &forwardDeclarations )
{
  for ( int i = 0; i < files.count(); ++i ) {
    if ( !mIncludes.contains( files[ i ] ) )
      if ( !files[ i ].isEmpty() )
        mIncludes.append( files[ i ] );
  }

  for ( int i = 0; i < forwardDeclarations.count(); ++i ) {
    if ( !mForwardDeclarations.contains( forwardDeclarations[ i ] ) )
      mForwardDeclarations.append( forwardDeclarations[ i ] );
  }
}

void Class::addHeaderInclude( const QString &include )
{
  if ( include.isEmpty() )
    return;

  if ( !mHeaderIncludes.contains( include ) )
    mHeaderIncludes.append( include );
}

void Class::addHeaderIncludes( const QStringList &includes )
{
  QStringList::ConstIterator it;
  for ( it = includes.begin(); it != includes.end(); ++it )
    addHeaderInclude( *it );
}

void Class::addBaseClass( const Class &c )
{
  mBaseClasses.append( c );
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
  return mBaseClasses;
}

void Class::addTypedef( const Typedef &t )
{
  mTypedefs.append( t );
}

void Class::addEnum( const Enum &e )
{
  mEnums.append( e );
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

bool Class::isQObject() const
{
  Function::List::ConstIterator it;
  for( it = mFunctions.begin(); it != mFunctions.end(); ++it ) {
    if ( (*it).access() & Function::Signal || (*it).access() & Function::Slot )
      return true;
  }

  return false;
}

void Class::setDocs( const QString &str )
{
  mDocs = str;
}
