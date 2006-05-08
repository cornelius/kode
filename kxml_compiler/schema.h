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
#include <QStringList>
#include <QDomElement>
#include <kdepim_export.h>

namespace Schema {

class KSCHEMA_EXPORT Relation
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

    void setChoice( const QString & );
    QString choice() const;

    QString asString( const QString &type = QString::null ) const;

  private:
    QString mTarget;

    int mMinOccurs;
    int mMaxOccurs;
    
    QString mChoice;
};

class KSCHEMA_EXPORT Annotatable
{
  public:
  
    void setDocumentation( const QString & );
    QString documentation() const;

    void setAnnotations( const QList<QDomElement> & );
    /**
      XML annotations. The actual annotations are child elements of the elements
      contained in this list. There should not be made any assumptions about the
      names of the elements in this list.
    */
    QList<QDomElement> annotations() const;
    
  private:
    QString mDocumentation;
    QList<QDomElement> mAnnotations;
};

class KSCHEMA_EXPORT Node : public Annotatable
{
  public:
    enum Type { String, NormalizedString, Token, Enumeration, ComplexType };
  
    Node();
    virtual ~Node();

    void setType( Type );
    Type type() const;
    
    void setIdentifier( const QString & );
    QString identifier() const;
    
    void setName( const QString & );
    QString name() const;

    virtual QString ref() const = 0;

    bool isValid() const;

    void setEnumerationValues( const QStringList & );
    QStringList enumerationValues() const;

  private:
    Type mType;
    QString mIdentifier;
    QString mName;

    QStringList mEnumerationValues;
};

class KSCHEMA_EXPORT Element : public Node
{
  public:
    typedef QList<Element> List;
  
    Element();

    QString ref() const;

    bool mixed() const;

    void setText( bool );
    bool text() const;

    void addElementRelation( const Relation & );
    Relation::List elementRelations() const;

    void addAttributeRelation( const Relation & );
    Relation::List attributeRelations() const;

    bool isEmpty() const;

  private:
    bool mText;

    Relation::List mElementRelations;
    Relation::List mAttributeRelations;
};

class KSCHEMA_EXPORT Attribute : public Node
{
  public:
    typedef QList<Attribute> List;

    Attribute();

    QString ref() const;
};

class KSCHEMA_EXPORT Document : public Annotatable
{
  public:
    Document();

    void setStartElement( const Element & );
    Element startElement() const;

    void addElement( const Element & );
    Element::List elements() const;
    Element element( const QString &identifier ) const;
    Element element( const Relation & ) const;

    void addAttribute( const Attribute & );
    Attribute::List attributes() const;
    Attribute attribute( const QString &identifier ) const;
    Attribute attribute( const Relation & ) const;

    Element::List usedElements() const;

    void dump() const;

    bool isEmpty() const;

  protected:
    void findUsedElements( const Element &e ) const;
    bool addUsedElement( const Element &element ) const;

  private:
    Element mStartElement;

    Element::List mElements;
    Attribute::List mAttributes;

    mutable Element::List mUsedElements;
};

}

#endif
