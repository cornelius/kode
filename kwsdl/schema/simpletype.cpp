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

#include "simpletype.h"

using namespace Schema;

SimpleType::SimpleType()
  : mRestriction( false ), mFacetId( NONE ), mAnonymous( false ),
    mSubType( TypeRestriction )
{
}

SimpleType::SimpleType( const QString &nameSpace )
  : XSDType( nameSpace ), mRestriction( false ), mFacetId( NONE ),
    mAnonymous( false ), mSubType( TypeRestriction )
{
}

SimpleType::~SimpleType()
{
}

void SimpleType::setDocumentation( const QString &documentation )
{
  mDocumentation = documentation;
}

QString SimpleType::documentation() const
{
  return mDocumentation;
}

void SimpleType::setBaseTypeName( const QName &baseTypeName )
{
  mBaseTypeName = baseTypeName;
  mRestriction = true;
}

QName SimpleType::baseTypeName() const
{
  return mBaseTypeName;
}

void SimpleType::setSubType( SubType subType )
{
  mSubType = subType;
}

SimpleType::SubType SimpleType::subType() const
{
  return mSubType;
}

void SimpleType::setListTypeName( const QName &name )
{
  mListTypeName = name;
}

QName SimpleType::listTypeName() const
{
  return mListTypeName;
}

void SimpleType::setAnonymous( bool anonymous )
{
  mAnonymous = anonymous;
}

bool SimpleType::isAnonymous() const
{
  return mAnonymous;
}

bool SimpleType::isValidFacet( const QString &facet )
{
  if ( mBaseTypeName.isEmpty() ) {
    qDebug( "isValidFacet:Unknown base type" );
    return false;
  }

  if ( facet == "length" )
    mFacetId |= LENGTH;
  else if ( facet == "minLength" )
    mFacetId |= MINLEN;
  else if ( facet == "maxLength" )
    mFacetId |= MAXLEN;
  else if ( facet == "enumeration" )
    mFacetId |= ENUM;
  else if ( facet == "whiteSpace" )
    mFacetId |= WSP;
  else if ( facet == "pattern" )
    mFacetId |= PATTERN;
  else if ( facet == "maxInclusive" )
    mFacetId |= MAXINC;
  else if ( facet == "maxExclusive" )
    mFacetId |= MAXEX;
  else if ( facet == "minInclusive" )
    mFacetId |= MININC;
  else if ( facet == "minExclusive" )
    mFacetId |= MINEX;
  else if ( facet == "totalDigits" )
    mFacetId |= TOT;
  else if ( facet == "fractionDigits" )
    mFacetId |= FRAC;
  else {
    mFacetId = NONE;
    return false;
  }

  return true;
}

void SimpleType::setFacetValue( const QString &value )
{
  int number = -1;

  if ( mFacetId & ENUM ) {
    mEnums.append( value );
  } else if ( mFacetId & PATTERN ) {
    mFacetValue.pattern = value;
  } else if ( mFacetId & WSP ) {
    if ( value == "preserve" )
      mFacetValue.wsp = PRESERVE;
    else if ( value == "collapse" )
      mFacetValue.wsp = COLLAPSE;
    else if ( value == "replace" )
      mFacetValue.wsp = REPLACE;
    else {
      qDebug( "Invalid facet value for whitespace" );
      return;
    }
  } else {
    number = value.toInt();
  }

  if ( mFacetId & MAXEX ) {
    mFacetValue.valRange.maxex = number;
  } else if ( mFacetId & MAXINC ) {
    mFacetValue.valRange.maxinc = number;
  } else if ( mFacetId & MININC ) {
    mFacetValue.valRange.mininc = number;
  } else if ( mFacetId & MINEX ) {
    mFacetValue.valRange.minex = number;
  } else if ( mFacetId & MAXEX ) {
    mFacetValue.valRange.maxex = number;
  } else if ( mFacetId & LENGTH ) {
    mFacetValue.length = number;
  } else if ( mFacetId & MINLEN ) {
    mFacetValue.lenRange.minlen = number;
  } else if ( mFacetId & MAXLEN ) {
    mFacetValue.lenRange.maxlen = number;
  } else if ( mFacetId & TOT ) {
    mFacetValue.tot = number;
  } else if ( mFacetId & FRAC ) {
    mFacetValue.frac = number;
  }
}

int SimpleType::facetType() const
{
  return mFacetId;
}

int SimpleType::facetLength() const
{
  return mFacetValue.length;
}

int SimpleType::facetMinimumLength() const
{
  return mFacetValue.lenRange.minlen;
}

int SimpleType::facetMaximumLength() const
{
  return mFacetValue.lenRange.maxlen;
}

QStringList SimpleType::facetEnums() const
{
  return mEnums;
}

SimpleType::WhiteSpaceType SimpleType::facetWhiteSpace() const
{
  return mFacetValue.wsp;
}

int SimpleType::facetMinimumInclusive() const
{
  return mFacetValue.valRange.mininc;
}

int SimpleType::facetMaximumInclusive() const
{
  return mFacetValue.valRange.maxinc;
}

int SimpleType::facetMinimumExclusive() const
{
  return mFacetValue.valRange.minex;
}

int SimpleType::facetMaximumExclusive() const
{
  return mFacetValue.valRange.maxex;
}

int SimpleType::facetTotalDigits() const
{
  return mFacetValue.tot;
}

int SimpleType::facetFractionDigits() const
{
  return mFacetValue.frac;
}

QString SimpleType::facetPattern() const
{
  return mFacetValue.pattern;
}
