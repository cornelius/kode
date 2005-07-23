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

#include <kode/class.h>
#include <schema/parser.h>

/*
#include "binding.h"
#include "message.h"
#include "port.h"
#include "service.h"
*/

#include "namemapper.h"
#include "typemapper.h"
#include "wsdl.h"

namespace KWSDL {

class Converter
{
  public:
    Converter();

    void setWSDL( const WSDL &wsdl );

    void convert();

    KODE::Class::List classes() const;

  private:
    void convertTypes( const Schema::Types& );
    void convertSimpleType( const Schema::SimpleType* );
    void convertComplexType( const Schema::ComplexType* );

    void createSimpleTypeSerializer( const Schema::SimpleType* );
    void createComplexTypeSerializer( const Schema::ComplexType* );

    void convertService( const Service& );
    void convertInputMessage( const Port&, const Message&, KODE::Class& );
    void convertOutputMessage( const Port&, const Message&, KODE::Class& );
    void createUtilClasses();
    void createTransportClass();

    WSDL mWSDL;

    KODE::Class::List mClasses;
    KODE::Class mSerializer;
    KODE::Class mQObject;

    NameMapper mNameMapper;
    TypeMapper mTypeMapper;
};

}

#endif
