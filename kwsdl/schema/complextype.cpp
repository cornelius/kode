/* 
    This file is part of KDE Schema Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>
                       based on wsdlpull parser by Vivek Krishna

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

#include "complextype.h"

using namespace Schema;

ComplexType::ComplexType( const QString &nameSpace )
  : XSDType( nameSpace ), mAnonymous( false ), mIsArray( false )
{
}

ComplexType::ComplexType()
  : mAnonymous( false ), mIsArray( false )
{
}

ComplexType::~ComplexType()
{
}

void ComplexType::setDocumentation( const QString &documentation )
{
  mDocumentation = documentation;
}

QString ComplexType::documentation() const
{
  return mDocumentation;
}

void ComplexType::setBaseTypeName( const QName &baseTypeName )
{
  mBaseTypeName = baseTypeName;
}

QName ComplexType::baseTypeName() const
{
  return mBaseTypeName;
}

void ComplexType::setBaseDerivation( Derivation derivation )
{
  mBaseDerivation = derivation;
}

ComplexType::Derivation ComplexType::baseDerivation() const
{
  return mBaseDerivation;
}

bool ComplexType::isSimple() const
{
  return false;
}

void ComplexType::setIsArray( bool isArray )
{
  mIsArray = isArray;
}

bool ComplexType::isArray() const
{
  return mIsArray;
}

void ComplexType::setAnonymous( bool anonymous )
{
  mAnonymous = anonymous;
}

bool ComplexType::isAnonymous() const
{
  return mAnonymous;
}

void ComplexType::setElements( const Element::List &elements )
{
  mElements = elements;
}

Element::List ComplexType::elements() const
{
  return mElements;
}

void ComplexType::setAttributes( const Attribute::List &attributes )
{
  mAttributes = attributes;
}

Attribute::List ComplexType::attributes() const
{
  return mAttributes;
}

void ComplexType::setAttributeGroups( const AttributeGroup::List &attributeGroups )
{
  mAttributeGroups = attributeGroups;
}

AttributeGroup::List ComplexType::attributeGroups() const
{
  return mAttributeGroups;
}

void ComplexType::addAttribute( const Attribute &attribute )
{
  mAttributes.append( attribute );
}

void ComplexType::addElement( const Element &element )
{
  mElements.append( element );
}
