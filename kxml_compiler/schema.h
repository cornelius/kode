/*
    This file is part of KDE.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef SCHEMA_H
#define SCHEMA_H

#include <QList>
#include <QString>

namespace Schema {

class Relation
{
  public:
    typedef QList<Relation> List;
    
    enum { Unbounded = -1 };

    Relation( const QString &targetName );
  
    void setMinOccurs( int );
    void setMaxOccurs( int );
  
    int minOccurs() const;
    int maxOccurs() const;
  
    bool isOptional() const;
    bool isRequired() const;
    bool isList() const;

    void setTarget( const QString &identifier );
    QString target() const;

  private:
    QString mTarget;

    int mMinOccurs;
    int mMaxOccurs;
};

class Node
{
  public:
    Node();
    
    void setIdentifier( const QString & );
    QString identifier() const;
    
    void setName( const QString & );
    QString name() const;
    
    bool isValid() const;
    
  private:
    QString mIdentifier;
    QString mName;
};

class Element : public Node
{
  public:
    typedef QList<Element> List;
  
    Element();

    void setMixed( bool );
    bool mixed() const;

    void addElementRelation( const Relation & );
    Relation::List elementRelations() const;
  
    void addAttributeRelation( const Relation & );
    Relation::List attributeRelations() const;

  private:
    bool mMixed;

    Relation::List mElementRelations;
    Relation::List mAttributeRelations;
};

class Attribute : public Node
{
  public:
    typedef QList<Attribute> List;

    Attribute();
};

class Document
{
  public:
    Document();

    void setStartElement( const Element & );
    Element startElement() const;

    void addElement( const Element & );
    Element::List elements() const;
    Element element( const QString &identifier );

    void addAttribute( const Attribute & );
    Attribute::List attributes() const;
    Attribute attribute( const QString &identifier );

    void dump() const;
    void dumpRelation( const Relation &r, const QString &type ) const;

  private:
    Element mStartElement;

    Element::List mElements;
    Attribute::List mAttributes;
};

}

#endif
