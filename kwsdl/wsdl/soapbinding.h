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

#ifndef KWSDL_SOAPBINDING_H
#define KWSDL_SOAPBINDING_H

#include <QDomElement>
#include <QMap>
#include <QUrl>

#include <common/qname.h>
#include <wsdl/abstractbinding.h>

namespace KWSDL {

class SoapBinding : public AbstractBinding
{
  public:
    enum Style
    {
      RPCStyle,
      DocumentStyle
    };

    enum Use
    {
      LiteralUse,
      EncodedUse
    };

    enum Transport
    {
      HTTPTransport
    };

    class Binding
    {
      public:
        Binding();
        ~Binding();

        void setTransport( Transport transport );
        Transport transport() const;

        void setStyle( Style style );
        Style style() const;

        void loadXML( ParserContext *context, const QDomElement &element );
        void saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;

      private:
        Transport mTransport;
        Style mStyle;
    };


    class Body
    {
      public:
        Body();
        ~Body();

        void setEncodingStyle( const QString &encodingStyle );
        QString encodingStyle() const;

        void setPart( const QString &part );
        QString part() const;

        void setUse( Use use );
        Use use() const;

        void setNameSpace( const QString &nameSpace );
        QString nameSpace() const;

        void loadXML( ParserContext *context, const QDomElement &element );
        void saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;

      private:
        QString mEncodingStyle;
        QString mPart;
        Use mUse;
        QString mNameSpace;
    };

    class Fault
    {
      public:
        Fault();
        ~Fault();

        void setEncodingStyle( const QString &encodingStyle );
        QString encodingStyle() const;

        void setUse( Use use );
        Use use() const;

        void setNameSpace( const QString &nameSpace );
        QString nameSpace() const;

        void loadXML( ParserContext *context, const QDomElement &element );
        void saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;

      private:
        QString mEncodingStyle;
        Use mUse;
        QString mNameSpace;
    };

    class HeaderFault
    {
      public:
        HeaderFault();
        ~HeaderFault();

        void loadXML( ParserContext *context, const QDomElement &element );
        void saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;

        void setMessage( const QName &message );
        QName message() const;

        void setEncodingStyle( const QString &encodingStyle );
        QString encodingStyle() const;

        void setPart( const QString &part );
        QString part() const;

        void setUse( Use use );
        Use use() const;

        void setNameSpace( const QString &nameSpace );
        QString nameSpace() const;

      private:
        QName mMessage;
        QString mPart;
        Use mUse;
        QString mEncodingStyle;
        QString mNameSpace;
    };

    class Header
    {
      public:
        Header();
        ~Header();

        void loadXML( ParserContext *context, const QDomElement &element );
        void saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;

        void setHeaderFault( const HeaderFault &headerFault );
        HeaderFault headerFault() const;

        void setMessage( const QName &message );
        QName message() const;

        void setPart( const QString &part );
        QString part() const;

        void setUse( const Use &use );
        Use use() const;

        void setEncodingStyle( const QString encodingStyle );
        QString encodingStyle() const;

        void setNameSpace( const QString &nameSpace );
        QString nameSpace() const;

      private:
        HeaderFault mHeaderFault;
        QName mMessage;
        QString mPart;
        Use mUse;
        QString mEncodingStyle;
        QString mNameSpace;
    };

    class Operation
    {
      public:
        typedef QList<Operation> List;
        typedef QMap<QString, Operation> Map;

        Operation();
        Operation( const QString &name );
        ~Operation();

        void loadXML( ParserContext *context, const QDomElement &element );
        void saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;

        void setName( const QString &name );
        QString name() const;

        void setAction( const QString &action );
        QString action() const;

        void setStyle( Style style );
        Style style() const;

        void setInput( const Body &input );
        Body input() const;

        void setOutput( const Body &output );
        Body output() const;

        void setInputHeader( const Header &inputHeader );
        Header inputHeader() const;

        void setOutputHeader( const Header &outputHeader );
        Header outputHeader() const;

        void setFault( const Fault &fault );
        Fault fault() const;

      private:
        QString mName;
        QString mSoapAction;
        Style mStyle;
        Body mInputBody;
        Header mInputHeader;
        Body mOutputBody;
        Header mOutputHeader;
        Fault mFault;
    };

    class Address
    {
      public:
        Address();
        ~Address();

        void setLocation( const QUrl &location );
        QUrl location() const;

        void loadXML( ParserContext *context, const QDomElement &element );
        void saveXML( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;

      private:
        QUrl mLocation;
    };

    SoapBinding();
    ~SoapBinding();

    void setAddress( const Address &address );
    Address address() const;

    void setOperations( const Operation::Map &operations );
    Operation::Map operations() const;

    void setBinding( const Binding &binding );
    Binding binding() const;

    void parseBinding( ParserContext *context, const QDomElement &parent );
    void parseOperation( ParserContext *context, const QString &name, const QDomElement &parent );
    void parseOperationInput( ParserContext *context, const QString &name, const QDomElement &parent );
    void parseOperationOutput( ParserContext *context, const QString &name, const QDomElement &parent );
    void parseOperationFault( ParserContext *context, const QString &name, const QDomElement &parent );
    void parsePort( ParserContext *context, const QDomElement &parent );

    void synthesizeBinding( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;
    void synthesizeOperation( ParserContext *context, const QString &name, QDomDocument &document, QDomElement &parent ) const;
    void synthesizeOperationInput( ParserContext *context, const QString &name, QDomDocument &document, QDomElement &parent ) const;
    void synthesizeOperationOutput( ParserContext *context, const QString &name, QDomDocument &document, QDomElement &parent ) const;
    void synthesizeOperationFault( ParserContext *context, const QString &name, QDomDocument &document, QDomElement &parent ) const;
    void synthesizePort( ParserContext *context, QDomDocument &document, QDomElement &parent ) const;

  private:
    Binding mBinding;
    Operation::Map mOperations;
    Address mAddress;
};

}

#endif // KWSDL_SOAPBINDING_H

