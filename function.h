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
#ifndef KODE_FUNCTION_H
#define KODE_FUNCTION_H

#include "code.h"

#include <qvaluelist.h>
#include <qstring.h>
#include <qstringlist.h>
#include <kdepimmacros.h>

namespace KODE {

class KDE_EXPORT Function
{
  public:
    typedef QValueList<Function> List;

    enum AccessSpecifier { Public, Protected, Private };

    Function();
    Function( const QString &name, const QString &returnType = QString::null,
              AccessSpecifier access = Public, bool isStatic = false );

    void setConst( bool isConst );
    bool isConst() const { return mIsConst; }
    
    void setStatic( bool isStatic );
    bool isStatic() const { return mIsStatic; }
    
    void addArgument( const QString &argument );
    void setArgumentString( const QString &argumentString );
    
    void addInitializer( const QString & );
    QStringList initializers() const { return mInitializers; }
    
    void setBody( const QString &body );
    void setBody( const Code &code );
    void addBodyLine( const QString &bodyLine );

    void setAccess( AccessSpecifier );
    AccessSpecifier access() const { return mAccess; }
    QString accessAsString() const;

    void setReturnType( const QString & );
    QString returnType() const { return mReturnType; }

    void setName( const QString & );
    QString name() const { return mName; }

    QStringList arguments() const { return mArguments; }

    QString body() const { return mBody; }

    void setDocs( const QString & );
    QString docs() const { return mDocs; }

  private:
    AccessSpecifier mAccess;
    bool mIsConst;
    bool mIsStatic;
    QString mReturnType;
    QString mName;
    QStringList mArguments;
    QStringList mInitializers;
    QString mBody;
    QString mDocs;
};

}

#endif
