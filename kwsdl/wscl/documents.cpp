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

#include "documents.h"

using namespace WSCL;

void XMLDocument::setId( const QString &id )
{
  mId = id;
}

QString XMLDocument::id() const
{
  return mId;
}

void XMLDocument::setSchema( const QString &schema )
{
  mSchema = schema;
}

QString XMLDocument::schema() const
{
  return mSchema;
}


void ReceiveSendDocument::setInputDocument( const XMLDocument &document )
{
  mInputDocument = document;
}

XMLDocument ReceiveSendDocument::inputDocument() const
{
  return mInputDocument;
}

void ReceiveSendDocument::setOutputDocuments( const XMLDocument::List &documents )
{
  mOutputDocuments = documents;
}

XMLDocument::List ReceiveSendDocument::outputDocuments() const
{
  return mOutputDocuments;
}


void SendReceiveDocument::setInputDocuments( const XMLDocument::List &documents )
{
  mInputDocuments = documents;
}

XMLDocument::List SendReceiveDocument::inputDocuments() const
{
  return mInputDocuments;
}

void SendReceiveDocument::setOutputDocument( const XMLDocument &document )
{
  mOutputDocument = document;
}

XMLDocument SendReceiveDocument::outputDocument() const
{
  return mOutputDocument;
}

void ReceiveDocument::setInputDocument( const XMLDocument &document )
{
  mInputDocument = document;
}

XMLDocument ReceiveDocument::inputDocument() const
{
  return mInputDocument;
}

void SendDocument::setOutputDocument( const XMLDocument &document )
{
  mOutputDocument = document;
}

XMLDocument SendDocument::outputDocument() const
{
  return mOutputDocument;
}

