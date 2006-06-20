/*
    This file is part of KDE.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef PARSERRELAXNG_H
#define PARSERRELAXNG_H

#include "schema.h"

#include <QMap>

#include <iostream>
#include <kode_export.h>

namespace RNG {

class KSCHEMA_EXPORT Pattern
{
  public:
    Pattern();

    bool isEmpty();

    QString asString();

    void merge( Pattern p );
  
    bool optional;
    bool zeroOrMore;
    bool oneOrMore;
    bool choice;
};

class KSCHEMA_EXPORT Reference
{
  public:
    typedef QList<Reference *> List;
  
    Reference() : substituted( false ) {}
  
    QString name;
    Pattern pattern;

    bool substituted;
};

class KSCHEMA_EXPORT Attribute
{
  public:
    typedef QList<Attribute *> List;
  
    QString name;
    QStringList choices;
    QString defaultValue;
    Pattern pattern;
};

class KSCHEMA_EXPORT Element
{
  public:
    typedef QList<Element *> List;
  
    Element();
  
    QString name;
    Element::List elements;
    Attribute::List attributes;
    Reference::List references;
    Pattern pattern;
    bool hasText;
    bool isEmpty;
};

class KSCHEMA_EXPORT ParserRelaxng
{
  public:
    ParserRelaxng();

    Element *parse( const QDomElement &docElement );

    Reference *parseReference( const QDomElement &referenceElement );
    bool parseAttribute( const QDomElement &attributeElement,
                               Attribute *a );
    bool parseElement( const QDomElement &elementElement, Element *e,
                       Pattern pattern );

    void substituteReferences( Element *s );

    Schema::Document convertToSchema( Element *start );
    Schema::Element convertToSchemaElement( Element *e );
    Schema::Attribute convertToSchemaAttribute( const QString &path,
      Attribute *a );
    Schema::Relation convertToRelation( const Pattern &pattern,
      const QString &id );

    void doIndent( int cols );

    void dumpPattern( Pattern pattern );
    void dumpReferences( const Reference::List &references,
                         int indent );
    void dumpAttributes( const Attribute::List &attributes,
                         int indent );
    void dumpElements( const Element::List &elements, int indent );
    void dumpElement( Element *element, int indent );
    void dumpTree( Element *s );
    void dumpDefinitionMap();

  private:
    QMap<QString,Element::List> mDefinitionMap;
    
    Schema::Document mDocument;
};

}

#endif
