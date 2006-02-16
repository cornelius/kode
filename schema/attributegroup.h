/*
    This file is part of KDE Schema Parser.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef ATTRIBUTEGROUP_H
#define ATTRIBUTEGROUP_H

#include <schema/xmlelement.h>
#include <schema/attribute.h>
#include <common/qname.h>

namespace Schema {

class AttributeGroup : public XmlElement
{
  public:
    typedef QList<AttributeGroup> List;
    
    AttributeGroup();
    
    void setReference( const QName &reference );
    QName reference() const;
    
    void setAttributes( const Attribute::List &attributes );
    Attribute::List attributes() const;

    bool isResolved() const;

  private:
    QName mReference;
    
    Attribute::List mAttributes;
};

}

#endif
