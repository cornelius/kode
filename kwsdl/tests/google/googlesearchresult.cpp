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

#include "googlesearchresult.h"

#include <serializer.h>
#include <qstring.h>
#include <resultelementarray.h>
#include <directorycategoryarray.h>

void GoogleSearchResult::setDocumentFiltering( bool* documentFiltering )
{
  mDocumentFiltering = documentFiltering;
}

bool*GoogleSearchResult::documentFiltering() const
{
   return mDocumentFiltering;
}

void GoogleSearchResult::setSearchComments( QString* searchComments )
{
  mSearchComments = searchComments;
}

QString*GoogleSearchResult::searchComments() const
{
   return mSearchComments;
}

void GoogleSearchResult::setEstimatedTotalResultsCount( int* estimatedTotalResultsCount )
{
  mEstimatedTotalResultsCount = estimatedTotalResultsCount;
}

int*GoogleSearchResult::estimatedTotalResultsCount() const
{
   return mEstimatedTotalResultsCount;
}

void GoogleSearchResult::setEstimateIsExact( bool* estimateIsExact )
{
  mEstimateIsExact = estimateIsExact;
}

bool*GoogleSearchResult::estimateIsExact() const
{
   return mEstimateIsExact;
}

void GoogleSearchResult::setResultElements( ResultElementArray* resultElements )
{
  mResultElements = resultElements;
}

ResultElementArray*GoogleSearchResult::resultElements() const
{
   return mResultElements;
}

void GoogleSearchResult::setSearchQuery( QString* searchQuery )
{
  mSearchQuery = searchQuery;
}

QString*GoogleSearchResult::searchQuery() const
{
   return mSearchQuery;
}

void GoogleSearchResult::setStartIndex( int* startIndex )
{
  mStartIndex = startIndex;
}

int*GoogleSearchResult::startIndex() const
{
   return mStartIndex;
}

void GoogleSearchResult::setEndIndex( int* endIndex )
{
  mEndIndex = endIndex;
}

int*GoogleSearchResult::endIndex() const
{
   return mEndIndex;
}

void GoogleSearchResult::setSearchTips( QString* searchTips )
{
  mSearchTips = searchTips;
}

QString*GoogleSearchResult::searchTips() const
{
   return mSearchTips;
}

void GoogleSearchResult::setDirectoryCategories( DirectoryCategoryArray* directoryCategories )
{
  mDirectoryCategories = directoryCategories;
}

DirectoryCategoryArray*GoogleSearchResult::directoryCategories() const
{
   return mDirectoryCategories;
}

void GoogleSearchResult::setSearchTime( double* searchTime )
{
  mSearchTime = searchTime;
}

double*GoogleSearchResult::searchTime() const
{
   return mSearchTime;
}

GoogleSearchResult::GoogleSearchResult()
{
  mDocumentFiltering = 0;
  mSearchComments = 0;
  mEstimatedTotalResultsCount = 0;
  mEstimateIsExact = 0;
  mResultElements = 0;
  mSearchQuery = 0;
  mStartIndex = 0;
  mEndIndex = 0;
  mSearchTips = 0;
  mDirectoryCategories = 0;
  mSearchTime = 0;
}

GoogleSearchResult::~GoogleSearchResult()
{
  delete mDocumentFiltering;
  mDocumentFiltering = 0;
  delete mSearchComments;
  mSearchComments = 0;
  delete mEstimatedTotalResultsCount;
  mEstimatedTotalResultsCount = 0;
  delete mEstimateIsExact;
  mEstimateIsExact = 0;
  delete mResultElements;
  mResultElements = 0;
  delete mSearchQuery;
  mSearchQuery = 0;
  delete mStartIndex;
  mStartIndex = 0;
  delete mEndIndex;
  mEndIndex = 0;
  delete mSearchTips;
  mSearchTips = 0;
  delete mDirectoryCategories;
  mDirectoryCategories = 0;
  delete mSearchTime;
  mSearchTime = 0;
}


