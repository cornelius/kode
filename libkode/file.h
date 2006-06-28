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

#include "class.h"
#include "code.h"
#include "license.h"
#include "variable.h"

#include <kode_export.h>

class QString;
class QStringList;

namespace KODE {

class LIBKODE_EXPORT File
{
  public:
    File();
    File( const File &other );

    ~File();

    File& operator=( const File &other );

    void setFilename( const QString &filename );
    QString filename() const;

    void setNameSpace( const QString &nameSpace );
    QString nameSpace() const;

    void setProject( const QString &project );
    QString project() const;

    void addCopyright( int year, const QString &name, const QString &email );
    QStringList copyrightStrings() const;

    void setLicense( const License &license );
    License license() const;

    void addInclude( const QString &include );
    QStringList includes() const;

    void insertClass( const Class &newClass );
    Class::List classes() const;

    bool hasClass( const QString &className );

    Class findClass( const QString &name );

    void clearClasses();
    void clearFileFunctions();
    void clearFileVariables();

    void clearCode();

    void addFileVariable( const Variable &variable );
    Variable::List fileVariables() const;

    void addFileFunction( const Function &function );
    Function::List fileFunctions() const;

    void addFileEnum( const Enum &enumValue );
    Enum::List fileEnums() const;

    void addExternCDeclaration( const QString &externalCDeclaration );
    QStringList externCDeclarations() const;

    void addFileCode( const Code &code );
    Code fileCode() const;

  private:
    class Private;
    Private *d;
};

}

#endif
