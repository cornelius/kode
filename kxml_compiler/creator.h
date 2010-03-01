/*
    This file is part of KDE.

    Copyright (c) 2004-2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "schema.h"

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
#include <kstandarddirs.h>

#include <QFile>
#include <QTextStream>
#include <qdom.h>
#include <QRegExp>
#include <QMap>

#include <iostream>

class Creator
{
  public:
    class ClassFlags {
      public:
        ClassFlags( const Schema::Element &element );
      
        bool hasUpdatedTimestamp() const;
        bool hasId() const;

      private:
        bool m_hasUpdatedTimestamp;
        bool m_hasId;
    };
  
    enum XmlParserType { XmlParserDom, XmlParserDomExternal,
                         XmlParserCustomExternal };
    enum XmlWriterType { XmlWriterCustom, XmlWriterCustomExternal };

    Creator( const Schema::Document &document, XmlParserType p = XmlParserDom,
             XmlWriterType w = XmlWriterCustom );
    void setVerbose( bool verbose );

    void setUseKde( bool useKde );
    bool useKde() const;

    void setLicense( const KODE::License & );

    void setExternalClassPrefix( const QString & );

    bool externalParser() const;
    bool externalWriter() const;

    KODE::File &file();
    void setFilename( const QString& baseName );

    void setParserClass( const KODE::Class & );
    KODE::Class &parserClass();

    const Schema::Document &document() const;

    void create();

    void createCrudFunctions( KODE::Class &c, const QString &type );
    void createProperty( KODE::Class &c, const QString &type,
                         const QString &name );
    void createElementFunctions( KODE::Class &c, const Schema::Element &e,
      const Schema::Relation & );
    void createClass( const Schema::Element &element );

    void registerListTypedef( const QString &type );

    void createListTypedefs();

    void createFileParser( const Schema::Element &element );

    void setDtd( const QString &dtd );

    void createFileWriter( const Schema::Element &element );

    void printFiles( KODE::Printer & );

    QString errorStream() const;
    QString debugStream() const;

    QString getClassName( const Schema::Element & ) const;
    QString getClassName( const Schema::Attribute & ) const;
    QString getClassName( const QString & ) const;

    QString getAccessor( const Schema::Element & ) const;
    QString getAccessor( const Schema::Attribute & ) const;
    QString getAccessor( const QString & ) const;

    QString getMutator( const Schema::Element & ) const;
    QString getMutator( const Schema::Attribute & ) const;
    QString getMutator( const QString & ) const;

  protected:
    void setExternalClassNames();

    void createElementParser( KODE::Class &c, const Schema::Element &e );

    void createElementWriter( KODE::Class &c, const Schema::Element &e );

    void createIndenter( KODE::File & );

    QString upperFirst( const QString &str ) const;
    QString lowerFirst( const QString &str ) const;
    
  private:
    Schema::Document mDocument;
  
    XmlParserType mXmlParserType;
    XmlWriterType mXmlWriterType;
    QString mExternalClassPrefix;

    KODE::File mFile;
    KODE::Class mParserClass;
    KODE::Class mWriterClass;
    QStringList mProcessedClasses;
    QStringList mListTypedefs;

    QString mBaseName;
    QString mDtd;
    bool mVerbose;
    bool mUseKde;
};

class ParserCreator
{
  public:
    ParserCreator( Creator * );
    virtual ~ParserCreator();
  
    Creator *creator() const;

    virtual void createFileParser( const Schema::Element &element ) = 0;
    virtual void createElementParser( KODE::Class &c,
      const Schema::Element &e ) = 0;
  
  private:
    Creator *mCreator;
};

#endif
