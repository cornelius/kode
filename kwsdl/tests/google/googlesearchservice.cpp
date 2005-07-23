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

#include "googlesearchservice.h"

#include <serializer.h>

void GoogleSearchService::doGetCachedPage( QString* key, QString* url )
{
  QDomDocument doc( "kwsdl" );
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );
  QDomElement env = doc.createElement( "SOAP-ENV:Envelope" );
  env.setAttribute( "xmlns:SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/" );
  env.setAttribute( "xmlns:xsi", "http://www.w3.org/1999/XMLSchema-instance" );
  env.setAttribute( "xmlns:xsd", "http://www.w3.org/1999/XMLSchema" );
  doc.appendChild( env );
  QDomElement body = doc.createElement( "SOAP-ENV:Body" );
  env.appendChild( body );
  QDomElement method = doc.createElement( "ns1:doGetCachedPage" );
  method.setAttribute( "xmlns:ns1", "urn:GoogleSearch" );
  method.setAttribute( "SOAP-ENV:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/" );
  body.appendChild( method );
  
  Serializer::marshal( doc, method, "key", key );
  delete key;
  Serializer::marshal( doc, method, "url", url );
  delete url;
  qDebug( "%s", doc.toString().latin1() );
  mDoGetCachedPageTransport->query( doc.toString() );
}

void GoogleSearchService::doGetCachedPageResponseSlot( const QString &xml )
{
  QDomDocument doc;
  QString errorMsg;
  int column, row;
  
  qDebug( "%s", xml.latin1() );
  
  if ( !doc.setContent( xml, true, &errorMsg, &row, &column ) ) {
    qDebug( "Unable to parse xml: %s (%d:%d)", errorMsg.latin1(), row, column );
    return;
  }
  
  QByteArray* value = new QByteArray;
  QDomElement envelope = doc.documentElement();
  QDomElement body = envelope.firstChild().toElement();
  QDomElement method = body.firstChild().toElement();
  Serializer::demarshal( method.firstChild().toElement(), value );
  
  emit doGetCachedPageResponse( value );
}

void GoogleSearchService::doSpellingSuggestion( QString* key, QString* phrase )
{
  QDomDocument doc( "kwsdl" );
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );
  QDomElement env = doc.createElement( "SOAP-ENV:Envelope" );
  env.setAttribute( "xmlns:SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/" );
  env.setAttribute( "xmlns:xsi", "http://www.w3.org/1999/XMLSchema-instance" );
  env.setAttribute( "xmlns:xsd", "http://www.w3.org/1999/XMLSchema" );
  doc.appendChild( env );
  QDomElement body = doc.createElement( "SOAP-ENV:Body" );
  env.appendChild( body );
  QDomElement method = doc.createElement( "ns1:doSpellingSuggestion" );
  method.setAttribute( "xmlns:ns1", "urn:GoogleSearch" );
  method.setAttribute( "SOAP-ENV:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/" );
  body.appendChild( method );
  
  Serializer::marshal( doc, method, "key", key );
  delete key;
  Serializer::marshal( doc, method, "phrase", phrase );
  delete phrase;
  qDebug( "%s", doc.toString().latin1() );
  mDoSpellingSuggestionTransport->query( doc.toString() );
}

void GoogleSearchService::doSpellingSuggestionResponseSlot( const QString &xml )
{
  QDomDocument doc;
  QString errorMsg;
  int column, row;
  
  qDebug( "%s", xml.latin1() );
  
  if ( !doc.setContent( xml, true, &errorMsg, &row, &column ) ) {
    qDebug( "Unable to parse xml: %s (%d:%d)", errorMsg.latin1(), row, column );
    return;
  }
  
  QString* value = new QString;
  QDomElement envelope = doc.documentElement();
  QDomElement body = envelope.firstChild().toElement();
  QDomElement method = body.firstChild().toElement();
  Serializer::demarshal( method.firstChild().toElement(), value );
  
  emit doSpellingSuggestionResponse( value );
}

