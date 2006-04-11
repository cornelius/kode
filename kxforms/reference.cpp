/*
    This file is part of KXForms.

    Copyright (c) 2005,2006 Cornelius Schumacher <schumacher@kde.org>

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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "reference.h"

#include <kdebug.h>

#include <QStringList>
#include <QMap>

using namespace KXForms;

Reference::Segment::Segment()
  : mCount( 1 ), mIsAttribute( false )
{
}

Reference::Segment::Segment( const QString &str )
{
  fromString( str );
}

Reference::Segment::Segment( const QString &name, int count )
{
  mName = name;
  mCount = count;
  mIsAttribute = false;
}

void Reference::Segment::setName( const QString &name )
{
  if ( name.startsWith( "@" ) ) {
    mName = name.mid( 1 );
    mIsAttribute = true;
  } else {
    mName = name;
    mIsAttribute = false;
  }
}

void Reference::Segment::fromString( const QString &str )
{
  int pos1 = str.indexOf( "[" );
  int pos2 = str.indexOf( "]" );

  int startPos;
  if ( str.startsWith( "@" ) ) {
    mIsAttribute = true;
    startPos = 1;
  } else {
    mIsAttribute = false;
    startPos = 0;
  }

  if ( pos1 >= 0 ) mName = str.mid( startPos, pos1 - startPos );
  else mName = str.mid( startPos );

  if ( pos1 >= 0 && pos2 > pos1 ) {
    QString count = str.mid( pos1 + 1, pos2 - pos1 - 1 );
    bool ok;
    mCount = count.toInt( &ok );
    if ( !ok ) kError() << "Illegal count in reference '" << str << "'" << endl;
  } else {
    mCount = 1;
  }
}

QString Reference::Segment::toString() const
{
  QString str;
  if ( mIsAttribute ) str += "@";
  str += mName;
  if ( mCount > 0 ) str += "[" + QString::number( mCount ) + "]";

  return str;
}

bool Reference::Segment::operator==( const Segment &s ) const
{
  return mName == s.mName && mCount == s.mCount &&
    mIsAttribute == s.mIsAttribute;
}

bool Reference::Segment::operator!=( const Segment &s ) const
{
  return !( *this == s );
}

bool Reference::Segment::isEmpty() const
{
  return mName.isEmpty();
}


Reference::Reference()
  : mValid( true )
{
}

Reference::Reference( const QString &str )
{
  fromString( str );
}

Reference::Reference( const char *str )
{
  fromString( str );
}

Reference &Reference::append( const Reference::Segment &segment )
{
  mSegments.append( segment );

  return *this;
}
 
Reference &Reference::append( const Reference &ref )
{
  Segment::List segments = ref.segments();
  Segment::List::ConstIterator it;
  for( it = segments.begin(); it != segments.end(); ++it ) {
    mSegments.append( *it );
  }
  
  return *this;
}

Reference Reference::operator+( const Reference &ref ) const
{
  Reference r = *this;
  return r.append( ref );
}

Reference Reference::operator+( const Reference::Segment &ref ) const
{
  Reference r = *this;
  return r.append( ref );
}

Reference::Segment::List Reference::segments() const
{
  return mSegments;
}

Reference::Segment &Reference::lastSegment()
{
  return mSegments.last();
}

const Reference::Segment &Reference::lastSegment() const
{
  return mSegments.last();
}

bool Reference::isAbsolute() const
{
  return mAbsolute;
}

bool Reference::isRelative() const{
  return !mAbsolute;
}

void Reference::fromString( const QString &str )
{
  mAbsolute = str.startsWith( "/" );

  QStringList s = str.split( "/" );
  QStringList::ConstIterator it;
  for( it = s.begin(); it != s.end(); ++it ) {
    mSegments.append( Segment( *it ) );
  }
}

QString Reference::toString() const
{
  QString str;
  Segment::List::ConstIterator it;
  for( it = mSegments.begin(); it != mSegments.end(); ++it ) {
    if ( !str.isEmpty() ) str += "/";
    str += (*it).toString();
  }
  if ( mAbsolute ) str.prepend( "/" );
  return str;
}

bool Reference::isValid() const
{
  return mValid;
}

bool Reference::isEmpty() const
{
  return mSegments.isEmpty();
}

bool Reference::operator==( const Reference &ref ) const
{
  if ( mAbsolute != ref.mAbsolute ) return false;

  Segment::List::ConstIterator it = mSegments.begin();
  Segment::List::ConstIterator it2 = ref.mSegments.begin();
  while( it != mSegments.end() && it2 != ref.mSegments.end() ) {
    if ( *it != *it2 ) return false;
    ++it;
    ++it2;
  }
  if ( it != mSegments.end() || it2 != ref.mSegments.end() ) return false;

  return true;
}

bool Reference::operator!=( const Reference &ref ) const
{
  return !( *this == ref );
}

Reference &Reference::operator=( const QString &str )
{
  fromString( str );
  return *this;
}

Reference &Reference::operator=( const char *str )
{
  fromString( str );
  return *this;
}

QDomElement Reference::apply( const QDomDocument &doc ) const
{
  QDomElement result;

  Reference::Segment::List::ConstIterator it;
  for( it = mSegments.begin(); it != mSegments.end(); ++it ) {
    Reference::Segment segment = *it;
    if ( result.isNull() ) {
      kDebug() << "ROOT" << endl;
      if ( doc.documentElement().tagName() == segment.name() ) {
        result = doc.documentElement();
        continue;
      } else {
        kError() << "Document element '" << doc.documentElement().tagName() <<
           "' isn't '" << segment.name() << "'" << endl;
        return QDomElement();
      }
    }

    QMap<QString, int> counts;
    QDomNode n;
    for( n = result.firstChild(); !n.isNull(); n = n.nextSibling() ) {
      QDomElement e = n.toElement();
      int count = 1;
      QMap<QString, int>::ConstIterator itCount = counts.find( e.tagName() );
      if ( itCount != counts.end() ) count = itCount.value();
      if ( e.tagName() == segment.name() && count == segment.count() ) {
        result = e;
        break;
      }
      counts.insert( e.tagName(), ++count );
    }
    if ( n.isNull() ) {
      kError() << "Reference::apply(): Unable to find element '" <<
        segment.toString() << "'" << endl;
      return QDomElement();
    }
  }
  
  return result;
}

QDomElement Reference::applyElement( const QDomElement &context ) const
{
  kDebug() << "Reference::applyElement() " << toString() << " Context: "
    << context.tagName() << endl;

  QDomElement result = context;

  if ( mSegments.count() == 1 && lastSegment().name() == "." ) return result;

  Reference::Segment::List::ConstIterator it;
  for( it = mSegments.begin(); it != mSegments.end(); ++it ) {
    Reference::Segment segment = *it;

    kDebug() << "  Segment: " << segment.toString() << " Count: " <<
      segment.count() << endl;

    QMap<QString, int> counts;
    QDomNode n;
    for( n = result.firstChild(); !n.isNull(); n = n.nextSibling() ) {
      QDomElement e = n.toElement();
      
      kDebug() << "  E: " << e.tagName() << endl;
      
      int count = 1;
      QMap<QString, int>::ConstIterator itCount = counts.find( e.tagName() );
      if ( itCount != counts.end() ) count = itCount.value();
      
      kDebug() << "  COUNT: " << count << endl;

      if ( e.tagName() == segment.name() && count == segment.count() ) {
        result = e;
        break;
      }
      counts.insert( e.tagName(), ++count );
    }
    if ( n.isNull() ) {
      kError() << "Reference::apply(): Unable to find element '" <<
        segment.toString() << "'" << endl;
      return QDomElement();
    }
  }
  
  return result;
}

QDomElement Reference::applyAttributeContext( const QDomElement &context ) const
{
  if ( mSegments.count() == 1 ) {
    return context;
  } else {
    Reference r;
    Segment::List::ConstIterator it = mSegments.begin();
    do {
      r.append( *it );
      ++it;
      Segment::List::ConstIterator it2 = it;
      if ( ++it2 == mSegments.end() ) break;
    } while( it != mSegments.end() );
    return r.applyElement( context );
  }
}

QString Reference::applyString( const QDomElement &context ) const
{
  Reference::Segment s = lastSegment();

  QString txt;
  if ( s.isAttribute() ) {
    QDomElement targetElement = applyAttributeContext( context );
    txt = targetElement.attribute( s.name() );
  } else {
    txt = applyElement( context ).text();
  }

  return txt;
}
