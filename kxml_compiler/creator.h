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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
#ifndef CREATOR_H
#define CREATOR_H

#include "parser.h"

#include <kode/kode.h>
#include <kode/printer.h>
#include <kode/typedef.h>
#include <kode/file.h>

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
    Creator();

    KODE::File &file();

    QString upperFirst( const QString &str );
    QString lowerFirst( const QString &str );
    
    void createProperty( KODE::Class &c, const QString &type,
                         const QString &name );
    void createElementFunctions( KODE::Class &c, Element *e );
    void createClass( Element *element );
    void createElementParser( KODE::Class &c, Element *e );
    void createElementWriter( KODE::Class &c, Element *e );

    void registerListTypedef( const QString &type );

    void createListTypedefs();

    void createFileParser( Element *element );
    void createFileWriter( Element *element, const QString &dtd );

  private:
    KODE::File mFile;
    QStringList mProcessedClasses;
    QStringList mListTypedefs;    
};

#endif
