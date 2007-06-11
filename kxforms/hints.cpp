/*
    This file is part of KXForms.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "hints.h"

#include <common/qname.h>

#include <kdebug.h>
#include <QDomDocument>
#include <QDomElement>
#include <QTextDocument>

using namespace KXForms;

Hint::Hint()
{
}

Hint::Hint( const Reference &ref )
  : mRef( ref )
{
  if( !mRef.isAbsolute() && !mRef.toString().contains( '@' ) )
    mRef.fromString( '/' + mRef.toString() );
}

bool Hint::isValid() const
{
  return !mRef.isEmpty();
}

void Hint::setRef( const Reference &ref )
{
  mRef = ref;

  if( !mRef.isAbsolute() && !mRef.toString().contains( '@' ) )
    mRef.fromString( '/' + mRef.toString() );
}

Reference Hint::ref() const
{
  return mRef;
}

void Hint::setValue( Type key, const QString &value )
{
  if( mValues.contains( key ) ) {
    mValues.remove( key );
  }
  mValues[key] = value;
}

QString Hint::value( Type key) const
{
  return mValues[key];
}

bool Hint::hasValue( Type key) const
{
  return ( mValues.contains( key ) || mElements.contains( key ) );
}

void Hint::setEnumValue( const QString &value, const QString &replacement )
{
  mEnums.insert( value, replacement );
}

QString Hint::enumValue( const QString &value ) const
{
  if ( mEnums.contains( value ) ) return mEnums[ value ];
  else return QString();
}

void Hint::addElement( Type type, const QDomElement e )
{
  mElements[type].append( e.cloneNode().toElement() );
}

QList<QDomElement> Hint::elements( Type type) const
{
  return mElements[type];
}

QMap< Hint::Type, QList<QDomElement> > Hint::allElements() const
{
  return mElements;
}

void Hint::merge( const Hint &h )
{
  QMap<Type, QString> values = h.values();
  QMap<Type, QString>::iterator it;
  for( it = values.begin(); it != values.end(); ++it ) {
    setValue( it.key(), it.value() );
  }

  foreach( Type type, h.allElements().keys() ) {
      mElements[ type ] = h.elements( type );
  }
}

QString Hint::nameForType( Type t )
{
  QString s;
  switch( t ) {
    case Label:
      s = "label";
      break;
    case ListShowHeader:
      s = "listShowHeader";
      break;
    case ListHeader:
      s = "listHeader";
      break;
    case ListShowSearch:
      s = "listShowSearch";
      break;
    case ListItemLabel:
      s = "listItemLabel";
      break;
    case ListItemList:
      s = "listItemList";
      break;
    case Groups:
      s = "groups";
      break;
    case GroupReference:
      s = "groupRef";
      break;
    case Appearance:
      s = "appearance";
      break;
    case Position:
      s = "position";
      break;
    case LayoutStyle:
      s = "layoutStyle";
      break;
    case ReadOnly:
      s = "readOnly";
      break;
    case InputType:
      s = "type";
      break;
    case FormSizeThreshold:
      s = "formsizethreshold";
    default:
      s = "unknown";
  }

  return s;
}


Hints::Hints()
{
}

bool Hints::parseFile( QFile &file )
{
  kDebug() << "---Hints::parseFile() " << file.fileName() << endl;

  QDomDocument doc;
  doc.setContent( &file );
  
  return parse( doc );
}

bool Hints::parseString( const QString &data )
{
  kDebug() << "---Hints::parseString() " << endl;

  QDomDocument doc;
  doc.setContent( data );

  return parse( doc );
}

bool Hints::parse( const QDomDocument &doc )
{  
  QDomElement documentElement = doc.documentElement();
  QDomNode n;
  for( n = documentElement.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    parseHint( e );
  }

  kDebug() << "---Hints::parse() done" << endl;

  return true;
}

void Hints::parseHint( const QDomElement &element, const Reference &refPrefix )
{
//  kDebug() << "Hints::parseHint()" << element.tagName() << refPrefix << endl;

  Reference ref( element.attribute( "ref" ) );

//  kDebug() << "  REF:" << ref << endl;

  if ( ref.isEmpty() ) ref = refPrefix;
  if ( ref.isEmpty() ) {
    kDebug() << "Error: No reference attribute in hint tag." << endl;
    return;
  }
  if ( !refPrefix.isEmpty() && ref.isRelative() ) {
    ref = refPrefix + ref;
  }
  
  Hint hint( ref );

//  kDebug() << "Hints::parseHint()" << ref << endl;

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    QName name = e.tagName();
    if ( name.localName() == "enum" ) {
      hint.setEnumValue( e.attribute( "value" ), contentAsString( e ) );
    } else if (name.localName() == "label" ) {
      hint.setValue( Hint::Label, contentAsString( e ) );
    } else if (name.localName() == "listHeader" ) {
      hint.setValue( Hint::ListHeader, contentAsString( e ) );
    } else if (name.localName() == "listSearch" ) {
      hint.setValue( Hint::ListShowSearch, contentAsString( e ) );
    } else if (name.localName() == "listItemLabels" ) {
      QDomNode child = n.firstChild();
      while( !child.isNull() ) {
        if( child.nodeName() == "listItemLabel" ) {
          QDomElement e = child.toElement();
          if( !e.isNull() )
            hint.addElement( Hint::ListItemLabel, e );
        }
        child = child.nextSibling();
      }
    } else if (name.localName() == "listItemList" ) {
      hint.setValue( Hint::ListItemList, contentAsString( e ) );
    } else if (name.localName() == "groupRef" ) {
      hint.setValue( Hint::GroupReference, contentAsString( e ) );
    } else if (name.localName() == "appearance" ) {
      hint.setValue( Hint::Appearance, contentAsString( e ) );
    } else if (name.localName() == "layoutStyle" ) {
      hint.setValue( Hint::LayoutStyle, contentAsString( e ) );
    } else if (name.localName() == "position" ) {
      QDomNode child = n.firstChild();
      while( !child.isNull() ) {
        QDomElement e = child.toElement();
        if( !e.isNull() )
          hint.addElement( Hint::Position, e );
        child = child.nextSibling();
      }
    } else if (name.localName() == "groups" ) {
      QDomNode child = n.firstChild();
      while( !child.isNull() ) {
        if( child.nodeName() == "group" ) {
          QDomElement e = child.toElement();
          if( !e.isNull() )
            hint.addElement( Hint::Groups, e );
        }
        child = child.nextSibling();
      }
    } else if (name.localName() == "listShowHeader" ) {
      hint.setValue( Hint::ListShowHeader, contentAsString( e ) );
    } else if (name.localName() == "readOnly" ) {
      hint.setValue( Hint::ReadOnly, contentAsString( e ) );
    } else if (name.localName() == "type" ) {
      hint.setValue( Hint::InputType, contentAsString( e ) );
    } else if (name.localName() == "formSizeThreshold" ) {
      hint.setValue( Hint::FormSizeThreshold, contentAsString( e ) );
    } else {
      kDebug() << "Unknown hint type: " << name.localName() << endl;
    }
  }

  mHints[ ref.toString() ] = hint;
}

QString Hints::toXml()
{
  QString xml;
  xml = "<ugh>\n";

  Hint::List theHints = hints();
  foreach( Hint h, theHints) {
    xml.append( QString( "  <hint ref=\"%1\">\n" ).arg( h.ref().toString() ) );

    QMap<Hint::Type, QString>::iterator it;
    QMap<Hint::Type, QString> values = h.values();
    for( it = values.begin(); it != values.end(); ++it ) {
      xml.append( QString( "    <%1>%2</%1>\n" ).arg( Hint::nameForType( it.key() ), it.value() ) );
    }

    QMap< Hint::Type, QList<QDomElement> >::iterator it2;
    QMap< Hint::Type, QList<QDomElement> > elements = h.allElements();
    for( it2 = elements.begin(); it2 != elements.end(); ++it2 ) {
      xml.append( QString( "    <%1>\n").arg( Hint::nameForType( it2.key() ) ) );

      foreach( QDomElement e, it2.value() ) {
        QString s;
        QTextStream stream( &s );
        e.save( stream, 6);
        xml.append( s );
      }

      xml.append( QString( "    </%1>\n").arg( Hint::nameForType( it2.key() ) ) );
    }
    xml.append( QString( "  </hint>\n") );
  }


  xml.append( "<ugh>" );

  kDebug() << k_funcinfo << xml << endl;

  return xml;
}


void Hints::merge( const Hints &hints )
{
  foreach( Hint h, hints.hints() ) {
    merge( h );
  }
}

void Hints::merge( const Hint &hint )
{
  if( mHints.contains( hint.ref().toString() ) )
    mHints[hint.ref().toString()].merge( hint );
  else
    insertHint( hint );
}

// TODO: Share with snippet from TextArea::loadData()
QString Hints::contentAsString( const QDomElement &element )
{
  QString txt;
  QTextStream ts( &txt, QIODevice::WriteOnly );

  QDomNode n;
  for( n = element.firstChild(); ! n.isNull(); n = n.nextSibling() ) {
    n.save( ts, 0 );
  }

  if ( txt.endsWith( "\n" ) ) txt = txt.left( txt.length() - 1 );

  return txt;
}

Hint Hints::hint( const Reference &ref ) const
{
  // Try to find an exact match
  if( mHints.contains( ref.toString() ) )
    return mHints[ ref.toString() ];

  // Try to find a partial match if there was no exact one
  foreach( Hint h, mHints ) {
    if( ref.matches( h.ref(), false ) ) {
      return h;
    }
  }

  return Hint();
}

Hint::List Hints::hints() const
{
  Hint::List result;

  foreach( Hint h, mHints ) {
    result.append( h );
  }
  
  return result;
}

void Hints::extractHints( const Schema::Document &schemaDocument )
{
  extractHints( schemaDocument.annotations() );
  
  foreach( Schema::Element e, schemaDocument.elements() ) {
    extractHints( e.annotations(), e.identifier() );
  }
  foreach( Schema::Attribute a, schemaDocument.attributes() ) {
    extractHints( a.annotations(), a.identifier() );
  }
}

void Hints::extractHints( const QList<QDomElement> &annotations,
  const Reference &refPrefix )
{
  foreach( QDomElement element, annotations ) {
    for( QDomElement e = element.firstChildElement(); !e.isNull();
         e = e.nextSiblingElement() ) {
      QName name = e.tagName();
      if ( name.localName() == "hint" ) {
        parseHint( e, refPrefix );
      }
    }
  }
}

void Hints::insertHint( const Hint &hint )
{
  mHints[ hint.ref().toString() ] = hint;
}

QString Hints::toRichText() const
{
  QString s;

  foreach( Hint h, mHints ) {
    s += QString( "<b>%1</b><br>" ).arg( h.ref().toString() );

    QMap<Hint::Type, QString> values = h.values();
    foreach( Hint::Type t, values.keys() ) {
      s += QString( "&nbsp;&nbsp;&nbsp;%1: \"%2\"<br>" ).arg( Hint::nameForType( t ), values[t] );
    }
    QMap< Hint::Type, QList<QDomElement> > elementValues = h.allElements();
    foreach( Hint::Type t, elementValues.keys() ) {
      QList<QDomElement> list = elementValues[t];
      foreach( QDomElement e, list ) {
        QString tmp;
        QTextStream stream( &tmp );
        e.save( stream, 0 );
        s += QString( "&nbsp;&nbsp;&nbsp;%1: \"%2\"<br>" ).arg( Hint::nameForType( t ), Qt::escape( tmp.simplified() ) );
      }
    }
  }
  s = QString("<qt><b><u>Hints:</u></b><br>%1</qt>").arg( s );
  return s;
}

void Hints::dump() const
{
  kDebug() << "---Hints::dump()" << endl;
  foreach( Hint h, mHints ) {
    kDebug() << "HINT:" << h.ref().toString() << endl;
  }
  kDebug() << "---Hints::dump() done" << endl << endl;
}
