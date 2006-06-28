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

#include <QtCore/QStringList>

#include "class.h"

using namespace KODE;

class Class::Private
{
  public:
    Private()
      : mUseDPointer( false )
    {
    }

    QString mName;
    QString mNameSpace;
    bool mUseDPointer;
    Function::List mFunctions;
    MemberVariable::List mMemberVariables;
    QStringList mIncludes;
    QStringList mForwardDeclarations;
    QStringList mHeaderIncludes;
    QList<Class> mBaseClasses;
    Typedef::List mTypedefs;
    Enum::List mEnums;
    QString mDocs;
};

Class::Class()
  : d( new Private )
{
}

Class::Class( const Class &other )
  : d( new Private )
{
  *d = *other.d;
}

Class::Class( const QString &name, const QString &nameSpace )
  : d( new Private )
{
  d->mName = name;
  d->mNameSpace = nameSpace;
}

Class::~Class()
{
  delete d;
}

Class& Class::operator=( const Class &other )
{
  if ( this == &other )
    return *this;

  *d = *other.d;

  return *this;
}

void Class::setName( const QString &name )
{
  d->mName = name;
}

QString Class::name() const
{
  return d->mName;
}

void Class::setNameSpace( const QString &nameSpace )
{
  d->mNameSpace = nameSpace;
}

QString Class::nameSpace() const
{
  return d->mNameSpace;
}

void Class::setUseDPointer( const bool &useDPointer )
{
  d->mUseDPointer = useDPointer;
}

bool Class::useDPointer() const
{
  return d->mUseDPointer;
}

void Class::addInclude( const QString &include,
                        const QString &forwardDeclaration )
{
  if ( !include.isEmpty() && !d->mIncludes.contains( include ) )
      d->mIncludes.append( include );

  if ( !forwardDeclaration.isEmpty() &&
       !d->mForwardDeclarations.contains( forwardDeclaration ) )
    d->mForwardDeclarations.append( forwardDeclaration );
}

void Class::addIncludes( const QStringList &files,
                         const QStringList &forwardDeclarations )
{
  for ( int i = 0; i < files.count(); ++i ) {
    if ( !d->mIncludes.contains( files[ i ] ) )
      if ( !files[ i ].isEmpty() )
        d->mIncludes.append( files[ i ] );
  }

  for ( int i = 0; i < forwardDeclarations.count(); ++i ) {
    if ( !d->mForwardDeclarations.contains( forwardDeclarations[ i ] ) )
      d->mForwardDeclarations.append( forwardDeclarations[ i ] );
  }
}

QStringList Class::includes() const
{
  return d->mIncludes;
}

QStringList Class::forwardDeclarations() const
{
  return d->mForwardDeclarations;
}

void Class::addHeaderInclude( const QString &include )
{
  if ( include.isEmpty() )
    return;

  if ( !d->mHeaderIncludes.contains( include ) )
    d->mHeaderIncludes.append( include );
}

void Class::addHeaderIncludes( const QStringList &includes )
{
  QStringList::ConstIterator it;
  for ( it = includes.begin(); it != includes.end(); ++it )
    addHeaderInclude( *it );
}

QStringList Class::headerIncludes() const
{
  return d->mHeaderIncludes;
}

void Class::addBaseClass( const Class &c )
{
  d->mBaseClasses.append( c );
}

Class::List Class::baseClasses() const
{
  return d->mBaseClasses;
}

void Class::addFunction( const Function &function )
{
  d->mFunctions.append( function );
}

Function::List Class::functions() const
{
  return d->mFunctions;
}

void Class::addMemberVariable( const MemberVariable &v )
{
  d->mMemberVariables.append( v );
}

MemberVariable::List Class::memberVariables() const
{
  return d->mMemberVariables;
}

void Class::addTypedef( const Typedef &typeDefinition )
{
  d->mTypedefs.append( typeDefinition );
}

Typedef::List Class::typedefs() const
{
  return d->mTypedefs;
}

void Class::addEnum( const Enum &enumValue )
{
  d->mEnums.append( enumValue );
}

Enum::List Class::enums() const
{
  return d->mEnums;
}

bool Class::isValid() const
{
  return !d->mName.isEmpty();
}

bool Class::hasFunction( const QString &functionName ) const
{
  Function::List::ConstIterator it;
  for ( it = d->mFunctions.begin(); it != d->mFunctions.end(); ++it ) {
    if ( (*it).name() == functionName )
      return true;
  }

  return false;
}

bool Class::isQObject() const
{
  Function::List::ConstIterator it;
  for ( it = d->mFunctions.begin(); it != d->mFunctions.end(); ++it ) {
    if ( (*it).access() & Function::Signal || (*it).access() & Function::Slot )
      return true;
  }

  return false;
}

void Class::setDocs( const QString &str )
{
  d->mDocs = str;
}

QString Class::docs() const
{
  return d->mDocs;
}
