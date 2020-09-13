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
#include <QObject>
#include <QString>
#include <QStringList>
#include <QDomElement>

#include <kode_export.h>

namespace Schema {

class SCHEMA_EXPORT Relation
{
public:
    typedef QList<Relation> List;

    enum { Unbounded = -1 };

    Relation();
    Relation(const QString &targetName);

    void setMinOccurs(int);
    void setMaxOccurs(int);

    int minOccurs() const;
    int maxOccurs() const;

    bool isOptional() const;
    bool isRequired() const;
    bool isList() const;

    void setTarget(const QString &identifier);
    QString target() const;

    void setChoice(const QString &);
    QString choice() const;

    QString asString(const QString &type = QString()) const;

private:
    QString mTarget;

    int mMinOccurs;
    int mMaxOccurs;

    QString mChoice;
};

class SCHEMA_EXPORT Annotatable
{
public:
    void setDocumentation(const QString &);
    QString documentation() const;

    void setAnnotations(const QList<QDomElement> &);
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

class SCHEMA_EXPORT Node : public Annotatable
{
    Q_GADGET
public:
    enum Type {
        None,
        String,
        NormalizedString,
        Token,
        Integer, // xs:integer -> integer unbounded value
        Int, // xs:int -> signed 32-bit integer
        Date,
        Enumeration,
        ComplexType,
        DateTime,
        Decimal,
        Boolean,
        UnsignedLong // xs:unsignedLong -> Unsigned integer of 64 bits
    };
    Q_ENUM(Type)
    Node();
    virtual ~Node();

    void setType(Type);
    Type type() const;
    QString typeName() const;

    void setIdentifier(const QString &);
    QString identifier() const;

    void setName(const QString &);
    QString name() const;

    void setBaseType(Type);
    Type baseType() const;

    virtual QString ref() const = 0;

    bool isValid() const;

    void setEnumerationValues(const QStringList &);
    QStringList enumerationValues() const;

private:
    Type mType;
    QString mIdentifier;
    QString mName;
    Type mBaseType;

    QStringList mEnumerationValues;
};

class SCHEMA_EXPORT Attribute : public Node
{
public:
    typedef QList<Attribute> List;

    Attribute();

    bool required() const;
    void setRequired(bool required);

    QString defaultValue() const;
    void setDefaultValue(const QString defVal);

    QString ref() const;
    void setElementName(const QString &elementName);
    QString elementName() const;

private:
    bool mRequired = false;
    QString mDefVal;
    QString mElementName;
};

class SCHEMA_EXPORT Element : public Node
{
public:
    typedef QList<Element> List;

    Element();

    QString ref() const;

    bool mixed() const;

    void setText(bool);
    bool text() const;

    bool hasRelation(const QString &identifier) const;

    void addElementRelation(const Relation &);
    bool hasElementRelation(const Element &) const;
    bool hasElementRelation(const QString &identifier) const;
    Relation &elementRelation(const Element &);
    Relation::List elementRelations() const;
    bool hasElementRelations() const;

    void addAttributeRelation(const Relation &);
    bool hasAttributeRelation(const Attribute &) const;
    bool hasAttributeRelation(const QString &identifier) const;
    Relation::List attributeRelations() const;
    bool hasAttributeRelations() const;

    bool isEmpty() const;

private:
    bool mText;

    Relation::List mElementRelations;
    Relation::List mAttributeRelations;

    Relation mNullRelation;
};

class SCHEMA_EXPORT Document : public Annotatable
{
public:
    Document();

    void setStartElement(const Element &);
    Element startElement() const;

    void addElement(const Element &);
    Element::List elements() const;
    bool hasElement(const Element &);
    Element element(const QString &identifier) const;
    Element element(const Relation &) const;

    void addAttribute(const Attribute &);
    Attribute::List attributes() const;
    bool hasAttribute(const Attribute &);
    Attribute attribute(const QString &identifier, const QString &parentName = QString("")) const;
    Attribute attribute(const Relation &relation, const QString &parentName = QString("")) const;

    Element::List usedElements() const;

    void dump() const;

    bool isEmpty() const;

protected:
    void findUsedElements(const Element &e) const;
    bool addUsedElement(const Element &element) const;

private:
    Element mStartElement;

    Element::List mElements;
    Attribute::List mAttributes;

    mutable Element::List mUsedElements;
};
}

#endif
