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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
#ifndef KODE_CLASS_H
#define KODE_CLASS_H

#include "membervariable.h"
#include "function.h"
#include "typedef.h"

#include <kdepimmacros.h>
#include <qvaluelist.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qptrlist.h>

namespace KODE {

class KDE_EXPORT Class
{
  public:
    typedef QValueList<Class> List;
    
    Class();
    Class( const QString &name, const QString &nameSpace = QString::null );

    Class( const Class & );
    Class &operator=( const Class &c );

    bool isValid() const;

    void addInclude( const QString &file,
      const QString &forwardDeclaration = QString::null );
    void addHeaderInclude( const QString &file );
    void addBaseClass( const Class & );
    void addFunction( const Function & );
    void addMemberVariable( const MemberVariable &v );
    void addTypedef( const Typedef & );

    void setName( const QString &name );
    QString name() const { return mName; }
    void setNameSpace( const QString &nameSpace );
    QString nameSpace() const { return mNameSpace; }
    QStringList includes() const { return mIncludes; }
    QStringList headerIncludes() const { return mHeaderIncludes; }
    QStringList forwardDeclarations() const { return mForwardDeclarations; }
    Function::List functions() const { return mFunctions; }
    MemberVariable::List memberVariables() const { return mMemberVariables; }
    Class::List baseClasses() const;
    Typedef::List typedefs() const { return mTypedefs; }

    void setDocs( const QString & );
    QString docs() const { return mDocs; }

    bool hasFunction( const QString &name ) const;
    
  private:
    // WARNING: If you add member variables, you have to adjust the copy
    //          constructor.
    QString mName;
    QString mNameSpace;
    Function::List mFunctions;
    MemberVariable::List mMemberVariables;
    QStringList mIncludes;
    QStringList mForwardDeclarations;
    QStringList mHeaderIncludes;
    QPtrList<Class> mBaseClasses;
    Typedef::List mTypedefs;
    QString mDocs;
};

}

#endif
