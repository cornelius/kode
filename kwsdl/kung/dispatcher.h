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

#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <qobject.h>

#include <kwsdl/wsdl.h>

class ConversationManager;
class Transport;

class Dispatcher : public QObject
{
  Q_OBJECT

  public:
    Dispatcher();
    ~Dispatcher();

    void setWSDL( const KWSDL::WSDL &wsdl );

    void run();

  private slots:
    void nextMessage();
    void result( const QString &xml );
    void error( const QString &errorMsg );

  private:
    KWSDL::WSDL mWSDL;

    QString mCurrentMessage;
    QString mLastMessage;
    ConversationManager *mConversationManager;
    Transport *mTransport;
};

#endif
