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

#ifndef KWSDL_BINDING_H
#define KWSDL_BINDING_H

#include <qmap.h>

namespace KWSDL {

class Binding
{
  public:
    typedef QValueList<Binding> List;

    class Operation
    {
      public:
        typedef QValueList<Operation> List;

        class Item
        {
          public:
            void setUse( const QString &use ) { mUse = use; }
            QString use() const { return mUse; }

            void setNameSpace( const QString &nameSpace ) { mNameSpace = nameSpace; }
            QString nameSpace() const { return mNameSpace; }

            void setEncodingStyle( const QString &encodingStyle ) { mEncodingStyle = encodingStyle; }
            QString encodingStyle() const { return mEncodingStyle; }

          private:
            QString mUse;
            QString mNameSpace;
            QString mEncodingStyle;
        };

        Operation();
        Operation( const QString &name, const QString &action );

        void setName( const QString &name ) { mName = name; }
        QString name() const { return mName; }

        void setAction( const QString &action ) { mAction = action; }
        QString action() const { return mAction; }

        void setInput( const Item &item ) { mInput = item; }
        Item input() const { return mInput; }

        void setOutput( const Item &item ) { mOutput = item; }
        Item output() const { return mOutput; }

      private:
        QString mName;
        QString mAction;
        Item mInput;
        Item mOutput;
    };

    Binding();
    Binding( const QString &name, const QString &type );

    void setName( const QString &name ) { mName = name; }
    QString name() const { return mName; }

    void setType( const QString &type ) { mType = type; }
    QString type() const { return mType; }

    void setStyle( const QString &style ) { mStyle = style; }
    QString style() const { return mStyle; }

    void setTransport( const QString &transport ) { mTransport = transport; }
    QString transport() const { return mTransport; }

    void addOperation( const Operation &operation );
    Operation operation( const QString &name ) const;
    Operation::List operations() const;

  private:
    QString mName;
    QString mType;
    QString mStyle;
    QString mTransport;
    Operation::List mOperations;
};

}

#endif
