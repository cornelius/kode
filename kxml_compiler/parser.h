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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
#ifndef PARSER_H
#define PARSER_H

#include <kode/code.h>
#include <kode/printer.h>
#include <kode/typedef.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>

#include <iostream>

class Pattern
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

class Reference
{
  public:
    typedef QValueList<Reference *> List;
  
    Reference() : substituted( false ) {}
  
    QString name;
    Pattern pattern;

    bool substituted;
};

class Attribute
{
  public:
    typedef QValueList<Attribute *> List;
  
    QString name;
    QValueList<QString> choices;
    QString defaultValue;
    Pattern pattern;
};

class Element
{
  public:
    typedef QValueList<Element *> List;
  
    Element();
  
    QString name;
    Element::List elements;
    Attribute::List attributes;
    Reference::List references;
    Pattern pattern;
    bool hasText;
    bool isEmpty;
};

class Parser
{
  public:
    Parser();

    Element *parse( const QDomElement &docElement );

    Reference *parseReference( const QDomElement &referenceElement );
    bool parseAttribute( const QDomElement &attributeElement,
                               Attribute *a );
    bool parseElement( const QDomElement &elementElement, Element *e,
                       Pattern pattern );

    void substituteReferences( Element *s );

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
};

#endif
