/*
    This file is part of Kung.

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

#include <schema/complextype.h>
#include <schema/simpletype.h>

#include "inputfield.h"

InputField::InputField( const QString &name )
  : QObject( 0, QString( "InputField( %1 )" ).arg( name ).latin1() ),
    mName( name )
{
}

InputField::~InputField()
{
  InputField::List::Iterator it;
  for ( it = mFields.begin(); it != mFields.end(); ++it )
    delete *it;

  mFields.clear();
}

QString InputField::name() const
{
  return mName;
}

void InputField::appendChild( InputField *field )
{
  mFields.append( field );
}

void InputField::removeChild( InputField *field )
{
  mFields.remove( field );
}

InputField *InputField::childField( const QString &name ) const
{
  InputField::List::ConstIterator it;
  for ( it = mFields.begin(); it != mFields.end(); ++it )
    if ( (*it)->name() == name )
      return *it;

  return 0;
}

InputField::List InputField::childFields() const
{
  return mFields;
}

SimpleInputField::SimpleInputField( const QString &name, const Schema::SimpleType *type )
  : InputField( name ), mType( 0 )
{
  if ( type ) {
    mType = new Schema::SimpleType();
    *mType = *type;
  }
}

SimpleInputField::~SimpleInputField()
{
  delete mType;
  mType = 0;
}

ComplexInputField::ComplexInputField( const QString &name, const Schema::ComplexType *type )
  : InputField( name ), mType( 0 )
{
  if ( type ) {
    mType = new Schema::ComplexType();
    *mType = *type;
  }
}

ComplexInputField::~ComplexInputField()
{
  delete mType;
  mType = 0;
}

#include "inputfield.moc"
