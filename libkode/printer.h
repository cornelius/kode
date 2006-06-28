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

/**
 * This class prints the abstract class definitions
 * as C++ code to a file.
 */
class LIBKODE_EXPORT Printer
{
  public:
    /**
     * Creates a new printer.
     */
    Printer();

    /**
     * Creates a new printer from @param other.
     */
    Printer( const Printer &other );

    /**
     * Creates a new printer, which uses the given @param style
     * to format the C++ code.
     */
    Printer( const Style &style );

    /**
     * Destroys the printer.
     */
    virtual ~Printer();

    /**
     * Assignment operator.
     */
    Printer& operator=( const Printer &other );

    /**
     * Sets whether the implementation shall contain a comment
     * which warns about changing the C++ code manually.
     */
    void setCreationWarning( bool value );

    /**
     * Sets the name of the @param generator which shall be
     * included in the C++ code.
     */
    void setGenerator( const QString &generator );

    /**
     * Sets the directory where the decleration and implementation
     * files shall be stored. If now directory is set, the current
     * working directory is used.
     */
    void setOutputDirectory( const QString &outputDirectory );

    /**
     * Sets the name of the source file which is included in
     * the generator statement.
     */
    void setSourceFile( const QString &sourceFile );

    /**
     * Prints the header of the class definitions in @param file.
     */
    void printHeader( const File &file );

    /**
     * Prints the implementation of the class definitions in @param file.
     *
     * @param createHeaderInclude If true, the header for the declaration of
     *                            this implementation is included.
     */
    void printImplementation( const File &file, bool createHeaderInclude = true );

    /**
     * Prints a automake file as defined by @param autoMakefile.
     */
    void printAutoMakefile( const AutoMakefile &autoMakefile );

    /**
     * Returns the function signature for the given @param function
     * with class name @param className.
     *
     * If @param includeClassQualifier is true, the class qualifier
     * is part of the signature as well.
     */
    QString functionSignature( const Function &function,
                               const QString &className = QString(),
                               bool includeClassQualifier = false );

  protected:
    /**
     * Returns the creation warning.
     *
     * Reimplement this method to provide a custom warning.
     */
    virtual QString creationWarning() const;

    /**
     * Returns the license header for the given @param file.
     *
     * Reimplement this method to provide a custom license
     * header.
     */
    virtual QString licenseHeader( const File &file ) const;


  private:
    class Private;
    Private *d;
};

}

#endif
