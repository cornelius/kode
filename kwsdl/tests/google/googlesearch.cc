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

#include "googlesearch.hh"
#include "resultelement.h"
#include "resultelementarray.h"

GoogleSearch::GoogleSearch()
  : QObject( 0, "" )
{
  connect( &mService, SIGNAL( doGetCachedPageResponse( QByteArray* ) ),
           this, SLOT( cachedPageResult( QByteArray* ) ) );
  connect( &mService, SIGNAL( doSpellingSuggestionResponse( QString* ) ),
           this, SLOT( spellingSuggestionResult( QString* ) ) );
  connect( &mService, SIGNAL( doGoogleSearchResponse( GoogleSearchResult* ) ),
           this, SLOT( googleSearchResult( GoogleSearchResult* ) ) );

  mKey = "";
}

void GoogleSearch::cachedPage( const QString &url )
{
  mService.doGetCachedPage( new QString( mKey ), new QString( url ) );
}

void GoogleSearch::spellingSuggestion( const QString &phrase )
{
  mService.doSpellingSuggestion( new QString( mKey ), new QString( phrase ) );
}

void GoogleSearch::googleSearch( const QString &query, int start, int maxResults, bool filter,
                                 const QString &restrict, bool safeSearch, const QString &lr, const QString &ie,
                                 const QString &oe )
{
  mService.doGoogleSearch( new QString( mKey ), new QString( query ), new int( start ), new int( maxResults ),
                           new bool( filter ), new QString( restrict ), new bool( safeSearch ), new QString( lr ),
                           new QString( ie ), new QString( oe ) );
}

void GoogleSearch::cachedPageResult( QByteArray *array )
{
  qDebug( "--------------- Cached Page Results ---------------------" );
  qDebug( "%s", array->data() );
  qDebug( "---------------------------------------------------------" );

  delete array;
}

void GoogleSearch::spellingSuggestionResult( QString *word )
{
  qDebug( "--------------- Spelling Suggestion ---------------------" );
  qDebug( "%s", word->latin1() );
  qDebug( "---------------------------------------------------------" );

  delete word;
}

void GoogleSearch::googleSearchResult( GoogleSearchResult *result )
{
  qDebug( "--------------------- Search Results ---------------------" );
  ResultElementArray *array = result->resultElements();
  QPtrList<ResultElement> *list = array->items();
  QPtrListIterator<ResultElement> it( *list );
  while ( it.current() != 0 ) {
    qDebug( "%s: %s", it.current()->summary()->latin1(), it.current()->uRL()->latin1() );
    ++it;
  }
  qDebug( "---------------------------------------------------------" );

  delete result;
}

#include "googlesearch.moc"
