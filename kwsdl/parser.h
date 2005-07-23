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

#ifndef KWSDL_PARSER_H
#define KWSDL_PARSER_H

#include <qdom.h>
#include <qvaluelist.h>

#include <schema/parser.h>

#include "wsdl.h"

namespace KWSDL {

class Parser
{
  public:
    Parser();

    void parse( const QDomElement &element );
    void reset();

    void setSchemaBaseUrl( const QString& );

    WSDL wsdl() const;

  private:
    void parseTypes( const QDomElement& );
    void parseMessage( const QDomElement&, Message& );
    void parsePortType( const QDomElement&, Port& );
    void parseBinding( const QDomElement& );
    void parseService( const QDomElement& );

    void parseSchema( const QDomElement& );

    Binding::List mBindings;
    Message::List mMessages;
    Port::List mPorts;
    Service mService;
    Schema::Parser mParser;
};

}

#endif
