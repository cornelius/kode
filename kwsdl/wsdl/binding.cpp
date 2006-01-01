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

#include <common/messagehandler.h>
#include <common/parsercontext.h>

#include "binding.h"

using namespace KWSDL;

Binding::Binding()
  : mType( SOAPBinding )
{
}

Binding::Binding( const QString &nameSpace )
  : Element( nameSpace ), mType( SOAPBinding )
{
}

Binding::~Binding()
{
}

void Binding::loadXML( ParserContext *context, const QDomElement &element )
{
  mName = element.attribute( "name" );
  if ( mName.isEmpty() )
    context->messageHandler()->warning( "Binding: 'name' required" );

  mPortTypeName = element.attribute( "type" );
  if ( mPortTypeName.isEmpty() )
    context->messageHandler()->warning( "Binding: 'type' required" );
  else
    if ( mPortTypeName.nameSpace().isEmpty() )
      mPortTypeName.setNameSpace( nameSpace() );

  mSoapBinding.parseBinding( context, element );

  QDomElement child = element.firstChildElement();
  while ( !child.isNull() ) {
    QName tagName = child.tagName();
    if ( tagName.localName() == "operation" ) {
      BindingOperation operation( nameSpace() );
      operation.loadXML( &mSoapBinding, context, child );
      mOperations.append( operation );
    }

    child = child.nextSiblingElement();
  }
}

void Binding::saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  QDomElement element = document.createElement( "binding" );
  parent.appendChild( element );

  if ( !mName.isEmpty() )
    element.setAttribute( "name", mName );
  else
    context->messageHandler()->warning( "Binding: 'name' required" );

  if ( !mPortTypeName.isEmpty() )
    element.setAttribute( "type", mPortTypeName.localName() );
  else
    context->messageHandler()->warning( "Binding: 'type' required" );

  mSoapBinding.synthesizeBinding( context, document, element );

  BindingOperation::List::ConstIterator it( mOperations.begin() );
  const BindingOperation::List::ConstIterator endIt( mOperations.end() );
  for ( ; it != endIt; ++it )
    (*it).saveXML( &mSoapBinding, context, document, element );
}

void Binding::setName( const QString &name )
{
  mName = name;
}

QString Binding::name() const
{
  return mName;
}

void Binding::setPortTypeName( const QName &portTypeName )
{
  mPortTypeName = portTypeName;
}

QName Binding::portTypeName() const
{
  return mPortTypeName;
}

void Binding::setOperations( const BindingOperation::List &operations )
{
  mOperations = operations;
}

BindingOperation::List Binding::operations() const
{
  return mOperations;
}

void Binding::setType( Type type )
{
  mType = type;
}

Binding::Type Binding::type() const
{
  return mType;
}

void Binding::setSoapBinding( const SoapBinding &soapBinding )
{
  mSoapBinding = soapBinding;
}

SoapBinding Binding::soapBinding() const
{
  return mSoapBinding;
}

const AbstractBinding *Binding::binding() const
{
  if ( mType == SOAPBinding )
    return &mSoapBinding;
  else
    return 0;
}
