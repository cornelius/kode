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
#ifndef KODE_VARIABLE_H
#define KODE_VARIABLE_H

#include <QtCore/QList>

#include <kode_export.h>

class QString;

namespace KODE {

class LIBKODE_EXPORT Variable
{
  public:
    typedef QList<Variable> List;

    Variable();
    Variable( const Variable &other );
    Variable( const QString &name, const QString &type, bool isStatic = false );

    virtual ~Variable();

    Variable& operator=( const Variable &other );

    void setType( const QString &type );
    QString type() const;

    void setName( const QString &name );
    QString name() const;

    void setStatic( bool isStatic );
    bool isStatic() const;

    void setInitializer( const QString &initializer );
    QString initializer() const;

  private:
    class Private;
    Private *d;
};

}

#endif
