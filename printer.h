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
#ifndef KODE_PRINTER_H
#define KODE_PRINTER_H

#include "code.h"
#include "file.h"
#include "style.h"
#include "automakefile.h"

#include <qvaluelist.h>
#include <qstring.h>
#include <qstringlist.h>

namespace KODE {

class Printer
{
  public:
    Printer();
    Printer( const Style & );

    void setCreationWarning( bool );
    void setGenerator( const QString & );
    void setOutputDirectory( const QString & );
    void setSourceFile( const QString & );

    void printHeader( const File & );
    void printImplementation( const File & );
    void printAutoMakefile( const AutoMakefile & );

    QString functionSignature( const Function &f,
      const QString &className = QString::null,
      bool includeClassQualifier = false );

  protected:
    QString creationWarning();
    QString licenseHeader( const File & );
    QString classHeader( const Class & );
    QString classImplementation( const Class & );
    Code functionHeaders( const Function::List &functions,
                                   const QString &className,
                                   Function::AccessSpecifier access );

  private:
    Style mStyle;

    bool mCreationWarning;
    QString mGenerator;
    QString mOutputDirectory;
    QString mSourceFile;
};

}

#endif
