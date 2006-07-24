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

namespace XSD {

class ComplexType::Private
{
public:
    Private()
      : mAnonymous(false), mIsArray(false)
    {}

    QString mDocumentation;

    Element::List mElements;
    Attribute::List mAttributes;
    AttributeGroup::List mAttributeGroups;

    bool mAnonymous;
    bool mIsArray;

    Derivation mBaseDerivation;
    QName mBaseTypeName;
};

ComplexType::ComplexType( const QString &nameSpace )
  : XSDType( nameSpace ), d(new Private)
{
}

ComplexType::ComplexType()
  : XSDType(), d(new Private)
{
}

ComplexType::ComplexType( const ComplexType &other )
  : XSDType( other ), d(new Private)
{
  *d = *other.d;
}

ComplexType::~ComplexType()
{
  delete d;
}

ComplexType &ComplexType::operator=( const ComplexType &other )
{
  if ( this == &other )
    return *this;

  *d = *other.d;

  return *this;
}

void ComplexType::setDocumentation( const QString &documentation )
{
  d->mDocumentation = documentation;
}

QString ComplexType::documentation() const
{
  return d->mDocumentation;
}

void ComplexType::setBaseTypeName( const QName &baseTypeName )
{
  d->mBaseTypeName = baseTypeName;
}

QName ComplexType::baseTypeName() const
{
  return d->mBaseTypeName;
}

void ComplexType::setBaseDerivation( Derivation derivation )
{
  d->mBaseDerivation = derivation;
}

ComplexType::Derivation ComplexType::baseDerivation() const
{
  return d->mBaseDerivation;
}

bool ComplexType::isSimple() const
{
  return false;
}

void ComplexType::setIsArray( bool isArray )
{
  d->mIsArray = isArray;
}

bool ComplexType::isArray() const
{
  return d->mIsArray;
}

void ComplexType::setAnonymous( bool anonymous )
{
  d->mAnonymous = anonymous;
}

bool ComplexType::isAnonymous() const
{
  return d->mAnonymous;
}

void ComplexType::setElements( const Element::List &elements )
{
  d->mElements = elements;
}

Element::List ComplexType::elements() const
{
  return d->mElements;
}

void ComplexType::setAttributes( const Attribute::List &attributes )
{
  d->mAttributes = attributes;
}

Attribute::List ComplexType::attributes() const
{
  return d->mAttributes;
}

void ComplexType::setAttributeGroups( const AttributeGroup::List &attributeGroups )
{
  d->mAttributeGroups = attributeGroups;
}

AttributeGroup::List ComplexType::attributeGroups() const
{
  return d->mAttributeGroups;
}

void ComplexType::addAttribute( const Attribute &attribute )
{
  d->mAttributes.append( attribute );
}

void ComplexType::addElement( const Element &element )
{
  d->mElements.append( element );
}

}
