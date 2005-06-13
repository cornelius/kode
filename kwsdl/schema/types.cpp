/* 
    This file is part of KDE Schema Parser

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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
 */

#include "types.h"

using namespace Schema;

void Types::setSimpleTypes( const SimpleType::List &simpleTypes )
{
  mSimpleTypes = simpleTypes;
}

SimpleType::List Types::simpleTypes() const
{
  return mSimpleTypes;
}

void Types::setComplexTypes( const ComplexType::List &complexTypes )
{
  mComplexTypes = complexTypes;
}

ComplexType::List Types::complexTypes() const
{
  return mComplexTypes;
}

void Types::setNameMap( const XSDType::NameMap &nameMap )
{
  mNameMap = nameMap;
}

XSDType::NameMap Types::nameMap() const
{
  return mNameMap;
}

QString Types::typeName( int type ) const
{
  return mNameMap[ type ];
}

void Types::setElements( const Element::List &elements )
{
  mElements = elements;
}

Element::List Types::elements() const
{
  return mElements;
}
