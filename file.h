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
#ifndef KODE_FILE_H
#define KODE_FILE_H

#include "license.h"
#include "code.h"
#include "class.h"
#include "membervariable.h"

#include <qstring.h>
#include <qstringlist.h>
#include <kdepimmacros.h>

namespace KODE {

class KDE_EXPORT File
{
  public:
    File();

    void setFilename( const QString & );
    QString filename() const;

    void setNameSpace( const QString & );
    QString nameSpace() const { return mNameSpace; }

    void setProject( const QString &project );
    QString project() const { return mProject; }

    void addCopyright( int year, const QString &name, const QString &email );
    QStringList copyrightStrings() const { return mCopyrightStrings; }

    void setLicense( const License & );
    License license() const { return mLicense; }

    void addInclude( const QString &include );
    QStringList includes() const { return mIncludes; }

    void insertClass( const Class & );
    Class::List classes() const { return mClasses; }

    bool hasClass( const QString &className );

    Class findClass( const QString &name );

    void clearClasses();
    void clearFileFunctions();
    void clearFileVariables();

    void clearCode();

    void addFileVariable( const Variable & );
    Variable::List fileVariables() const { return mFileVariables; }

    void addFileFunction( const Function & );
    Function::List fileFunctions() const { return mFileFunctions; }

    void addExternCDeclaration( const QString & );
    QStringList externCDeclarations() const { return mExternCDeclarations; }

    void addFileCode( const Code & );
    Code fileCode() const { return mFileCode; }

  private:
    QString mFilename;
    QString mNameSpace;
    QString mProject;
    QStringList mCopyrightStrings;
    License mLicense;
    QStringList mIncludes;
    Class::List mClasses;
    Variable::List mFileVariables;
    Function::List mFileFunctions;
    QStringList mExternCDeclarations;
    Code mFileCode;
};

}

#endif
