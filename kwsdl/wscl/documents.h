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

#ifndef WSCL_DOCUMENTS_H
#define WSCL_DOCUMENTS_H

#include <qstring.h>
#include <qvaluelist.h>

namespace WSCL {

class XMLDocument
{
  public:
    typedef QValueList<XMLDocument> List;

    void setId( const QString &id );
    QString id() const;

    void setSchema( const QString &schema );
    QString schema() const;

  private:
    QString mId;
    QString mSchema;
};

class ReceiveSendDocument
{
  public:
    void setInputDocument( const XMLDocument &document );
    XMLDocument inputDocument() const;

    void setOutputDocuments( const XMLDocument::List &documents );
    XMLDocument::List outputDocuments() const;

  private:
    XMLDocument mInputDocument;
    XMLDocument::List mOutputDocuments;
};

class SendReceiveDocument
{
  public:
    void setInputDocuments( const XMLDocument::List &documents );
    XMLDocument::List inputDocuments() const;

    void setOutputDocument( const XMLDocument &document );
    XMLDocument outputDocument() const;

  private:
    XMLDocument::List mInputDocuments;
    XMLDocument mOutputDocument;
};

class ReceiveDocument
{
  public:
    void setInputDocument( const XMLDocument &document );
    XMLDocument inputDocument() const;
  private:
    XMLDocument mInputDocument;
};

class SendDocument
{
  public:
    void setOutputDocument( const XMLDocument &document );
    XMLDocument outputDocument() const;

  private:
    XMLDocument mOutputDocument;
};

}

#endif
