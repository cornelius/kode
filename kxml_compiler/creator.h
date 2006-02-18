/*
    This file is part of KDE.

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
#ifndef CREATOR_H
#define CREATOR_H

#include "parserrelaxng.h"

#include <libkode/code.h>
#include <libkode/printer.h>
#include <libkode/typedef.h>
#include <libkode/file.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>

#include <iostream>

class Creator
{
  public:
    enum XmlParserType { XmlParserDom, XmlParserDomExternal,
                         XmlParserCustomExternal };
    enum XmlWriterType { XmlWriterCustom, XmlWriterCustomExternal };

    Creator( XmlParserType p = XmlParserDom,
             XmlWriterType w = XmlWriterCustom );

    void setExternalClassPrefix( const QString & );

    bool externalParser() const;
    bool externalWriter() const;

    KODE::File &file();

    QString upperFirst( const QString &str );
    QString lowerFirst( const QString &str );
    
    void createProperty( KODE::Class &c, const QString &type,
                         const QString &name );
    void createElementFunctions( KODE::Class &c, RNG::Element *e );
    void createClass( RNG::Element *element );

    void registerListTypedef( const QString &type );

    void createListTypedefs();

    void createFileParser( RNG::Element *element );

    void createFileWriter( RNG::Element *element, const QString &dtd );

    void printFiles( KODE::Printer & );

  protected:
    void setExternalClassNames();

    void createFileParserDom( RNG::Element *element );
    void createFileParserCustom( RNG::Element *element );

    void createElementParser( KODE::Class &c, RNG::Element *e );

    void createElementParserDom( KODE::Class &c, RNG::Element *e );

    void createElementParserCustom( KODE::Class &c, RNG::Element *e );
    void createTextElementParserCustom( KODE::Class &c, RNG::Element *e );
    KODE::Code createAttributeScanner( RNG::Attribute *a, bool firstAttribute );
    void createFoundTextFunction( const QString &text );

    void createElementWriter( KODE::Class &c, RNG::Element *e );

    void createIndenter( KODE::File & );

  private:
    XmlParserType mXmlParserType;
    XmlWriterType mXmlWriterType;
    QString mExternalClassPrefix;

    KODE::File mFile;
    KODE::Class mParserClass;
    KODE::Class mWriterClass;
    QStringList mProcessedClasses;
    QStringList mListTypedefs;
};

#endif
