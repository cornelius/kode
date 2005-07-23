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

#include "wsdl.h"

using namespace KWSDL;

void WSDL::setBindings( const Binding::List &bindings )
{
  mBindings = bindings;
}

Binding::List WSDL::bindings() const
{
  return mBindings;
}

void WSDL::setMessages( const Message::List &messages )
{
  mMessages = messages;
}

Message::List WSDL::messages() const
{
  return mMessages;
}

void WSDL::setPorts( const Port::List &ports )
{
  mPorts = ports;
}

Port::List WSDL::ports() const
{
  return mPorts;
}

void WSDL::setService( const Service &service )
{
  mService = service;
}

Service WSDL::service() const
{
  return mService;
}

void WSDL::setTypes( const Schema::Types &types )
{
  mTypes = types;
}

Schema::Types WSDL::types() const
{
  return mTypes;
}

Message WSDL::findMessage( const QString &name ) const
{
  Message::List::ConstIterator it;
  for ( it = mMessages.begin(); it != mMessages.end(); ++it ) {
    if ( (*it).name() == name ) {
      return *it;
    }
  }

  return Message();
}

Message WSDL::findOutputMessage( const QString &name ) const
{
  Port::List::ConstIterator it;
  for ( it = mPorts.begin(); it != mPorts.end(); ++it ) {
    Port::Operation::List operations = (*it).operations();
    Port::Operation::List::ConstIterator opIt;
    for ( opIt = operations.begin(); opIt != operations.end(); ++opIt ) {
      if ( (*opIt).input() == name )
        return findMessage( (*opIt).output() );
    }
  }

  return Message();
}

Port WSDL::findPort( const QString &name ) const
{
  Port::List::ConstIterator it;
  for ( it = mPorts.begin(); it != mPorts.end(); ++it ) {
    if ( (*it).name() == name )
      return *it;
  }

  return Port();
}

Binding WSDL::findBinding( const QString &name ) const
{
  Binding::List::ConstIterator it;
  for ( it = mBindings.begin(); it != mBindings.end(); ++it ) {
    if ( (*it).name() == name )
      return *it;
  }

  return Binding();
}

Binding::Operation WSDL::findBindingOperation( const QString &portName, const QString &operationName ) const
{
  Binding::List::ConstIterator it;
  for ( it = mBindings.begin(); it != mBindings.end(); ++it ) {
    if ( (*it).type() == portName ) {
      return (*it).operation( operationName );
    }
  }

  return Binding::Operation();
}
