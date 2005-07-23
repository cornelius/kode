/*
    This file is part of Kung.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#ifndef INPUTFIELD_H
#define INPUTFIELD_H

#include <qdom.h>
#include <qobject.h>
#include <qstring.h>
#include <qvaluelist.h>

class QWidget;

namespace Schema {
class SimpleType;
class ComplexType;
}

/**
  Abstract base class for all kinds of input fields.
 */
class InputField : public QObject
{
  Q_OBJECT

  public:
    typedef QValueList<InputField*> List;

    /**
      Constructs a input field.
     */
    InputField( const QString &name );

    /**
      Destructor.
     */
    virtual ~InputField();

    /**
      Returns the name of the field.
     */
    QString name() const;

    /**
      Sets the xml data of this field.
     */
    virtual void setXMLData( const QDomElement &element ) = 0;

    /**
      Returns the xml value of this field.
     */
    virtual void xmlData( QDomDocument &document, QDomElement &parent ) = 0;

    /**
      Sets the plain data of this field as string.
     */
    virtual void setData( const QString &data ) = 0;

    /**
      Returns the data of this field as string.
     */
    virtual QString data() const = 0;

    /**
      Returns a pointer to a new created input widget which can be used to
      to modify the value.
     */
    virtual QWidget *createWidget( QWidget *parent ) = 0;

    /**
      Add a child field.
     */
    virtual void appendChild( InputField *field );

    /**
      Removes a child field.
     */
    virtual void removeChild( InputField *field );

    /**
      Returns a field with the given name or 0 when no field with this name
      exists.
     */
    virtual InputField *childField( const QString &name ) const;

    /**
      Returns a list of all child fields.
     */
    virtual InputField::List childFields() const;

  signals:
    /**
      This signal is emitted whenever the value is changed by the user.
     */
    void modified();

  protected:
    QString mName;
    InputField::List mFields;
};

/**
  Abstract base class for simple input fields like text or number input.
 */
class SimpleInputField : public InputField
{
  public:
    SimpleInputField( const QString &name, const Schema::SimpleType *type );
    ~SimpleInputField();

  protected:
    Schema::SimpleType *mType;
};

/**
  Abstract base class for complex input fields like pages, lists or tables.
 */
class ComplexInputField : public InputField
{
  public:
    ComplexInputField( const QString &name, const Schema::ComplexType *type );
    ~ComplexInputField();

  protected:
    Schema::ComplexType *mType;
};

#endif
