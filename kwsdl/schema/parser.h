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

#include <qdom.h>
#include <qvaluelist.h>

#include "types.h"
#include "typestable.h"

namespace Schema {

class Parser
{
  public:
    Parser( const QString &nameSpace = QString() );

    ~Parser();

    Types types() const;

    void clear();
    void setSchemaBaseUrl( const QString& );

    void parseNameSpace( const QDomElement &element );
    bool parseSchemaTag( const QDomElement &element );

    const XSDType *type( const QualifiedName &type );
    const XSDType *type( int id ) const;
    int numTypes() const;

    QString typeName( int id ) const;

    Element *element( const QualifiedName &element ) const;
    Element *element( int id ) const;
    Element::PtrList elements() const;
    int numElements() const;

    Attribute *attribute( const QualifiedName &attribute ) const;
    Attribute *attribute( int id ) const;
    int numAttributes() const;

    QString targetNamespace() const;

    int typeId( const QualifiedName &name, bool create = false );

    bool isBasicType( int sType ) const;

    bool finalize();

    int elementId( const QualifiedName &type );
    int elementType( const QualifiedName &type );
    int attributeId( const QualifiedName &type ) const;
    int attributeType( const QualifiedName &type );

  private:
    void parseImport( const QDomElement& );
    void parseElement( const QDomElement& );
    void parseAttribute( const QDomElement& );

    void parseAnnotation( const QDomElement& );
    void parseAnnotation( const QDomElement&, QString& );
    void parseAnnotation( const QDomElement&, ComplexType* );
    void parseAnnotation( const QDomElement&, SimpleType* );
    XSDType *parseComplexType( const QDomElement& );

    void all( const QDomElement&, ComplexType* );
    void cs( const QDomElement&, ComplexType* );

    void addElement( const QDomElement&, ComplexType* );

    void addAttribute( const QDomElement&, ComplexType* );
    void addAny( const QDomElement&, ComplexType* );
    void addAnyAttribute( const QDomElement&, ComplexType* );
    int addExternalElement( const QString&, int );

    XSDType *parseSimpleType( const QDomElement& );
    void parseRestriction( const QDomElement&, SimpleType* );
    void parseComplexContent( const QDomElement&, ComplexType* );
    void parseSimpleContent( const QDomElement&, ComplexType* );


    void resolveForwardElementRefs();
    void resolveForwardAttributeRefs();
    void resolveForwardDerivations();
    bool shouldResolve();

    void importSchema( const QString &location );

    bool mElementQualified;
    bool mAttributeQualified;
    QMap<QString, QString> mNameSpaceMap;
    QString mNameSpace;
    QString mPrefix;
  
    TypesTable mTypesTable;
    Element::PtrList mElements;
    Attribute::PtrList mAttributes;

    QualifiedName::List mForwardElementRef;
    QualifiedName::List mForwardAttributeRef;

    typedef struct {
      QualifiedName type;
      QualifiedName baseType;
      ComplexType::Derivation derivation;
    } ForwardDerivation;

    QValueList<ForwardDerivation> mForwardDerivations;
    QStringList mImportedSchemas;
    QString mSchemaBaseUrl;
};

}

#endif


