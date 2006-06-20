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

#include "enum.h"
#include "function.h"
#include "membervariable.h"
#include "typedef.h"

#include <kode_export.h>

#include <QList>
#include <QString>
#include <QStringList>

namespace KODE {

class LIBKODE_EXPORT Class
{
  public:
    typedef QList<Class> List;

    Class();
    Class( const QString &name, const QString &nameSpace = QString::null );

    bool isValid() const;

    void addInclude( const QString &file,
      const QString &forwardDeclaration = QString::null );
    void addIncludes( const QStringList &files,
      const QStringList &forwardDeclarations = QStringList() );
    void addHeaderInclude( const QString &file );
    void addHeaderIncludes( const QStringList &files );
    void addBaseClass( const Class & );
    void addFunction( const Function & );
    void addMemberVariable( const MemberVariable &v );
    void addTypedef( const Typedef & );
    void addEnum( const Enum & );

    void setName( const QString &name );
    QString name() const { return mName; }
    void setNameSpace( const QString &nameSpace );
    QString nameSpace() const { return mNameSpace; }
    void setUseDPointer( const bool &useDPointer );
    bool useDPointer() const { return mUseDPointer; }
    QStringList includes() const { return mIncludes; }
    QStringList headerIncludes() const { return mHeaderIncludes; }
    QStringList forwardDeclarations() const { return mForwardDeclarations; }
    Function::List functions() const { return mFunctions; }
    MemberVariable::List memberVariables() const { return mMemberVariables; }
    Class::List baseClasses() const;
    Typedef::List typedefs() const { return mTypedefs; }
    Enum::List enums() const { return mEnums; }

    void setDocs( const QString & );
    QString docs() const { return mDocs; }

    bool hasFunction( const QString &name ) const;

    bool isQObject() const;

  private:
    QString mName;
    QString mNameSpace;
    bool mUseDPointer;
    Function::List mFunctions;
    MemberVariable::List mMemberVariables;
    QStringList mIncludes;
    QStringList mForwardDeclarations;
    QStringList mHeaderIncludes;
    QList<Class> mBaseClasses;
    Typedef::List mTypedefs;
    Enum::List mEnums;
    QString mDocs;
};

}

#endif
