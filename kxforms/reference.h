/*
    This file is part of KDE.

    Copyright (c) 2005 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/
#ifndef KXFORMS_REFERENCE_H
#define KXFORMS_REFERENCE_H

#include <QList>
#include <QString>
#include <QDomElement>

namespace KXForms {

/**
  The Reference class represents an XPath reference to data elements or parts of
  forms. They only support a small subset of XPath.
*/
class Reference
{
  public:
    class Segment {
      public:
        typedef QList<Segment> List;

        Segment();
        Segment( const QString &str );
        Segment( const QString &name, int count );

        void setName( const QString &name );
        QString name() const { return mName; }

        void setCount( int count ) { mCount = count; }
        int count() const { return mCount; }

        bool isAttribute() const { return mIsAttribute; }

        void fromString( const QString & );
        QString toString() const;

        bool operator==( const Segment & ) const;
        bool operator!=( const Segment & ) const;

        bool isEmpty() const;

      private:
        QString mName;
        int mCount;
        bool mIsAttribute;
    };

    Reference();
    Reference( const QString & );
    Reference( const char * );

    Reference &operator=( const QString & );
    Reference &operator=( const char * );

    void setPath( const QString & );
    QString path() const;

    Reference &append( const Reference & );
    Reference &append( const Segment & );

    bool isAbsolute() const;
    bool isRelative() const;

    bool isValid() const;
    bool isEmpty() const;

    void fromString( const QString & );
    QString toString() const;

    Segment::List segments() const;

    Reference operator+( const Reference & ) const;
    Reference operator+( const Reference::Segment & ) const;

    bool operator==( const Reference & ) const;
    bool operator!=( const Reference & ) const;

    QString text( const QDomElement & ) const;

  private:
    Segment::List mSegments;

    bool mValid;
    bool mAbsolute;
};

}

#endif
