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

#ifndef PAGE_INPUTFIELD_H
#define PAGE_INPUTFIELD_H

#include <qobject.h>

#include <kwsdl/message.h>

#include "inputfield.h"

class PageInputField : public InputField
{
  Q_OBJECT

  public:
    PageInputField( const QString &name, const KWSDL::Message &message );

    virtual void setXMLData( const QDomElement &element );
    virtual void xmlData( QDomDocument &document, QDomElement &parent );

    virtual void setData( const QString &data );
    virtual QString data() const;

    virtual QWidget *createWidget( QWidget *parent );

  private:
    QWidget *mInputWidget;
    const KWSDL::Message mMessage;
};

#endif
