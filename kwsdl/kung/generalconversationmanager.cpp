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

#include "generalconversationmanager.h"

GeneralConversationManager::GeneralConversationManager( const KWSDL::WSDL &wsdl )
  : mWSDL( wsdl )
{
  const KWSDL::Port::List servicePorts = mWSDL.definitions().service().ports();
  KWSDL::Port::List::ConstIterator it;
  for ( it = servicePorts.begin(); it != servicePorts.end(); ++it ) {
    KWSDL::Binding binding = mWSDL.findBinding( (*it).bindingName() );

    // KUNGPORT
    //KWSDL::Port port = mWSDL.findPort( binding.type() );
    //const KWSDL::Operation::List operations = port.operations();
    const KWSDL::Operation::List operations;
    KWSDL::Operation::List::ConstIterator opIt;
    for ( opIt = operations.begin(); opIt != operations.end(); ++opIt ) {
      mInputMessages.append( mWSDL.findMessage( (*opIt).input().message() ) );
      mOutputMessages.append( mWSDL.findMessage( (*opIt).output().message() ) );
    }
  }
}

QStringList GeneralConversationManager::nextActions( const QString&, const QString& )
{
  QStringList actions;

  KWSDL::Message::List::ConstIterator it;
  for ( it = mInputMessages.begin(); it != mInputMessages.end(); ++it )
    actions.append( (*it).name() );

  return actions;
}
