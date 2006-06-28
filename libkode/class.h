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
#ifndef KODE_CLASS_H
#define KODE_CLASS_H

#include <QtCore/QList>
#include <QtCore/QStringList>

#include "enum.h"
#include "function.h"
#include "membervariable.h"
#include "typedef.h"

#include <kode_export.h>

class QString;

namespace KODE {

class LIBKODE_EXPORT Class
{
  public:
    typedef QList<Class> List;

    Class();
    Class( const Class &other );
    Class( const QString &name, const QString &nameSpace = QString() );

    ~Class();

    Class& operator=( const Class &other );

    bool isValid() const;

    void setName( const QString &name );
    QString name() const;

    void setNameSpace( const QString &nameSpace );
    QString nameSpace() const;

    void setUseDPointer( const bool &useDPointer );
    bool useDPointer() const;

    void addInclude( const QString &file,
                     const QString &forwardDeclaration = QString() );
    void addIncludes( const QStringList &files,
                      const QStringList &forwardDeclarations = QStringList() );
    QStringList includes() const;
    QStringList forwardDeclarations() const;

    void addHeaderInclude( const QString &file );
    void addHeaderIncludes( const QStringList &files );
    QStringList headerIncludes() const;

    void addFunction( const Function & );
    Function::List functions() const;

    void addMemberVariable( const MemberVariable &v );
    MemberVariable::List memberVariables() const;

    void addBaseClass( const Class & );
    Class::List baseClasses() const;

    void addTypedef( const Typedef & );
    Typedef::List typedefs() const;

    void addEnum( const Enum & );
    Enum::List enums() const;

    void setDocs( const QString & );
    QString docs() const;

    bool hasFunction( const QString &name ) const;

    bool isQObject() const;

  private:
    class Private;
    Private* d;
};

}

#endif
