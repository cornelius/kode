/*
    This file is part of KXForms.

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
#ifndef XMLBUILDER_H
#define XMLBUILDER_H

#include <QList>
#include <QString>
#include <QMap>

class XmlBuilder
{
  public:
    typedef QList<XmlBuilder> List;

    XmlBuilder( const QString &tagName );
    ~XmlBuilder();

    XmlBuilder *tag( const QString &tagName, const QString &text = QString() );

    XmlBuilder *attribute( const QString &name, const QString &value );

    XmlBuilder *text( const QString &text );

    QString print( int indent = 0, bool newLine = true ) const;

    bool isEmpty() const;

    bool hasText() const;
    bool hasChildElements() const;

  protected:
    QString indent( int indent ) const;

  private:
    class Node
    {
      public:
        typedef QList<Node> List;

        Node( const QString & );
        Node( XmlBuilder * );

        bool isText() const;
        bool isXml() const;
      
        QString text() const;
        XmlBuilder *xml() const;

      private:
        QString mText;
        XmlBuilder *mXml;
    };
    
    QString mTagName;
    QMap<QString,QString> mAttributes;
    Node::List mChildren;
    bool mHasText;
    bool mHasChildElements;
};

#endif
