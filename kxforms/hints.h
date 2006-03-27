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

    Hint();
    Hint( const QString &ref );

    bool isValid() const;
    
    void setRef( const QString & );
    QString ref() const;
    
    void setLabel( const QString & );
    QString label() const;
    
  private:
    QString mRef;
    
    QString mLabel;
};

class Hints
{
  public:
    Hints();

    bool parseFile( QFile & );

    Hint hint( const QString &ref ) const;

    Hint::List hints() const;

  protected:
    void parseHint( const QDomElement & );

  private:
    QMap<QString,Hint> mHints;
};

}

#endif
