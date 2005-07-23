/*
    This file is part of KDE.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#ifndef KWSDL_MESSAGE_H
#define KWSDL_MESSAGE_H

#include <qmap.h>

namespace KWSDL {

class Message
{
  public:
    typedef QValueList<Message> List;

    class Part
    {
      public:
        typedef QMap<QString, Part> Map;
        typedef QValueList<Part> List;

        Part();
        Part( const QString &name, const QString &type );

        void setName( const QString &name ) { mName = name; }
        QString name() const { return mName; }

        void setType( const QString &type ) { mType = type; }
        QString type() const { return mType; }

      private:
        QString mName;
        QString mType;
    };

    Message();
    Message( const QString &name );

    void setName( const QString &name );
    QString name() const;

    void addPart( const Part &part );
    Part part( const QString &name ) const;
    Part::List parts() const;

  private:
    QString mName;
    Part::List mParts;
};

}

#endif
