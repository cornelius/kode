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

#ifndef KWSDL_CONVERTER_H
#define KWSDL_CONVERTER_H

#include <common/nsmanager.h>
#include <kode/class.h>
#include <schema/parser.h>
#include <wsdl/wsdl.h>

#include "namemapper.h"
#include "typemap.h"

namespace KWSDL {

class Converter
{
  public:
    Converter();

    void setWSDL( const WSDL &wsdl );

    void convert();

    KODE::Class::List classes() const;

  private:
    void convertTypes();

    void convertComplexType( const Schema::ComplexType* );
    void createComplexTypeSerializer( const Schema::ComplexType* );

    void convertSimpleType( const Schema::SimpleType* );
    void createSimpleTypeSerializer( const Schema::SimpleType* );

    void convertAttribute( const Schema::Attribute* );
    void createAttributeSerializer( const Schema::Attribute* );

    void convertElement( const Schema::Element* );
    void createElementSerializer( const Schema::Element* );

    // Client Stub
    void convertClientService();
    void convertClientInputMessage( const Operation&, const Param&, const Binding&, KODE::Class& );
    void convertClientOutputMessage( const Operation&, const Param&, const Binding&, KODE::Class& );

    // Server Stub
    void convertServerService();

    // Transport
    void createKDETransport();
    void createQtTransport();

    // Utils
    void createUtils();
    void createSoapUtils();

    WSDL mWSDL;

    KODE::Class::List mClasses;
    KODE::Class mSerializer;
    KODE::Class mQObject;

    NameMapper mNameMapper;
    TypeMap mTypeMap;
    NSManager mNSManager;
};

}

QString upperlize( const QString& );
QString lowerlize( const QString& );

static QName XmlAnyType( "http://www.w3.org/2001/XMLSchema", "any" );

#endif