void GoogleSearchService::doGoogleSearch( QString* key, QString* q, int* start, int* maxResults, bool* filter, QString* restrict, bool* safeSearch, QString* lr, QString* ie, QString* oe )
{
  QDomDocument doc( "kwsdl" );
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );
  QDomElement env = doc.createElement( "SOAP-ENV:Envelope" );
  env.setAttribute( "xmlns:SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/" );
  env.setAttribute( "xmlns:xsi", "http://www.w3.org/1999/XMLSchema-instance" );
  env.setAttribute( "xmlns:xsd", "http://www.w3.org/1999/XMLSchema" );
  doc.appendChild( env );
  QDomElement body = doc.createElement( "SOAP-ENV:Body" );
  env.appendChild( body );
  QDomElement method = doc.createElement( "ns1:doGoogleSearch" );
  method.setAttribute( "xmlns:ns1", "urn:GoogleSearch" );
  method.setAttribute( "SOAP-ENV:encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/" );
  body.appendChild( method );
  
  Serializer::marshal( doc, method, "key", key );
  delete key;
  Serializer::marshal( doc, method, "q", q );
  delete q;
  Serializer::marshal( doc, method, "start", start );
  delete start;
  Serializer::marshal( doc, method, "maxResults", maxResults );
  delete maxResults;
  Serializer::marshal( doc, method, "filter", filter );
  delete filter;
  Serializer::marshal( doc, method, "restrict", restrict );
  delete restrict;
  Serializer::marshal( doc, method, "safeSearch", safeSearch );
  delete safeSearch;
  Serializer::marshal( doc, method, "lr", lr );
  delete lr;
  Serializer::marshal( doc, method, "ie", ie );
  delete ie;
  Serializer::marshal( doc, method, "oe", oe );
  delete oe;
  qDebug( "%s", doc.toString().latin1() );
  mDoGoogleSearchTransport->query( doc.toString() );
}

void GoogleSearchService::doGoogleSearchResponseSlot( const QString &xml )
{
  QDomDocument doc;
  QString errorMsg;
  int column, row;
  
  qDebug( "%s", xml.latin1() );
  
  if ( !doc.setContent( xml, true, &errorMsg, &row, &column ) ) {
    qDebug( "Unable to parse xml: %s (%d:%d)", errorMsg.latin1(), row, column );
    return;
  }
  
  GoogleSearchResult* value = new GoogleSearchResult;
  QDomElement envelope = doc.documentElement();
  QDomElement body = envelope.firstChild().toElement();
  QDomElement method = body.firstChild().toElement();
  Serializer::demarshal( method.firstChild().toElement(), value );
  
  emit doGoogleSearchResponse( value );
}

GoogleSearchService::GoogleSearchService()
{
  mDoGetCachedPageTransport = new Transport( "http://api.google.com/search/beta2" );
  connect( mDoGetCachedPageTransport, SIGNAL( result( const QString& ) ),
    this, SLOT( doGetCachedPageResponseSlot( const QString& ) ) );
  mDoSpellingSuggestionTransport = new Transport( "http://api.google.com/search/beta2" );
  connect( mDoSpellingSuggestionTransport, SIGNAL( result( const QString& ) ),
    this, SLOT( doSpellingSuggestionResponseSlot( const QString& ) ) );
  mDoGoogleSearchTransport = new Transport( "http://api.google.com/search/beta2" );
  connect( mDoGoogleSearchTransport, SIGNAL( result( const QString& ) ),
    this, SLOT( doGoogleSearchResponseSlot( const QString& ) ) );
}

GoogleSearchService::~GoogleSearchService()
{
  delete mDoGetCachedPageTransport;
  mDoGetCachedPageTransport = 0;
  delete mDoSpellingSuggestionTransport;
  mDoSpellingSuggestionTransport = 0;
  delete mDoGoogleSearchTransport;
  mDoGoogleSearchTransport = 0;
}


#include "googlesearchservice.moc"

