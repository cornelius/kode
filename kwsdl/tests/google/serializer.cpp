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

#include "serializer.h"

#include <kmdcodec.h>
#include <resultelementarray.h>
#include <directorycategoryarray.h>
#include <googlesearchresult.h>
#include <directorycategory.h>
#include <resultelement.h>

QString Serializer::marshalValue( const QString* value )
{
  return *value;
}

void Serializer::demarshalValue( const QString &str, QString *value )
{
  *value = str;
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const QString* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:string" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, QString* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const bool* value )
{
  return (*value ? "true" : "false");
}

void Serializer::demarshalValue( const QString &str, bool *value )
{
  *value = (str.lower() == "true" ? true : false);
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const bool* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:boolean" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, bool* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const float* value )
{
  return QString::number( *value );
}

void Serializer::demarshalValue( const QString &str, float *value )
{
  *value = str.toFloat();
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const float* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:TODO" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, float* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const int* value )
{
  return QString::number( *value );
}

void Serializer::demarshalValue( const QString &str, int *value )
{
  *value = str.toInt();
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const int* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:int" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, int* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const unsigned int* value )
{
  return QString::number( *value );
}

void Serializer::demarshalValue( const QString &str, unsigned int *value )
{
  *value = str.toUInt();
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const unsigned int* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:unsignedByte" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, unsigned int* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const double* value )
{
  return QString::number( *value );
}

void Serializer::demarshalValue( const QString &str, double *value )
{
  *value = str.toDouble();
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const double* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:double" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, double* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const char* value )
{
  return QString( QChar( *value ) );
}

void Serializer::demarshalValue( const QString &str, char *value )
{
  *value = str[ 0 ].latin1();
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const char* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:byte" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, char* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const unsigned char* value )
{
  return QString( QChar( *value ) );
}

void Serializer::demarshalValue( const QString &str, unsigned char *value )
{
  *value = str[ 0 ].latin1();
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const unsigned char* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:unsignedByte" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, unsigned char* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const short* value )
{
  return QString::number( *value );
}

void Serializer::demarshalValue( const QString &str, short *value )
{
  *value = str.toShort();
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const short* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:short" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, short* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const QByteArray* value )
{
  return QString::fromUtf8( KCodecs::base64Encode( *value ) );
}

void Serializer::demarshalValue( const QString &str, QByteArray *value )
{
  *value = KCodecs::base64Decode( str.utf8() );
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const QByteArray* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:base64Binary" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, QByteArray* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const QDateTime* value )
{
  return value->toString( Qt::ISODate );
}

void Serializer::demarshalValue( const QString &str, QDateTime *value )
{
  *value = QDateTime::fromString( str, Qt::ISODate );
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const QDateTime* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:dateTime" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, QDateTime* value )
{
  Serializer::demarshalValue( element.text(), value );
}

QString Serializer::marshalValue( const QDate* value )
{
  return value->toString( Qt::ISODate );
}

void Serializer::demarshalValue( const QString &str, QDate *value )
{
  *value = QDate::fromString( str, Qt::ISODate );
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const QDate* value )
{
  QDomElement element = doc.createElement( name );
  element.setAttribute( "xsi:type", "xsd:date" );
  element.appendChild( doc.createTextNode( Serializer::marshalValue( value ) ) );
  parent.appendChild( element );
}

void Serializer::demarshal( const QDomElement &element, QDate* value )
{
  Serializer::demarshalValue( element.text(), value );
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const ResultElementArray* value )
{
  QDomElement root = doc.createElement( name );
  root.setAttribute( "xsi:type", "ns1:ResultElementArray" );
  parent.appendChild( root );
  if ( value->items() ) {
    const QPtrList<ResultElement>* list = value->items();
  
    QDomElement element = doc.createElement( name );
    element.setAttribute( "xmlns:ns1", "http://schemas.xmlsoap.org/soap/encoding/" );
    element.setAttribute( "xsi:type", "ns1:Array" );
    element.setAttribute( "ns1:arrayType", "ns1:ResultElement[" + QString::number( list->count() ) + "]" );
    parent.appendChild( element );
  
    QPtrListIterator<ResultElement> it( *list );
    while ( it.current() != 0 ) {
      Serializer::marshal( doc, element, "item", it.current() );
      ++it;
    }
  }
}

void Serializer::demarshal( const QDomElement &parent, ResultElementArray* value )
{
  QPtrList<ResultElement>* itemsList = new QPtrList<ResultElement>();
  itemsList->setAutoDelete( true );
  QDomNode node = parent.firstChild();
  while ( !node.isNull() ) {
    QDomElement element = node.toElement();
    if ( !element.isNull() ) {
  if ( element.tagName() == "item" ) {
    ResultElement *item = new ResultElement;
    Serializer::demarshal( element, item );
    itemsList->append( item );
  }
  }
  node = node.nextSibling();
  }
  
  value->setItems( itemsList );
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const DirectoryCategoryArray* value )
{
  QDomElement root = doc.createElement( name );
  root.setAttribute( "xsi:type", "ns1:DirectoryCategoryArray" );
  parent.appendChild( root );
  if ( value->items() ) {
    const QPtrList<DirectoryCategory>* list = value->items();
  
    QDomElement element = doc.createElement( name );
    element.setAttribute( "xmlns:ns1", "http://schemas.xmlsoap.org/soap/encoding/" );
    element.setAttribute( "xsi:type", "ns1:Array" );
    element.setAttribute( "ns1:arrayType", "ns1:DirectoryCategory[" + QString::number( list->count() ) + "]" );
    parent.appendChild( element );
  
    QPtrListIterator<DirectoryCategory> it( *list );
    while ( it.current() != 0 ) {
      Serializer::marshal( doc, element, "item", it.current() );
      ++it;
    }
  }
}

void Serializer::demarshal( const QDomElement &parent, DirectoryCategoryArray* value )
{
  QPtrList<DirectoryCategory>* itemsList = new QPtrList<DirectoryCategory>();
  itemsList->setAutoDelete( true );
  QDomNode node = parent.firstChild();
  while ( !node.isNull() ) {
    QDomElement element = node.toElement();
    if ( !element.isNull() ) {
  if ( element.tagName() == "item" ) {
    DirectoryCategory *item = new DirectoryCategory;
    Serializer::demarshal( element, item );
    itemsList->append( item );
  }
  }
  node = node.nextSibling();
  }
  
  value->setItems( itemsList );
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const GoogleSearchResult* value )
{
  QDomElement root = doc.createElement( name );
  root.setAttribute( "xsi:type", "ns1:GoogleSearchResult" );
  parent.appendChild( root );
  if ( value->documentFiltering() ) {
    Serializer::marshal( doc, root, "documentFiltering", value->documentFiltering() );
  }
  if ( value->searchComments() ) {
    Serializer::marshal( doc, root, "searchComments", value->searchComments() );
  }
  if ( value->estimatedTotalResultsCount() ) {
    Serializer::marshal( doc, root, "estimatedTotalResultsCount", value->estimatedTotalResultsCount() );
  }
  if ( value->estimateIsExact() ) {
    Serializer::marshal( doc, root, "estimateIsExact", value->estimateIsExact() );
  }
  if ( value->resultElements() ) {
    Serializer::marshal( doc, root, "resultElements", value->resultElements() );
  }
  if ( value->searchQuery() ) {
    Serializer::marshal( doc, root, "searchQuery", value->searchQuery() );
  }
  if ( value->startIndex() ) {
    Serializer::marshal( doc, root, "startIndex", value->startIndex() );
  }
  if ( value->endIndex() ) {
    Serializer::marshal( doc, root, "endIndex", value->endIndex() );
  }
  if ( value->searchTips() ) {
    Serializer::marshal( doc, root, "searchTips", value->searchTips() );
  }
  if ( value->directoryCategories() ) {
    Serializer::marshal( doc, root, "directoryCategories", value->directoryCategories() );
  }
  if ( value->searchTime() ) {
    Serializer::marshal( doc, root, "searchTime", value->searchTime() );
  }
}

void Serializer::demarshal( const QDomElement &parent, GoogleSearchResult* value )
{
  QDomNode node = parent.firstChild();
  while ( !node.isNull() ) {
    QDomElement element = node.toElement();
    if ( !element.isNull() ) {
      if ( element.tagName() == "documentFiltering" ) {
        bool* item = new bool;
        Serializer::demarshal( element, item );
        value->setDocumentFiltering( item );
      }
      if ( element.tagName() == "searchComments" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setSearchComments( item );
      }
      if ( element.tagName() == "estimatedTotalResultsCount" ) {
        int* item = new int;
        Serializer::demarshal( element, item );
        value->setEstimatedTotalResultsCount( item );
      }
      if ( element.tagName() == "estimateIsExact" ) {
        bool* item = new bool;
        Serializer::demarshal( element, item );
        value->setEstimateIsExact( item );
      }
      if ( element.tagName() == "resultElements" ) {
        ResultElementArray* item = new ResultElementArray;
        Serializer::demarshal( element, item );
        value->setResultElements( item );
      }
      if ( element.tagName() == "searchQuery" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setSearchQuery( item );
      }
      if ( element.tagName() == "startIndex" ) {
        int* item = new int;
        Serializer::demarshal( element, item );
        value->setStartIndex( item );
      }
      if ( element.tagName() == "endIndex" ) {
        int* item = new int;
        Serializer::demarshal( element, item );
        value->setEndIndex( item );
      }
      if ( element.tagName() == "searchTips" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setSearchTips( item );
      }
      if ( element.tagName() == "directoryCategories" ) {
        DirectoryCategoryArray* item = new DirectoryCategoryArray;
        Serializer::demarshal( element, item );
        value->setDirectoryCategories( item );
      }
      if ( element.tagName() == "searchTime" ) {
        double* item = new double;
        Serializer::demarshal( element, item );
        value->setSearchTime( item );
      }
    }
    node = node.nextSibling();
  }
  
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const DirectoryCategory* value )
{
  QDomElement root = doc.createElement( name );
  root.setAttribute( "xsi:type", "ns1:DirectoryCategory" );
  parent.appendChild( root );
  if ( value->fullViewableName() ) {
    Serializer::marshal( doc, root, "fullViewableName", value->fullViewableName() );
  }
  if ( value->specialEncoding() ) {
    Serializer::marshal( doc, root, "specialEncoding", value->specialEncoding() );
  }
}

void Serializer::demarshal( const QDomElement &parent, DirectoryCategory* value )
{
  QDomNode node = parent.firstChild();
  while ( !node.isNull() ) {
    QDomElement element = node.toElement();
    if ( !element.isNull() ) {
      if ( element.tagName() == "fullViewableName" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setFullViewableName( item );
      }
      if ( element.tagName() == "specialEncoding" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setSpecialEncoding( item );
      }
    }
    node = node.nextSibling();
  }
  
}

void Serializer::marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const ResultElement* value )
{
  QDomElement root = doc.createElement( name );
  root.setAttribute( "xsi:type", "ns1:ResultElement" );
  parent.appendChild( root );
  if ( value->summary() ) {
    Serializer::marshal( doc, root, "summary", value->summary() );
  }
  if ( value->uRL() ) {
    Serializer::marshal( doc, root, "URL", value->uRL() );
  }
  if ( value->snippet() ) {
    Serializer::marshal( doc, root, "snippet", value->snippet() );
  }
  if ( value->title() ) {
    Serializer::marshal( doc, root, "title", value->title() );
  }
  if ( value->cachedSize() ) {
    Serializer::marshal( doc, root, "cachedSize", value->cachedSize() );
  }
  if ( value->relatedInformationPresent() ) {
    Serializer::marshal( doc, root, "relatedInformationPresent", value->relatedInformationPresent() );
  }
  if ( value->hostName() ) {
    Serializer::marshal( doc, root, "hostName", value->hostName() );
  }
  if ( value->directoryCategory() ) {
    Serializer::marshal( doc, root, "directoryCategory", value->directoryCategory() );
  }
  if ( value->directoryTitle() ) {
    Serializer::marshal( doc, root, "directoryTitle", value->directoryTitle() );
  }
}

void Serializer::demarshal( const QDomElement &parent, ResultElement* value )
{
  QDomNode node = parent.firstChild();
  while ( !node.isNull() ) {
    QDomElement element = node.toElement();
    if ( !element.isNull() ) {
      if ( element.tagName() == "summary" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setSummary( item );
      }
      if ( element.tagName() == "URL" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setURL( item );
      }
      if ( element.tagName() == "snippet" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setSnippet( item );
      }
      if ( element.tagName() == "title" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setTitle( item );
      }
      if ( element.tagName() == "cachedSize" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setCachedSize( item );
      }
      if ( element.tagName() == "relatedInformationPresent" ) {
        bool* item = new bool;
        Serializer::demarshal( element, item );
        value->setRelatedInformationPresent( item );
      }
      if ( element.tagName() == "hostName" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setHostName( item );
      }
      if ( element.tagName() == "directoryCategory" ) {
        DirectoryCategory* item = new DirectoryCategory;
        Serializer::demarshal( element, item );
        value->setDirectoryCategory( item );
      }
      if ( element.tagName() == "directoryTitle" ) {
        QString* item = new QString;
        Serializer::demarshal( element, item );
        value->setDirectoryTitle( item );
      }
    }
    node = node.nextSibling();
  }
  
}


