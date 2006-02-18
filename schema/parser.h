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

#ifndef SCHEMA_PARSER_H
#define SCHEMA_PARSER_H

#include <QDomElement>
#include <QList>

#include "types.h"
#include "typestable.h"

class ParserContext;

namespace XSD {

class Parser
{
  public:
    Parser( const QString &nameSpace = QString() );
    ~Parser();

    Types types() const;

    void clear();

    bool parseSchemaTag( ParserContext *context, const QDomElement &element );

    QString targetNamespace() const;

    /**
      Returns the default schema URI.
     */
    static QString schemaUri();

  private:
    void parseImport( ParserContext *context, const QDomElement& );
    void parseElement( ParserContext *context, const QDomElement& );
    Attribute parseAttribute( ParserContext *context, const QDomElement& );
    AttributeGroup parseAttributeGroup( ParserContext *context, const QDomElement& );

    void parseAnnotation( ParserContext *context, const QDomElement& );
    void parseAnnotation( ParserContext *context, const QDomElement&, QString& );
    void parseAnnotation( ParserContext *context, const QDomElement&, ComplexType& );
    void parseAnnotation( ParserContext *context, const QDomElement&, SimpleType& );
    ComplexType parseComplexType( ParserContext *context, const QDomElement& );

    void all( ParserContext *context, const QDomElement&, ComplexType& );
    void cs( ParserContext *context, const QDomElement&, ComplexType& );

    void addElement( ParserContext *context, const QDomElement&, ComplexType& );

    void addAttribute( ParserContext *context, const QDomElement&, ComplexType& );
    void addAny( ParserContext *context, const QDomElement&, ComplexType& );
    void addAnyAttribute( ParserContext *context, const QDomElement&, ComplexType& );

    SimpleType parseSimpleType( ParserContext *context, const QDomElement& );
    void parseRestriction( ParserContext *context, const QDomElement&, SimpleType& );
    void parseComplexContent( ParserContext *context, const QDomElement&, ComplexType& );
    void parseSimpleContent( ParserContext *context, const QDomElement&, ComplexType& );


    void importSchema( ParserContext *context, const QString &location );
    QStringList joinNamespaces( const QStringList&, const QStringList& );

    Element findElement( const QName &name );
    Attribute findAttribute( const QName &name );
    AttributeGroup findAttributeGroup( const QName &name );
    void resolveForwardDeclarations();

    QString mNameSpace;

    SimpleType::List mSimpleTypes;
    ComplexType::List mComplexTypes;
    Element::List mElements;
    Attribute::List mAttributes;
    AttributeGroup::List mAttributeGroups;

    QStringList mImportedSchemas;
    QStringList mNamespaces;
};

}

#endif


