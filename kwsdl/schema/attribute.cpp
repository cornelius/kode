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

using namespace Schema;

Attribute::Attribute()
  : mQualified( false ), mUse( false )
{
}

Attribute::Attribute( const QString &nameSpace )
  : XmlElement( nameSpace ), mQualified( false ), mUse( false )
{
}

void Attribute::setType( const QName &type )
{
  mType = type;
}

QName Attribute::type() const
{
  return mType;
}

void Attribute::setDocumentation( const QString &documentation )
{
  mDocumentation = documentation;
}

QString Attribute::documentation() const
{
  return mDocumentation;
}

void Attribute::setDefaultValue( const QString &defaultValue )
{
  mDefaultValue = defaultValue;
}

QString Attribute::defaultValue() const
{
  return mDefaultValue;
}

void Attribute::setFixedValue( const QString &fixedValue )
{
  mFixedValue = fixedValue;
}

QString Attribute::fixedValue() const
{
  return mFixedValue;
}

void Attribute::setIsQualified( bool isQualified )
{
  mQualified = isQualified;
}

bool Attribute::isQualified() const
{
  return mQualified;
}

void Attribute::setIsUsed( bool isUsed )
{
  mUse = isUsed;
}

bool Attribute::isUsed() const
{
  return mUse;
}

void Attribute::setReference( const QName &reference )
{
  mReference = reference;
}

QName Attribute::reference() const
{
  return mReference;
}

bool Attribute::isResolved() const
{
  return !mType.isEmpty();
}

void Attribute::setArrayType( const QName &arrayType )
{
  mArrayType = arrayType;
}

QName Attribute::arrayType() const
{
  return mArrayType;
}
