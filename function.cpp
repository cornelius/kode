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

#include "function.h"

#include <kdebug.h>

using namespace KODE;

Function::Function()
  : mAccess( Public ), mIsConst( false ), mIsStatic( false )
{
}

Function::Function( const QString &name, const QString &returnType,
                    AccessSpecifier access, bool isStatic )
  :  mAccess( access ), mIsConst( false ), mIsStatic( isStatic ),
     mReturnType( returnType ), mName( name )
{
}
 
void Function::setConst( bool isConst )
{
  mIsConst = isConst;
} 
   
void Function::setStatic( bool isStatic )
{
  mIsStatic = isStatic;
}
   
void Function::addArgument( const QString &argument )
{
  mArguments.append( argument );
}

void Function::addInitializer( const QString &initializer )
{
  mInitializers.append( initializer );
}

void Function::setArgumentString( const QString &argumentString )
{
  mArguments.clear();
  QStringList arguments = QStringList::split( ",", argumentString );
  QStringList::ConstIterator it;
  for( it = arguments.begin(); it != arguments.end(); ++it ) {
    addArgument( *it );
  }
}

void Function::setBody( const QString &body )
{
  mBody = body;
}

void Function::setBody( const Code &body )
{
  mBody = body.text();
}

void Function::addBodyLine( const QString &bodyLine )
{
  mBody.append( bodyLine );
  if ( bodyLine.right( 1 ) != "\n" ) mBody.append( '\n' );
}

void Function::setAccess( AccessSpecifier a )
{
  mAccess = a;
}

QString Function::accessAsString() const
{
  switch( mAccess ) {
    case Public:
      return "public";
    case Protected:
      return "protected";
    case Private:
      return "private";
  }
  return QString::null;
}

void Function::setReturnType( const QString &str )
{
  mReturnType = str;
}

void Function::setName( const QString &str )
{
  mName = str;
}

void Function::setDocs( const QString &str )
{
  mDocs = str;
}
