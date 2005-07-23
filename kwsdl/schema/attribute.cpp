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


Attribute::Attribute( const QString &name, int type, bool qualified,
                      const QString &defaultValue, const QString &fixedValue,
                      bool use )
  : mName( name ), mType( type ), mQualified( qualified ),
    mDefaultValue( defaultValue ), mFixedValue( fixedValue ),
    mUse( use )
{
}

Attribute::Attribute()
  : mType( 0 ), mQualified( false ), mUse( false )
{
}

QString Attribute::name() const
{
  return mName;
}

int Attribute::type() const
{
  return mType;
}

void Attribute::setTypeName( const QString &typeName )
{
  mTypeName = typeName;
}

QString Attribute::typeName() const
{
  return mTypeName;
}

QString Attribute::defaultValue() const
{
  return mDefaultValue;
}

QString Attribute::fixedValue() const
{
  return mFixedValue;
}

bool Attribute::isQualified() const
{
  return mQualified;
}

bool Attribute::isUsed() const
{
  return mUse;
}
