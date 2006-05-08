/*
    This file is part of KDE Schema Parser

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <QDomElement>
#include <QList>
#include <kdepim_export.h>

namespace XSD {

class KODE_SCHEMA_EXPORT Annotation
{
  public:
    class List : public QList<Annotation>
    {
      public:
        QString documentation() const;
    };
  
    Annotation();
    Annotation( const QDomElement & );

    void setDomElement( const QDomElement & );
    QDomElement domElement() const;

    bool isDocumentation() const;
    bool isAppinfo() const;

    QString documentation() const;

  private:
    QDomElement mDomElement;
};

}

#endif
