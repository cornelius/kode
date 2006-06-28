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
#ifndef KODE_AUTOMAKEFILE_H
#define KODE_AUTOMAKEFILE_H

#include <QtCore/QList>
#include <QtCore/QString>

#include <kode_export.h>

namespace KODE {

class LIBKODE_EXPORT AutoMakefile
{
  public:
    class LIBKODE_EXPORT Target
    {
      public:
        typedef QList<Target> List;

        Target();
        Target( const Target &other );
        Target( const QString &type, const QString &name );

        ~Target();

        Target& operator=( const Target &other );

        void setType( const QString &type );
        QString type() const;

        void setName( const QString &name );
        QString name() const;

        void setSources( const QString &sources );
        QString sources() const;

        void setLibAdd( const QString &libAdd );
        QString libAdd() const;

        void setLdAdd( const QString &ldAdd );
        QString ldAdd() const;

        void setLdFlags( const QString &ldFlags );
        QString ldFlags() const;

      private:
        class Private;
        Private* d;
    };

    AutoMakefile();
    AutoMakefile( const AutoMakefile &other );

    ~AutoMakefile();

    AutoMakefile& operator=( const AutoMakefile &other );

    void addTarget( const Target &target );
    Target::List targets() const;

    void addEntry( const QString &variable,
                   const QString &value = QString() );

    void newLine();

    QString text() const;

  private:
    class Private;
    Private* d;
};

}

#endif
