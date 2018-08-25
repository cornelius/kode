/*
    This file is part of KDE.

    Copyright (c) 2010 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef NAMER_H
#define NAMER_H

#include "schema.h"

#include <QString>

class Namer
{
  public:
    static QString getClassName( const Schema::Element & );
    static QString getClassName( const Schema::Attribute & );
    static QString getClassName( const QString & );

    static QString getAccessor( const Schema::Element & );
    static QString getAccessor( const Schema::Attribute & );
    static QString getAccessor( const QString & );
    static QString getListAccessor( const QString & );

    static QString getMutator( const Schema::Element & );
    static QString getMutator( const Schema::Attribute & );
    static QString getMutator( const QString & );

    /**
     * Replaces the characters which are forbidden
     * in the C type/varaible names with an underscore (_)
     * in the  @param name.
     */
    static QString removeInvalidCharacters( const QString & name );

    /**
     * If the @param name is a reserved C/C++/Qt keyword it suffixes
     * with an underscore (_)
     */
    static QString checkForKeyWords( const QString &name );

    /**
     * Returns a new version of @param name converted to a format
     * which could be compiled to a valid C type or variable name.
     *
     * The sanitization includes:
     * - If the given string is a reserved keyword it will be
     * suffixed with and underscore (_)
     * - The non valid characters will be replaced with an
     * underscore
     */
    static QString sanitize( const QString & name );



  protected:
    static QString upperFirst( const QString &str );
    static QString lowerFirst( const QString &str );

  private:
    static QStringList m_reservedKeyWords;
};

#endif
