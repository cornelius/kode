/*
    This file is part of KDE.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <common/messagehandler.h>
#include <common/nsmanager.h>
#include <common/parsercontext.h>

#include "soapbinding.h"

using namespace KWSDL;

// TODO: set namespace correctly

static QString soapBindingTransportHTTP = "http://schemas.xmlsoap.org/soap/http";
static QString soapStandardNamespace = "http://schemas.xmlsoap.org/wsdl/soap/";
static QString soapEncStandardNamespace = "http://schemas.xmlsoap.org/soap/encoding/";

static QString soapPrefix( ParserContext *context )
{
  QString prefix = context->namespaceManager()->prefix( soapStandardNamespace );
  if ( !prefix.isEmpty() )
    return prefix + ":";
  else
    return QString();
}

SoapBinding::Binding::Binding()
  : mTransport( HTTPTransport ),
    mStyle( DocumentStyle )
{
}

SoapBinding::Binding::~Binding()
{
}

void SoapBinding::Binding::setTransport( Transport transport )
{
  mTransport = transport;
}

SoapBinding::Transport SoapBinding::Binding::transport() const
{
  return mTransport;
}

void SoapBinding::Binding::setStyle( Style style )
{
  mStyle = style;
}

SoapBinding::Style SoapBinding::Binding::style() const
{
  return mStyle;
}

void SoapBinding::Binding::loadXML( ParserContext *context, const QDomElement &element )
{
  if ( element.hasAttribute( "transport" ) ) {
    if ( element.attribute( "transport" ) == soapBindingTransportHTTP ) {
      mTransport = HTTPTransport;
    } else
      context->messageHandler()->warning( "SoapBinding::Binding: unknown 'transport' value" );
  }

  if ( element.hasAttribute( "style" ) ) {
    if ( element.attribute( "style" ) == "rpc" )
      mStyle = RPCStyle;
    else if ( element.attribute( "style" ) == "document" )
      mStyle = DocumentStyle;
    else
      context->messageHandler()->warning( "SoapBinding::Binding: unknown 'style' value" );
  }
}

void SoapBinding::Binding::saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  QDomElement element = document.createElementNS( soapStandardNamespace, soapPrefix( context ) + "binding" );
  parent.appendChild( element );

  if ( mTransport == HTTPTransport )
    element.setAttribute( "transport", soapBindingTransportHTTP );
  else
    context->messageHandler()->warning( "SoapBinding::Binding: unknown 'transport' value" );

  if ( mStyle == RPCStyle )
    element.setAttribute( "style", "rpc" );
  else
    element.setAttribute( "style", "document" );
}


SoapBinding::Operation::Operation()
  : mStyle( DocumentStyle )
{
}

SoapBinding::Operation::Operation( const QString &name )
  : mName( name ), mStyle( DocumentStyle )
{
}

SoapBinding::Operation::~Operation()
{
}

void SoapBinding::Operation::loadXML( ParserContext *context, const QDomElement &element )
{
  mSoapAction = element.attribute( "soapAction" );

  if ( element.hasAttribute( "style" ) ) {
    if ( element.attribute( "style" ) == "rpc" )
      mStyle = RPCStyle;
    else if ( element.attribute( "style" ) == "document" )
      mStyle = DocumentStyle;
    else
      context->messageHandler()->warning( "SoapBinding::Operation: unknown 'style' value" );
  }

  // TODO: fallback is style value of soap:binding
}

void SoapBinding::Operation::saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  QDomElement element = document.createElementNS( soapStandardNamespace, soapPrefix( context ) + "operation" );
  parent.appendChild( element );

  if ( !mSoapAction.isEmpty() )
    element.setAttribute( "soapAction", mSoapAction );

  if ( mStyle == RPCStyle )
    element.setAttribute( "style", "rpc" );
  else
    element.setAttribute( "style", "document" );
}

void SoapBinding::Operation::setName( const QString &name )
{
  mName = name;
}

QString SoapBinding::Operation::name() const
{
  return mName;
}

void SoapBinding::Operation::setAction( const QString &action )
{
  mSoapAction = action;
}

QString SoapBinding::Operation::action() const
{
  return mSoapAction;
}

void SoapBinding::Operation::setStyle( Style style )
{
  mStyle = style;
}

SoapBinding::Style SoapBinding::Operation::style() const
{
  return mStyle;
}

void SoapBinding::Operation::setInput( const Body &input )
{
  mInputBody = input;
}

SoapBinding::Body SoapBinding::Operation::input() const
{
  return mInputBody;
}

void SoapBinding::Operation::setOutput( const Body &output )
{
  mOutputBody = output;
}

SoapBinding::Body SoapBinding::Operation::output() const
{
  return mOutputBody;
}

void SoapBinding::Operation::setInputHeader( const Header &inputHeader )
{
  mInputHeader = inputHeader;
}

SoapBinding::Header SoapBinding::Operation::inputHeader() const
{
  return mInputHeader;
}

void SoapBinding::Operation::setOutputHeader( const Header &outputHeader )
{
  mOutputHeader = outputHeader;
}

SoapBinding::Header SoapBinding::Operation::outputHeader() const
{
  return mOutputHeader;
}

void SoapBinding::Operation::setFault( const Fault &fault )
{
  mFault = fault;
}

SoapBinding::Fault SoapBinding::Operation::fault() const
{
  return mFault;
}

SoapBinding::Body::Body()
  : mUse( LiteralUse )
{
}

SoapBinding::Body::~Body()
{
}

void SoapBinding::Body::setEncodingStyle( const QString &encodingStyle )
{
  mEncodingStyle = encodingStyle;
}

QString SoapBinding::Body::encodingStyle() const
{
  return mEncodingStyle;
}

void SoapBinding::Body::setPart( const QString &part )
{
  mPart = part;
}

QString SoapBinding::Body::part() const
{
  return mPart;
}

void SoapBinding::Body::setUse( Use use )
{
  mUse = use;
}

SoapBinding::Use SoapBinding::Body::use() const
{
  return mUse;
}

void SoapBinding::Body::setNameSpace( const QString &nameSpace )
{
  mNameSpace = nameSpace;
}

QString SoapBinding::Body::nameSpace() const
{
  return mNameSpace;
}

void SoapBinding::Body::loadXML( ParserContext *context, const QDomElement &element )
{
  if ( element.hasAttribute( "encodingStyle" ) )
    mEncodingStyle = element.attribute( "encodingStyle" );
  else
    mEncodingStyle = QString();

  mPart = element.attribute( "part" );
  mNameSpace = element.attribute( "namespace" );

  if ( element.hasAttribute( "use" ) ) {
    if ( element.attribute( "use" ) == "literal" )
      mUse = LiteralUse;
    else if ( element.attribute( "use" ) == "encoded" )
      mUse = EncodedUse;
    else
      context->messageHandler()->warning( "SoapBinding::Body: unknown 'use' value" );
  }
}

void SoapBinding::Body::saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  QDomElement element = document.createElementNS( soapStandardNamespace, soapPrefix( context ) + "body" );
  parent.appendChild( element );

  if ( !mEncodingStyle.isEmpty() )
    element.setAttribute( "encodingStyle", mEncodingStyle );
  if ( !mPart.isEmpty() )
    element.setAttribute( "part", mPart );
  if ( !mNameSpace.isEmpty() )
    element.setAttribute( "namespace", mNameSpace );

  if ( mUse == LiteralUse )
    element.setAttribute( "use", "literal" );
  else
    element.setAttribute( "use", "encoded" );
}


SoapBinding::Fault::Fault()
  : mUse( LiteralUse )
{
}

SoapBinding::Fault::~Fault()
{
}

void SoapBinding::Fault::setEncodingStyle( const QString &encodingStyle )
{
  mEncodingStyle = encodingStyle;
}

QString SoapBinding::Fault::encodingStyle() const
{
  return mEncodingStyle;
}

void SoapBinding::Fault::setUse( Use use )
{
  mUse = use;
}

SoapBinding::Use SoapBinding::Fault::use() const
{
  return mUse;
}

void SoapBinding::Fault::setNameSpace( const QString &nameSpace )
{
  mNameSpace = nameSpace;
}

QString SoapBinding::Fault::nameSpace() const
{
  return mNameSpace;
}
void SoapBinding::Fault::loadXML( ParserContext *context, const QDomElement &element )
{
  if ( element.hasAttribute( "encodingStyle" ) )
    mEncodingStyle = element.attribute( "encodingStyle" );
  else
    mEncodingStyle = QString();

  mNameSpace = element.attribute( "namespace" );

  if ( element.hasAttribute( "use" ) ) {
    if ( element.attribute( "use" ) == "literal" )
      mUse = LiteralUse;
    else if ( element.attribute( "use" ) == "encoded" )
      mUse = EncodedUse;
    else
      context->messageHandler()->warning( "SoapBinding::Fault: unknown 'use' value" );
  }
}

void SoapBinding::Fault::saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  QDomElement element = document.createElementNS( soapStandardNamespace, soapPrefix( context ) + "fault" );
  parent.appendChild( element );

  if ( !mEncodingStyle.isEmpty() )
    element.setAttribute( "encodingStyle", mEncodingStyle );
  if ( !mNameSpace.isEmpty() )
    element.setAttribute( "namespace", mNameSpace );

  if ( mUse == LiteralUse )
    element.setAttribute( "use", "literal" );
  else
    element.setAttribute( "use", "encoded" );
}


SoapBinding::Header::Header()
  : mUse( LiteralUse )
{
}

SoapBinding::Header::~Header()
{
}

void SoapBinding::Header::setHeaderFault( const HeaderFault &headerFault )
{
  mHeaderFault = headerFault;
}

SoapBinding::HeaderFault SoapBinding::Header::headerFault() const
{
  return mHeaderFault;
}

void SoapBinding::Header::setMessage( const QName &message )
{
  mMessage = message;
}

QName SoapBinding::Header::message() const
{
  return mMessage;
}

void SoapBinding::Header::setPart( const QString &part )
{
  mPart = part;
}

QString SoapBinding::Header::part() const
{
  return mPart;
}

void SoapBinding::Header::setUse( const Use &use )
{
  mUse = use;
}

SoapBinding::Use SoapBinding::Header::use() const
{
  return mUse;
}

void SoapBinding::Header::setEncodingStyle( const QString encodingStyle )
{
  mEncodingStyle = encodingStyle;
}

QString SoapBinding::Header::encodingStyle() const
{
  return mEncodingStyle;
}

void SoapBinding::Header::setNameSpace( const QString &nameSpace )
{
  mNameSpace = nameSpace;
}

QString SoapBinding::Header::nameSpace() const
{
  return mNameSpace;
}

void SoapBinding::Header::loadXML( ParserContext *context, const QDomElement &element )
{
  mMessage = element.attribute( "message" );
  if ( mMessage.isEmpty() )
    context->messageHandler()->warning( "SoapBinding::Header: 'message' required" );
  else {
    if ( !mMessage.prefix().isEmpty() )
      mMessage.setNameSpace( context->namespaceManager()->uri( mMessage.prefix() ) );
  }

  mPart = element.attribute( "part" );
  if ( mPart.isEmpty() )
    context->messageHandler()->warning( "SoapBinding::Header: 'part' required" );

  if ( element.attribute( "use" ) == "literal" )
    mUse = LiteralUse;
  else if ( element.attribute( "use" ) == "encoded" )
    mUse = EncodedUse;
  else if ( element.attribute( "use" ).isEmpty() )
    context->messageHandler()->warning( "SoapBinding::Header: 'use' required" );
  else
    context->messageHandler()->warning( "SoapBinding::Header: unknown 'use' value" );

  if ( element.hasAttribute( "encodingStyle" ) )
    mEncodingStyle = element.attribute( "encodingStyle" );
  else
    mEncodingStyle = QString();

  mNameSpace = element.attribute( "namespace" );

  QDomElement child = element.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == soapPrefix( context ) + "headerfault" ) {
      mHeaderFault.loadXML( context, child );
    } else {
      context->messageHandler()->warning( QString( "SoapBinding::Header: unknown tag %1" ).arg( child.tagName() ) );
    }

    child = child.nextSiblingElement();
  }
}

void SoapBinding::Header::saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  QDomElement element = document.createElementNS( soapStandardNamespace, soapPrefix( context ) + "header" );
  parent.appendChild( element );

  if ( !mMessage.isEmpty() )
    element.setAttribute( "message", mMessage.qname() );
  else
    context->messageHandler()->warning( "SoapBinding::Header: 'message' required" );

  if ( !mPart.isEmpty() )
    element.setAttribute( "part", mPart );
  else
    context->messageHandler()->warning( "SoapBinding::Header: 'part' required" );

  if ( !mEncodingStyle.isEmpty() )
    element.setAttribute( "encodingStyle", mEncodingStyle );
  if ( !mNameSpace.isEmpty() )
    element.setAttribute( "namespace", mNameSpace );

  if ( mUse == LiteralUse )
    element.setAttribute( "use", "literal" );
  else
    element.setAttribute( "use", "encoded" );

  mHeaderFault.saveXML( context, document, element );
}


SoapBinding::HeaderFault::HeaderFault()
  : mUse( LiteralUse )
{
}

SoapBinding::HeaderFault::~HeaderFault()
{
}

void SoapBinding::HeaderFault::setMessage( const QName &message )
{
  mMessage = message;
}

QName SoapBinding::HeaderFault::message() const
{
  return mMessage;
}

void SoapBinding::HeaderFault::setEncodingStyle( const QString &encodingStyle )
{
  mEncodingStyle = encodingStyle;
}

QString SoapBinding::HeaderFault::encodingStyle() const
{
  return mEncodingStyle;
}

void SoapBinding::HeaderFault::setPart( const QString &part )
{
  mPart = part;
}

QString SoapBinding::HeaderFault::part() const
{
  return mPart;
}

void SoapBinding::HeaderFault::setUse( Use use )
{
  mUse = use;
}

SoapBinding::Use SoapBinding::HeaderFault::use() const
{
  return mUse;
}

void SoapBinding::HeaderFault::setNameSpace( const QString &nameSpace )
{
  mNameSpace = nameSpace;
}

QString SoapBinding::HeaderFault::nameSpace() const
{
  return mNameSpace;
}

void SoapBinding::HeaderFault::loadXML( ParserContext *context, const QDomElement &element )
{
  mMessage = element.attribute( "message" );
  if ( mMessage.isEmpty() )
    context->messageHandler()->warning( "SoapBinding::HeaderFault: 'message' required" );

  mPart = element.attribute( "part" );
  if ( mPart.isEmpty() )
    context->messageHandler()->warning( "SoapBinding::HeaderFault: 'part' required" );

  if ( element.attribute( "use" ) == "literal" )
    mUse = LiteralUse;
  else if ( element.attribute( "use" ) == "encoded" )
    mUse = EncodedUse;
  else if ( element.attribute( "use" ).isEmpty() )
    context->messageHandler()->warning( "SoapBinding::HeaderFault: 'use' required" );
  else
    context->messageHandler()->warning( "SoapBinding::HeaderFault: unknown 'use' value" );

  if ( element.hasAttribute( "encodingStyle" ) )
    mEncodingStyle = element.attribute( "encodingStyle" );
  else
    mEncodingStyle = QString();

  mNameSpace = element.attribute( "namespace" );
}

void SoapBinding::HeaderFault::saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  QDomElement element = document.createElementNS( soapStandardNamespace, soapPrefix( context ) + "headerfault" );
  parent.appendChild( element );

  if ( !mMessage.isEmpty() )
    element.setAttribute( "message", mMessage.qname() );
  else
    context->messageHandler()->warning( "SoapBinding::HeaderFault: 'message' required" );

  if ( !mPart.isEmpty() )
    element.setAttribute( "part", mPart );
  else
    context->messageHandler()->warning( "SoapBinding::HeaderFault: 'part' required" );

  if ( !mEncodingStyle.isEmpty() )
    element.setAttribute( "encodingStyle", mEncodingStyle );
  if ( !mNameSpace.isEmpty() )
    element.setAttribute( "namespace", mNameSpace );

  if ( mUse == LiteralUse )
    element.setAttribute( "use", "literal" );
  else
    element.setAttribute( "use", "encoded" );
}


SoapBinding::Address::Address()
{
}

SoapBinding::Address::~Address()
{
}

void SoapBinding::Address::setLocation( const QUrl &location )
{
  mLocation = location;
}

QUrl SoapBinding::Address::location() const
{
  return mLocation;
}

void SoapBinding::setOperations( const Operation::Map &operations )
{
  mOperations = operations;
}

SoapBinding::Operation::Map SoapBinding::operations() const
{
  return mOperations;
}

void SoapBinding::Address::loadXML( ParserContext *context, const QDomElement &element )
{
  mLocation = element.attribute( "location" );
  if ( !mLocation.isValid() )
    context->messageHandler()->warning( "SoapBinding::Address: 'location' required" );
}

void SoapBinding::Address::saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  QDomElement element = document.createElementNS( soapStandardNamespace, soapPrefix( context ) + "address" );
  parent.appendChild( element );

  if ( mLocation.isValid() )
    element.setAttribute( "location", mLocation.toString() );
  else
    context->messageHandler()->warning( "SoapBinding::Address: 'location' required" );
}


SoapBinding::SoapBinding()
{
}

SoapBinding::~SoapBinding()
{
}

void SoapBinding::setAddress( const Address &address )
{
  mAddress = address;
}

SoapBinding::Address SoapBinding::address() const
{
  return mAddress;
}

void SoapBinding::setBinding( const Binding &binding )
{
  mBinding = binding;
}

SoapBinding::Binding SoapBinding::binding() const
{
  return mBinding;
}

void SoapBinding::parseBinding( ParserContext *context, const QDomElement &parent )
{
  QDomElement child = parent.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == soapPrefix( context ) + "binding" ) {
      mBinding.loadXML( context, child );
    }

    child = child.nextSiblingElement();
  }
}

void SoapBinding::parseOperation( ParserContext *context, const QString &name, const QDomElement &parent )
{
  QDomElement child = parent.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == soapPrefix( context ) + "operation" ) {
      Operation op( name );
      op.loadXML( context, child );
      mOperations.insert( name, op );
    }

    child = child.nextSiblingElement();
  }
}

void SoapBinding::parseOperationInput( ParserContext *context, const QString &name, const QDomElement &parent )
{
  QDomElement child = parent.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == soapPrefix( context ) + "body" ) {
      Operation &op = mOperations[ name ];
      Body inputBody;
      inputBody.loadXML( context, child );
      op.setInput( inputBody );
    } else if ( child.tagName() == soapPrefix( context ) + "header" ) {
      Operation &op = mOperations[ name ];
      Header inputHeader;
      inputHeader.loadXML( context, child );
      op.setInputHeader( inputHeader );
    }

    child = child.nextSiblingElement();
  }
}

void SoapBinding::parseOperationOutput( ParserContext *context, const QString &name, const QDomElement &parent )
{
  QDomElement child = parent.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == soapPrefix( context ) + "body" ) {
      Operation &op = mOperations[ name ];
      Body outputBody;
      outputBody.loadXML( context, child );
      op.setOutput( outputBody );
    } else if ( child.tagName() == soapPrefix( context ) + "header" ) {
      Operation &op = mOperations[ name ];
      Header outputHeader;
      outputHeader.loadXML( context, child );
      op.setOutputHeader( outputHeader );
    }

    child = child.nextSiblingElement();
  }
}

void SoapBinding::parseOperationFault( ParserContext *context, const QString &name, const QDomElement &parent )
{
  QDomElement child = parent.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == soapPrefix( context ) + "fault" ) {
      Operation &op = mOperations[ name ];
      Fault fault;
      fault.loadXML( context, child );
      op.setFault( fault );
    }

    child = child.nextSiblingElement();
  }
}

void SoapBinding::parsePort( ParserContext *context, const QDomElement &parent )
{
  QDomElement child = parent.firstChildElement();
  while ( !child.isNull() ) {
    if ( child.tagName() == soapPrefix( context ) + "address" ) {
      mAddress.loadXML( context, child );
    }

    child = child.nextSiblingElement();
  }
}

void SoapBinding::synthesizeBinding( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  mBinding.saveXML( context, document, parent );
}

void SoapBinding::synthesizeOperation( ParserContext *context, const QString &name, QDomDocument &document, QDomElement &parent ) const
{
  const Operation &op = mOperations[ name ];
  op.saveXML( context, document, parent );
}

void SoapBinding::synthesizeOperationInput( ParserContext *context, const QString &name, QDomDocument &document, QDomElement &parent ) const
{
  const Operation &op = mOperations[ name ];
  op.input().saveXML( context, document, parent );
  op.inputHeader().saveXML( context, document, parent );
}

void SoapBinding::synthesizeOperationOutput( ParserContext *context, const QString &name, QDomDocument &document, QDomElement &parent ) const
{
  const Operation &op = mOperations[ name ];
  op.output().saveXML( context, document, parent );
  op.outputHeader().saveXML( context, document, parent );
}

void SoapBinding::synthesizeOperationFault( ParserContext *context, const QString &name, QDomDocument &document, QDomElement &parent ) const
{
  const Operation &op = mOperations[ name ];
  op.fault().saveXML( context, document, parent );
}

void SoapBinding::synthesizePort( ParserContext *context, QDomDocument &document, QDomElement &parent ) const
{
  mAddress.saveXML( context, document, parent );
}
