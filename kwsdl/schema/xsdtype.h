/* 
    This file is part of KDE Schema Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>
                       based on wsdlpull parser by Vivek Krishna

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

#ifndef SCHEMA_XSDTYPE_H
#define SCHEMA_XSDTYPE_H

#include <qstring.h>
#include <qvaluelist.h>
#include <qmap.h>

#include "qualifiedname.h"

namespace Schema {

const QString SchemaUri = "http://www.w3.org/2001/XMLSchema";

class XSDType
{
  public:
    typedef QValueList<const XSDType*> List;

    enum
    {
      SIMPLE = 0,
      COMPLEX,
      MIXED
    };

    enum
    {
      INVALID = 0,
      STRING = 1,
      INTEGER,
      INT,
      BYTE,
      UBYTE,
      POSINT,
      UINT,
      LONG,
      ULONG,
      SHORT,
      USHORT,
      DECIMAL,
      FLOAT,
      DOUBLE,
      BOOLEAN,
      TIME,
      DATETIME,
      DATE,
      TOKEN,
      QNAME,
      NCNAME,
      NMTOKEN,
      NMTOKENS,
      BASE64BIN,
      HEXBIN,
      ANY,
      ANYTYPE,
      ANYURI
    };

    XSDType( const QString& )
    {
    };

    XSDType()
    {
    };

    virtual ~XSDType()
    {
    };

    virtual QString name() const = 0;
    virtual QualifiedName qualifiedName() const = 0;
    virtual QString documentation() const = 0;
    virtual int contentModel() const = 0;
    virtual int type() const = 0;
    virtual bool isAnonymous() const = 0;

    virtual bool isSimple() const
    {
      return true;
    }

    virtual void setName( const QString& ) = 0;
    virtual void setDocumentation( const QString& ) = 0;
    virtual void setContentModel( int ) = 0;
    virtual void setType( int ) = 0;
    virtual void setAnonymous( bool ) = 0;
};

}

#endif
