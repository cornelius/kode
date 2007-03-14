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
    enum Type { Label, ItemLabelRef };

    Hint();
    Hint( const QString &ref );

    bool isValid() const;
    
    void setRef( const QString & );
    QString ref() const;

    void setValue( Type, const QString & );
    QString value( Type ) const;

    void setEnumValue( const QString &value, const QString &replacement );    
    QString enumValue( const QString &value ) const;

  private:
    QString mRef;
    QMap<Type, QString> mValues;
    QMap<QString, QString> mEnums;
};

class Hints
{
  public:
    Hints();

    bool parseFile( QFile & );
    bool parseString( const QString & );

    void insertHint( const Hint & );
    
    Hint hint( const QString &ref ) const;

    Hint::List hints() const;

    void extractHints( const Schema::Document & );

    void dump() const;

  protected:
    bool parse( const QDomDocument &doc );
    void parseHint( const QDomElement &, const QString &refPrefix = QString() );

    void extractHints( const QList<QDomElement> &annotations,
      const QString &refPrefix = QString() );

    QString contentAsString( const QDomElement &e );

  private:
    QMap<QString,Hint> mHints;
};

}

#endif
