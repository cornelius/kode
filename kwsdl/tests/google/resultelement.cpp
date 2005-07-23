/*
    This file is part of KDE.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
    
    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/

#include "resultelement.h"

#include <serializer.h>
#include <qstring.h>
#include <directorycategory.h>

void ResultElement::setSummary( QString* summary )
{
  mSummary = summary;
}

QString*ResultElement::summary() const
{
   return mSummary;
}

void ResultElement::setURL( QString* uRL )
{
  mURL = uRL;
}

QString*ResultElement::uRL() const
{
   return mURL;
}

void ResultElement::setSnippet( QString* snippet )
{
  mSnippet = snippet;
}

QString*ResultElement::snippet() const
{
   return mSnippet;
}

void ResultElement::setTitle( QString* title )
{
  mTitle = title;
}

QString*ResultElement::title() const
{
   return mTitle;
}

void ResultElement::setCachedSize( QString* cachedSize )
{
  mCachedSize = cachedSize;
}

QString*ResultElement::cachedSize() const
{
   return mCachedSize;
}

void ResultElement::setRelatedInformationPresent( bool* relatedInformationPresent )
{
  mRelatedInformationPresent = relatedInformationPresent;
}

bool*ResultElement::relatedInformationPresent() const
{
   return mRelatedInformationPresent;
}

void ResultElement::setHostName( QString* hostName )
{
  mHostName = hostName;
}

QString*ResultElement::hostName() const
{
   return mHostName;
}

void ResultElement::setDirectoryCategory( DirectoryCategory* directoryCategory )
{
  mDirectoryCategory = directoryCategory;
}

DirectoryCategory*ResultElement::directoryCategory() const
{
   return mDirectoryCategory;
}

void ResultElement::setDirectoryTitle( QString* directoryTitle )
{
  mDirectoryTitle = directoryTitle;
}

QString*ResultElement::directoryTitle() const
{
   return mDirectoryTitle;
}

ResultElement::ResultElement()
{
  mSummary = 0;
  mURL = 0;
  mSnippet = 0;
  mTitle = 0;
  mCachedSize = 0;
  mRelatedInformationPresent = 0;
  mHostName = 0;
  mDirectoryCategory = 0;
  mDirectoryTitle = 0;
}

ResultElement::~ResultElement()
{
  delete mSummary;
  mSummary = 0;
  delete mURL;
  mURL = 0;
  delete mSnippet;
  mSnippet = 0;
  delete mTitle;
  mTitle = 0;
  delete mCachedSize;
  mCachedSize = 0;
  delete mRelatedInformationPresent;
  mRelatedInformationPresent = 0;
  delete mHostName;
  mHostName = 0;
  delete mDirectoryCategory;
  mDirectoryCategory = 0;
  delete mDirectoryTitle;
  mDirectoryTitle = 0;
}


