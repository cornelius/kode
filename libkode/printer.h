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
#ifndef KODE_PRINTER_H
#define KODE_PRINTER_H

#include "automakefile.h"
#include "code.h"
#include "file.h"
#include "style.h"

#include <kode_export.h>

class QString;

namespace KODE {

class LIBKODE_EXPORT Printer
{
  public:
    Printer();
    Printer( const Printer &other );
    Printer( const Style &style );

    ~Printer();

    Printer& operator=( const Printer &other );

    void setCreationWarning( bool );
    void setGenerator( const QString &generator );
    void setOutputDirectory( const QString &outputDirectory );
    void setSourceFile( const QString &sourceFile );

    void printHeader( const File &file );
    void printImplementation( const File &file, bool createHeaderInclude = true );
    void printAutoMakefile( const AutoMakefile &autoMakefile );

    QString functionSignature( const Function &function,
                               const QString &className = QString(),
                               bool includeClassQualifier = false );

  protected:
    QString creationWarning();
    QString licenseHeader( const File &file );
    QString classHeader( const Class &classObject, bool );
    QString classImplementation( const Class &classObject );
    Code functionHeaders( const Function::List &functions,
                          const QString &className,
                          int access );

  private:
    class Private;
    Private *d;
};

}

#endif
