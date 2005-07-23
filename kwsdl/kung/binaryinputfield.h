/*
    This file is part of Kung.

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

#ifndef BINARY_INPUTFIELD_H
#define BINARY_INPUTFIELD_H

#include <qwidget.h>

#include "inputfield.h"

class BinaryWidget;
class QGridLayout;
class QPushButton;

class BinaryInputField : public SimpleInputField
{
  Q_OBJECT

  public:
    BinaryInputField( const QString &name, const QString &typeName, const Schema::SimpleType *type );

    virtual void setXMLData( const QDomElement &element );
    virtual void xmlData( QDomDocument &document, QDomElement &parent );

    virtual void setData( const QString &data );
    virtual QString data() const;

    virtual QWidget *createWidget( QWidget *parent );

  private slots:
    void valueChanged( const QByteArray &value );

  private:
    BinaryWidget *mInputWidget;

    QByteArray mValue;
    QString mTypeName;
};

class BinaryWidget : public QWidget
{
  Q_OBJECT

  public:
    BinaryWidget( QWidget *parent );

    void setData( const QByteArray &data );

  signals:
    void valueChanged( const QByteArray &data );

  private slots:
    void load();
    void save();

  private:
    QByteArray mData;

    QGridLayout *mLayout;
    QPushButton *mLoadButton;
    QPushButton *mSaveButton;
    QWidget* mMainWidget;
};

#endif
