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

#include "attribute.h"

namespace XSD {

class Attribute::Private
{
public:
    Private()
     : mQualified(false), mUse(false)
    {}

    QName mType;
    QString mDocumentation;
    QString mDefaultValue;
    QString mFixedValue;
    bool mQualified;
    bool mUse;
    QName mReference;
    QName mArrayType;
};

Attribute::Attribute()
  : XmlElement(), d(new Private)
{
}

Attribute::Attribute( const QString &nameSpace )
  : XmlElement( nameSpace ), d(new Private)
{
}

Attribute::Attribute( const Attribute &other )
 : XmlElement(), d(new Private)
{
  *d = *other.d;
}

Attribute::~Attribute()
{
  delete d;
}

Attribute &Attribute::operator=( const Attribute &other )
{
  if( this == &other )
    return *this;

  *d = *other.d;

  return *this;
}

void Attribute::setType( const QName &type )
{
  d->mType = type;
}

QName Attribute::type() const
{
  return d->mType;
}

void Attribute::setDocumentation( const QString &documentation )
{
  d->mDocumentation = documentation;
}

QString Attribute::documentation() const
{
  return d->mDocumentation;
}

void Attribute::setDefaultValue( const QString &defaultValue )
{
  d->mDefaultValue = defaultValue;
}

QString Attribute::defaultValue() const
{
  return d->mDefaultValue;
}

void Attribute::setFixedValue( const QString &fixedValue )
{
  d->mFixedValue = fixedValue;
}

QString Attribute::fixedValue() const
{
  return d->mFixedValue;
}

void Attribute::setIsQualified( bool isQualified )
{
  d->mQualified = isQualified;
}

bool Attribute::isQualified() const
{
  return d->mQualified;
}

void Attribute::setIsUsed( bool isUsed )
{
  d->mUse = isUsed;
}

bool Attribute::isUsed() const
{
  return d->mUse;
}

void Attribute::setReference( const QName &reference )
{
  d->mReference = reference;
}

QName Attribute::reference() const
{
  return d->mReference;
}

bool Attribute::isResolved() const
{
  return !d->mType.isEmpty();
}

void Attribute::setArrayType( const QName &arrayType )
{
  d->mArrayType = arrayType;
}

QName Attribute::arrayType() const
{
  return d->mArrayType;
}

}
