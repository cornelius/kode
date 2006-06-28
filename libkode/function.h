/*
    This file is part of kdepim.

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
#ifndef KODE_FUNCTION_H
#define KODE_FUNCTION_H

#include <QtCore/QList>

#include "code.h"

#include <kode_export.h>

class QString;
class QStringList;

namespace KODE {

class LIBKODE_EXPORT Function
{
  public:
    typedef QList<Function> List;

    enum AccessSpecifier { Public = 1, Protected = 2, Private = 4, Signal = 8, Slot = 16 };

    Function();
    Function( const Function &other );
    Function( const QString &name, const QString &returnType = QString(),
              int access = Public, bool isStatic = false );

    ~Function();

    Function& operator=( const Function &other );

    void setConst( bool isConst );
    bool isConst() const;

    void setStatic( bool isStatic );
    bool isStatic() const;

    void addArgument( const QString &argument );
    void setArgumentString( const QString &argumentString );
    QStringList arguments() const;

    void addInitializer( const QString &initializer );
    QStringList initializers() const;

    void setBody( const QString &body );
    void setBody( const Code &code );
    void addBodyLine( const QString &bodyLine );
    QString body() const;

    void setAccess( int access );
    int access() const;
    QString accessAsString() const;

    void setReturnType( const QString &returnType );
    QString returnType() const;

    void setName( const QString &name );
    QString name() const;

    void setDocs( const QString &docs );
    QString docs() const;

  private:
    class FunctionPrivate;
    FunctionPrivate *d;
};

}

#endif
