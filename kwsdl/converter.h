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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#ifndef KWSDL_CONVERTER_H
#define KWSDL_CONVERTER_H

#include <kode/class.h>
#include <schema/parser.h>

#include "binding.h"
#include "message.h"
#include "namemapper.h"
#include "port.h"
#include "service.h"
#include "typemapper.h"

namespace KWSDL {

class Converter
{
  public:
    Converter();

    void setBindings( const Binding::List &bindings );
    void setMessages( const Message::List &messages );
    void setPorts( const Port::List &ports );
    void setService( const Service &service );
    void setTypes( const Schema::XSDType::List &types );
    void setParser( const Schema::Parser *parser );

    void convert();

    KODE::Class::List classes() const;

  private:
    void convertTypes( const Schema::XSDType::List& );
    void convertSimpleType( const Schema::SimpleType* );
    void convertComplexType( const Schema::ComplexType* );

    void createSimpleTypeSerializer( const Schema::SimpleType* );
    void createComplexTypeSerializer( const Schema::ComplexType* );

    void convertService( const Service& );
    void convertInputMessage( const Port&, const Message&, KODE::Class& );
    void convertOutputMessage( const Port&, const Message&, KODE::Class& );
    void createUtilClasses();
    void createTransportClass();

    Message findMessage( const QString& ) const;
    Port findPort( const QString& ) const;
    Binding findBinding( const QString& ) const;
    Binding::Operation findBindingOperation( const QString&, const QString& ) const;

    Binding::List mBindings;
    Message::List mMessages;
    Port::List mPorts;
    Service mService;
    Schema::XSDType::List mTypes;
    const Schema::Parser *mParser;

    KODE::Class::List mClasses;
    KODE::Class mSerializer;
    KODE::Class mQObject;

    NameMapper mNameMapper;
    TypeMapper mTypeMapper;
};

}

#endif
