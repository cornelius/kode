/* 
    This file is part of KDE WSCL Parser

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

#ifndef WSCL_INTERACTION_H
#define WSCL_INTERACTION_H

#include <qvaluelist.h>

#include "documents.h"

namespace WSCL {

class Interaction
{
  public:
    typedef QValueList<Interaction> List;

    enum Type
    {
      ReceiveSend,
      SendReceive,
      Receive,
      Send,
      Empty
    };

    Interaction();
    ~Interaction();

    void setId( const QString &id );
    QString id() const;

    void setType( Type type );
    Type type() const;

    void setSendReceiveDocument( const SendReceiveDocument &document );
    SendReceiveDocument sendReceiveDocument() const;

    void setReceiveSendDocument( const ReceiveSendDocument &document );
    ReceiveSendDocument receiveSendDocument() const;

    void setReceiveDocument( const ReceiveDocument &document );
    ReceiveDocument receiveDocument() const;

    void setSendDocument( const SendDocument &document );
    SendDocument sendDocument() const;

  private:
    QString mId;
    Type mType;

    SendReceiveDocument mSendReceiveDocument;
    ReceiveSendDocument mReceiveSendDocument;
    ReceiveDocument mReceiveDocument;
    SendDocument mSendDocument;
};

}

#endif
