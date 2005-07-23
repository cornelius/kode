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
#ifndef KODE_CODE_H
#define KODE_CODE_H

#include "license.h"

#include <qvaluelist.h>
#include <qstring.h>
#include <qstringlist.h>
#include <kdepimmacros.h>
namespace KODE {

class KDE_EXPORT Code
{
  public:
    Code();
    Code( int indent );

    void clear();

    bool isEmpty() const;

    void setIndent( int indent );
    void indent();
    void unindent();

    QString text() const { return mText; }

    void addLine( const QString & );
    void addBlock( const Code & );
    void addBlock( const QString & );
    void addBlock( const QString &, int indent );

    void addWrappedText( const QString & );
    void addFormattedText( const QString & );

    void newLine();

    Code &operator+=( const QString & );
    Code &operator+=( const char * );
    Code &operator+=( const Code & );

    static QString spaces( int count );

  private:
    QString mText;
    int mIndent;
};

}

#endif
