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

#include "element.h"

using namespace Schema;


Element::Element()
  : mType( 0 ), mMinOccurs( 1 ), mMaxOccurs( 1 ), mQualified( false ),
    mOccurrence( 0 )
{
}

Element::Element( const QString &name, int type, int minOccurs, int maxOccurs,
                  bool qualified, const QString &defaultValue, const QString &fixedValue )
  : mName( name ), mType( type ), mMinOccurs( minOccurs ), mMaxOccurs( maxOccurs ),
    mQualified( qualified ), mDefaultValue( defaultValue ), mFixedValue( fixedValue ),
    mOccurrence( 0 )
{
}

QString Element::name() const
{
  return mName;
}

void Element::setType( int type )
{
  mType = type;
}

int Element::type() const
{
  return mType;
}

void Element::setTypeName( const QString &typeName )
{
  mTypeName = typeName;
}

QString Element::typeName() const
{
  return mTypeName;
}

void Element::setDocumentation( const QString &documentation )
{
  mDocumentation = documentation;
}

QString Element::documentation() const
{
  return mDocumentation;
}

void Element::setGroupId( int group )
{
  mGroupId = group;
}

int Element::groupId() const
{
  return mGroupId;
}

void Element::setMinOccurs( int minOccurs )
{
  mMinOccurs = minOccurs;
}

int Element::minOccurs() const
{
  return mMinOccurs;
}

void Element::setMaxOccurs( int maxOccurs )
{
  mMaxOccurs = maxOccurs;
}

int Element::maxOccurs() const
{
  return mMaxOccurs;
}

QString Element::defaultValue() const
{
  return mDefaultValue;
}

QString Element::fixedValue() const
{
  return mFixedValue;
}

bool Element::isQualified() const
{
  return mQualified;
}

void Element::setOccurrence( int occurrence )
{
  mOccurrence = occurrence;
}

int Element::occurrence() const
{
  return mOccurrence;
}
