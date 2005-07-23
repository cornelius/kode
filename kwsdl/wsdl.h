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

#ifndef KWSDL_WSDL_H
#define KWSDL_WSDL_H

#include <schema/types.h>

#include "binding.h"
#include "message.h"
#include "port.h"
#include "service.h"

namespace KWSDL {

class WSDL
{
  public:
    void setBindings( const Binding::List &bindings );
    Binding::List bindings() const;

    void setMessages( const Message::List &messages );
    Message::List messages() const;

    void setPorts( const Port::List &ports );
    Port::List ports() const;

    void setService( const Service &service );
    Service service() const;

    void setTypes( const Schema::Types &types );
    Schema::Types types() const;

    Message findMessage( const QString& ) const;
    Message findOutputMessage( const QString& ) const;
    Port findPort( const QString& ) const;
    Binding findBinding( const QString& ) const;
    Binding::Operation findBindingOperation( const QString&, const QString& ) const;

  private:
    Binding::List mBindings;
    Message::List mMessages;
    Port::List mPorts;
    Service mService;
    Schema::Types mTypes;
};

}

#endif
