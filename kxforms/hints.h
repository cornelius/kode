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
#ifndef HINTS_H
#define HINTS_H

#include "reference.h"
#include <kxml_compiler/schema.h>

#include <QList>
#include <QString>
#include <QFile>
#include <QMap>
#include <QDomElement>

namespace KXForms {

class Hint
{
  public:
    typedef QList<Hint> List;
    enum Type { Label, ListShowHeader, ListHeader, ListShowSearch, ListItemLabel, 
        ListItemList, Groups, GroupReference, Appearance, Position, LayoutStyle,
        ReadOnly, InputType, FormSizeThreshold };

    Hint();
    Hint( const Reference &ref );

    bool isValid() const;
    
    void setRef( const Reference & );
    Reference ref() const;

    void setValue( Type, const QString & );
    QString value( Type ) const;

    bool hasValue( Type ) const;

    void addElement( Type, const QDomElement );
    QList<QDomElement> elements( Type ) const;
    QMap< Type, QList<QDomElement> > allElements() const;

    void merge( const Hint & h );

    void setEnumValue( const QString &value, const QString &replacement );
    QString enumValue( const QString &value ) const;

    QMap<Type, QString> values() const { return mValues; }

    static QString nameForType( Type t );
  private:
    Reference mRef;
    QMap<Type, QString> mValues;
    QMap<QString, QString> mEnums;
    QMap< Type, QList<QDomElement> > mElements;
};

class Hints
{
  public:
    Hints();

    bool parseFile( QFile & );
    bool parseString( const QString & );

    QString toXml();

    void insertHint( const Hint & );
    
    Hint hint( const Reference &ref ) const;

    Hint::List hints() const;

    void extractHints( const Schema::Document & );

    void merge( const Hints &h );
    void merge( const Hint &h );

    void dump() const;
    QString toRichText() const;

  protected:
    bool parse( const QDomDocument &doc );
    void parseHint( const QDomElement &, const Reference &refPrefix = Reference() );

    void extractHints( const QList<QDomElement> &annotations,
      const Reference &refPrefix = Reference() );

    QString contentAsString( const QDomElement &e );

  private:
    QMap<QString,Hint> mHints;
};

}

#endif
