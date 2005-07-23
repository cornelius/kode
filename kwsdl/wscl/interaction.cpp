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

#include "interaction.h"

using namespace WSCL;

Interaction::Interaction()
  : mType( Empty )
{
}

Interaction::~Interaction()
{
}

void Interaction::setId( const QString &id )
{
  mId = id;
}

QString Interaction::id() const
{
  return mId;
}

void Interaction::setType( Type type )
{
  mType = type;
}

Interaction::Type Interaction::type() const
{
  return mType;
}

void Interaction::setSendReceiveDocument( const SendReceiveDocument &document )
{
  mSendReceiveDocument = document;
}

SendReceiveDocument Interaction::sendReceiveDocument() const
{
  return mSendReceiveDocument;
}

void Interaction::setReceiveSendDocument( const ReceiveSendDocument &document )
{
  mReceiveSendDocument = document;
}

ReceiveSendDocument Interaction::receiveSendDocument() const
{
  return mReceiveSendDocument;
}

void Interaction::setReceiveDocument( const ReceiveDocument &document )
{
  mReceiveDocument = document;
}

ReceiveDocument Interaction::receiveDocument() const
{
  return mReceiveDocument;
}

void Interaction::setSendDocument( const SendDocument &document )
{
  mSendDocument = document;
}

SendDocument Interaction::sendDocument() const
{
  return mSendDocument;
}

