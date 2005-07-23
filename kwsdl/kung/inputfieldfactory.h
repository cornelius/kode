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

#ifndef INPUTFIELD_FACTORY_H
#define INPUTFIELD_FACTORY_H

#include "inputfield.h"

#include <schema/types.h>

namespace Schema {
class SimpleType;
};

/**
  Factory class, which creates InputFields for a given type.
 */
class InputFieldFactory
{
  public:
    /**
      Destructor.
     */
    ~InputFieldFactory();

    /**
      Creates an input field of the given type with the given name.
     */
    InputField *createField( const QString &name, const QString &typeName, bool isList = false );

    /**
      Creates an input field of the given type with the given name.
     */
    InputField *createBasicField( const QString &name, const QString &typeName, const Schema::SimpleType *type, bool isList = false );

    /**
      Sets the types description, so that the factory can pass
      them to the input fields.
     */
    void setTypes( const Schema::Types &types );

    static InputFieldFactory* self();

  private:
    InputFieldFactory();

    static InputFieldFactory *mSelf;
    Schema::Types mTypes;
};

#endif
