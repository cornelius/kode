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

#include <kstaticdeleter.h>

#include <schema/types.h>

#include "binaryinputfield.h"
#include "boolinputfield.h"
#include "complexbaseinputfield.h"
#include "dateinputfield.h"
#include "doubleinputfield.h"
#include "enuminputfield.h"
#include "integerinputfield.h"
#include "listinputfield.h"
#include "simplebaseinputfield.h"
#include "stringinputfield.h"
#include "timeinputfield.h"

#include "inputfieldfactory.h"

InputFieldFactory *InputFieldFactory::mSelf = 0;
static KStaticDeleter<InputFieldFactory> inputFieldFactoryDeleter;

InputFieldFactory::InputFieldFactory()
{
}

InputFieldFactory::~InputFieldFactory()
{
}

void InputFieldFactory::setTypes( const Schema::Types &types )
{
  mTypes = types;
}

InputField *InputFieldFactory::createField( const QString &name, const QString &typeName, bool isList )
{
  Schema::SimpleType::List simpleTypes = mTypes.simpleTypes();
  Schema::SimpleType::List::ConstIterator simpleIt;
  for ( simpleIt = simpleTypes.begin(); simpleIt != simpleTypes.end(); ++simpleIt ) {
    if ( (*simpleIt).name() == typeName ) {
      if ( isList )
        return new ListInputField( name, typeName, 0 );
      else
        return new SimpleBaseInputField( name, &(*simpleIt) );
    }
  }

  Schema::ComplexType::List complexTypes = mTypes.complexTypes();
  Schema::ComplexType::List::ConstIterator complexIt;
  for ( complexIt = complexTypes.begin(); complexIt != complexTypes.end(); ++complexIt ) {
    if ( (*complexIt).name() == typeName ) {
      if ( isList )
        return new ListInputField( name, typeName, 0 );
      else
        return new ComplexBaseInputField( name, &(*complexIt) );
    }
  }

  return createBasicField( name, typeName, 0, isList );
}

InputField *InputFieldFactory::createBasicField( const QString &name, const QString &typeName,
                                                 const Schema::SimpleType *type, bool isList )
{
  if ( isList )
    return new ListInputField( name, typeName, 0 );

  if ( typeName == "string" || typeName == "language" ) {
    if ( type && type->facetType() & Schema::SimpleType::ENUM )
      return new EnumInputField( name, type );
    else
      return new StringInputField( name, typeName, type );
  } else if ( typeName == "int" || typeName == "unsignedInt" ||
              typeName == "byte" || typeName == "unsignedByte" ||
              typeName == "integer" || typeName == "positiveInteger" || typeName == "negativeInteger" ||
              typeName == "nonNegativeInteger" || typeName == "nonPositiveInteger" ||
              typeName == "long" || typeName == "unsignedLong" ||
              typeName == "short" || typeName == "unsignedShort" ) {
    return new IntegerInputField( name, typeName, type );
  } else if ( typeName == "double" || typeName == "float" || typeName == "decimal" ) {
    return new DoubleInputField( name, typeName, type );
  } else if ( typeName == "boolean" ) {
    return new BoolInputField( name, type );
  } else if ( typeName == "date" ) {
    return new DateInputField( name, type );
  } else if ( typeName == "time" ) {
    return new TimeInputField( name, type );
  } else if ( typeName == "base64Binary" ) {
    return new BinaryInputField( name, typeName, type );
  } else {
    qDebug( "InputFieldFactory: Unknown type %s", typeName.latin1() );
    return 0;
  }
}

InputFieldFactory* InputFieldFactory::self()
{
  if ( !mSelf )
    inputFieldFactoryDeleter.setObject( mSelf, new InputFieldFactory() );

  return mSelf;
}
