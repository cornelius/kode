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

#include <qdir.h>
#include <qfile.h>
#include <qurl.h>

#include "fileprovider.h"
#include "parser.h"

using namespace Schema;

const QString soapEncUri = "http://schemas.xmlsoap.org/soap/encoding/";
const QString wsdlUri = "http://schemas.xmlsoap.org/wsdl/";


Parser::Parser( const QString &nameSpace )
  : mNameSpace( nameSpace )
{
  mElementQualified = false;
  mAttributeQualified = false;
}

Parser::~Parser()
{
  clear();
}

Types Parser::types() const
{
  Types types;

  SimpleType::List simpleTypes;
  ComplexType::List complexTypes;

  for ( int i = 0; i < numTypes(); i++ ) {
    const XSDType *pType = type( i + XSDType::ANYURI + 1 );
    if ( pType != 0 ) {
      if ( pType->isSimple() ) {
        const SimpleType *simpleType = static_cast<const SimpleType*>( pType );

        SimpleType type = *simpleType;
        type.setBaseTypeName( mTypesTable.typeName( type.baseType() ) );
        type.setListTypeName( mTypesTable.typeName( type.listType() ) );

        simpleTypes.append( type );
      } else {
        const ComplexType *complexType = static_cast<const ComplexType*>( pType );

        ComplexType type = *complexType;
        type.setBaseTypeName( mTypesTable.typeName( type.baseType() ) );

        Schema::Element::List elements = type.elements();
        Schema::Element::List::Iterator elemIt;
        for ( elemIt = elements.begin(); elemIt != elements.end(); ++elemIt )
          (*elemIt).setTypeName( mTypesTable.typeName( (*elemIt).type() ) );
        type.setElements( elements );

        Schema::Attribute::List attributes = type.attributes();
        Schema::Attribute::List::Iterator attrIt;
        for ( attrIt = attributes.begin(); attrIt != attributes.end(); ++attrIt )
          (*attrIt).setTypeName( mTypesTable.typeName( (*attrIt).type() ) );
        type.setAttributes( attributes );

        complexTypes.append( type );
      }
    }
  }

  Element::List elements;

  for ( uint i = 0; i < mElements.count(); ++i ) {
    Element element = *mElements[ i ];
    element.setTypeName( mTypesTable.typeName( element.type() ) );
    elements.append( element );
  }

  types.setSimpleTypes( simpleTypes );
  types.setComplexTypes( complexTypes );
  types.setElements( elements );

  return types;
}

void Parser::clear()
{
  mTypesTable.clear();
  mImportedSchemas.clear();

  for ( uint i = 0; i < mElements.count(); ++i )
    delete mElements[ i ];

  for ( uint i = 0; i < mAttributes.count(); ++i )
    delete mAttributes[ i ];
}

void Parser::setSchemaBaseUrl( const QString &url )
{
  mSchemaBaseUrl = url;
}

void Parser::parseNameSpace( const QDomElement &element )
{
  QDomNamedNodeMap attributes = element.attributes();
  for ( uint i = 0; i < attributes.count(); ++i ) {
    QDomNode node = attributes.item( i );
    QDomAttr attribute = node.toAttr();

    if ( attribute.name().startsWith( "xmlns:" ) )
      mNameSpaceMap.insert( attribute.value(), attribute.name().mid( 6 ) );
  }
}

bool Parser::parseSchemaTag( const QDomElement &root )
{
  QualifiedName name = root.tagName();
  if ( name.localName() != "schema" )
    return false;

  if ( root.hasAttribute( "targetNamespace" ) )
    mNameSpace = root.attribute( "targetNamespace" );

  if ( root.hasAttribute( "elementFormDefault" ) ) {
    const QString value = root.attribute( "elementFormDefault" );
    if ( value == "unqualified" )
      mElementQualified = false;
    else if ( value == "qualified" )
      mElementQualified = true;
  }

  mTypesTable.setTargetNamespace( mNameSpace );

/*
  for (i = xParser->getNamespaceCount(xParser->getDepth()) - 1;
       i > xParser->getNamespaceCount(xParser->getDepth() - 1) - 1; i--)
    if (xParser->getNamespaceUri(i) == mNameSpace)
      m_tnsPrefix = xParser->getNamespacePrefix(i);
*/

  QDomNode node = root.firstChild();
  while ( !node.isNull() ) {
    QDomElement element = node.toElement();
    if ( !element.isNull() ) {

      QualifiedName name = element.tagName();
      if ( name.localName() == "import" ) {
        parseImport( element );
      } else if ( name.localName() == "element" ) {
        parseElement( element );
      } else if ( name.localName() == "complexType" ) {
        XSDType *type = parseComplexType( element );
        mTypesTable.addType( type );
      } else if ( name.localName() == "simpleType" ) {
        XSDType *type = parseSimpleType( element );
        mTypesTable.addType( type );
      } else if ( name.localName() == "attribute" ) {
        parseAttribute( element );
      } else if ( name.localName() == "annotation" ) {
        parseAnnotation( element );
      } else if ( name.localName() == "import" ) {
        // TODO
      } else if ( name.localName() == "include" ) {
        // TODO
      }
    }

    node = node.nextSibling();
  }

  if ( shouldResolve() ) {
    resolveForwardElementRefs();
    resolveForwardAttributeRefs();
    resolveForwardDerivations();
  }

  return true;
}

void Parser::parseImport( const QDomElement &element )
{
  QString location = element.attribute( "schemaLocation" );
  if ( !location.isEmpty() ) {
    // don't import a schema twice
    if ( mImportedSchemas.contains( location ) )
      return;
    else
      mImportedSchemas.append( location );

    importSchema( location );
  }
}

void Parser::parseAnnotation( const QDomElement& )
{
}

void Parser::parseAnnotation( const QDomElement &element, QString &documentation )
{
  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {
      QualifiedName name = childElement.tagName();
      if ( name.localName() == "documentation" )
        documentation = childElement.text().stripWhiteSpace();
    }

    node = node.nextSibling();
  }
}

void Parser::parseAnnotation( const QDomElement &element, ComplexType *complexType )
{
  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {
      QualifiedName name = childElement.tagName();
      if ( name.localName() == "documentation" )
        complexType->setDocumentation( childElement.text().stripWhiteSpace() );
    }

    node = node.nextSibling();
  }
}

void Parser::parseAnnotation( const QDomElement &element, SimpleType *simpleType )
{
  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {
      QualifiedName name = childElement.tagName();
      if ( name.localName() == "documentation" )
        simpleType->setDocumentation( childElement.text().stripWhiteSpace() );
    }

    node = node.nextSibling();
  }
}

XSDType *Parser::parseComplexType( const QDomElement &element )
{
  ComplexType *newType = new ComplexType( mNameSpace );

  newType->setName( element.attribute( "name" ) );
  if ( element.hasAttribute( "mixed" ) )
    newType->setContentModel( newType->MIXED );

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {

      QualifiedName name = childElement.tagName();
      if ( name.localName() == "all" ) {
        all( childElement, newType );
      } else if ( name.localName() == "sequence" ) {
        cs( childElement, newType );
      } else if ( name.localName() == "choice" ) {
        cs( childElement, newType );
      } else if ( name.localName() == "attribute" ) {
        addAttribute( childElement, newType );
      } else if ( name.localName() == "anyAttribute" ) {
        addAnyAttribute( childElement, newType );
      } else if ( name.localName() == "complexContent" ) {
        parseComplexContent( childElement, newType );
      } else if ( name.localName() == "simpleContent" ) {
        parseSimpleContent( childElement, newType );
      } else if ( name.localName() == "annotation" ) {
        parseAnnotation( childElement, newType );
      }
    }

    node = node.nextSibling();
  }
  
  return newType;
}

void Parser::all( const QDomElement &element, ComplexType * ct )
{
  int min, max;
  QString tmp;

  min = element.attribute( "minOccurs", "1" ).toInt();
  max = element.attribute( "maxOccurs", "1" ).toInt();
  ct->setCompositor( ct->ALL, true, min, max );

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {

      QualifiedName name = childElement.tagName();
      if ( name.localName() == "element" ) {
        addElement( childElement, ct );
      } else if ( name.localName() == "annotation" ) {
        parseAnnotation( childElement, ct );
      }
    }

    node = node.nextSibling();
  }

/*
  if (xParser->getName() == "all"
      && xParser->getEventType() == xParser->END_TAG)
    ct->setCompositor(ct->ALL, false);
*/

  return;
}


void Parser::cs( const QDomElement &element, ComplexType *ct )
{
  int min = 1, max = 1;

  QualifiedName name = element.tagName();
  if ( name.localName() == "choice" || name.localName() == "sequence" ) {
    min = element.attribute( "minOccurs", "1" ).toInt();
    QString value = element.attribute( "maxOccurs", "1" );
    if ( value == "unbounded" )
      max = UNBOUNDED;
    else
      max = value.toInt();

    if ( name.localName() == "choice" )
      ct->setCompositor( ct->CHOICE, true, min, max );
    else        
      ct->setCompositor( ct->SEQ, true, min, max );

    QDomNode node = element.firstChild();
    while ( !node.isNull() ) {
      QDomElement childElement = node.toElement();
      if ( !childElement.isNull() ) {
        QualifiedName csName = childElement.tagName();
        if ( csName.localName() == "element" )
          addElement( childElement, ct );
        else if ( csName.localName() == "any" )
          addAny( childElement, ct );
        else if ( csName.localName() == "choice" )
          cs( childElement, ct );
        else if ( csName.localName() == "sequence" )
          cs( childElement, ct );
      }          

      node = node.nextSibling();
    }

    if ( name.localName() == "choice")
      ct->setCompositor( ct->CHOICE, false );
    else
      ct->setCompositor( ct->SEQ, false );
  }

  return;
}

void Parser::addElement( const QDomElement &element, ComplexType *cType )
{
  QString name, fixedValue, defaultValue, documentation;
  QualifiedName refName;
  int type_id = 0, minOccurs = 1, maxOccurs = 1;
  bool qualified = false, added = false, nill = false;
  XSDType *elemType;

  name = element.attribute( "name" );
  QualifiedName typeName = element.attribute( "type" );
//  typeName.setNamespace(xParser->getNamespace(typeName.getPrefix()));
  type_id = typeId( typeName, true );

  if ( element.hasAttribute( "form" ) ) {
    if ( element.attribute( "form" ) == "qualified" )
      qualified = true;
    else if ( element.attribute( "form" ) == "unqualified" )
      qualified = false;
  }

  if ( element.hasAttribute( "ref" ) ) {
    refName = element.attribute( "ref" );
//    refName.setNamespace(xParser->getNamespace(refName.getPrefix()));

    Element *e = 0;
    if ( refName.nameSpace() == mNameSpace )
      e = this->element( elementId( refName ) );

    if ( e == 0 ) {
      added = true;
      mForwardElementRef.append( refName );
    } else {
      name = e->name();
      type_id = e->type();
      qualified = e->isQualified();
      defaultValue = e->defaultValue();
      fixedValue = e->fixedValue();
    }
  }

  minOccurs = element.attribute( "minOccurs", "1" ).toInt();
  QString value = element.attribute( "maxOccurs", "1" );
  if ( value == "unbounded" )
    maxOccurs = UNBOUNDED;
  else
    maxOccurs = value.toInt();

  defaultValue = element.attribute( "default" );
  fixedValue = element.attribute( "fixed" );

  if ( element.hasAttribute( "nillable" ) )
    nill = true;

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {

      QualifiedName childName = childElement.tagName();
      if ( childName.localName() == "complexType" ) {
        elemType = parseComplexType( childElement );

        // create an anonymous type
        ComplexType *ct = (ComplexType *) elemType;
        if ( ct->numElements() == 1
             && ct->elementType( 0 ) == XSDType::ANY
             && ct->elementName( 0 ) == "any" ) {

          // if the complex type is <any> then we dont need a type for it.
          // make the parent's type as XSDType::ANY
          delete ct;
          type_id = XSDType::ANY;
        } else {
          elemType->setName( name );
          type_id = mTypesTable.addType( elemType );
        }
      } else if ( childName.localName() == "simpleType" ) {
        elemType = parseSimpleType( childElement );

        //create an anonymous type
        type_id = mTypesTable.addType( elemType );
      } else if ( childName.localName() == "annotation" ) {
        parseAnnotation( childElement, documentation );
      }
    }

    node = node.nextSibling();
  }

  if ( nill && type_id == 0 )
    type_id = XSDType::ANYTYPE;

  if ( !added ) {
    cType->addElement( name, type_id, minOccurs, maxOccurs, qualified, defaultValue, fixedValue, documentation );
  } else {
    cType->addElementRef( refName, minOccurs, maxOccurs );
  }
}

void Parser::addAny( const QDomElement &element, ComplexType *cType )
{
  QString ns, any( "any" );
  int type_id = XSDType::ANY, min = 1, max = 1;

  ns = element.attribute( "namespace" );
  min = element.attribute( "minOccurs", "1" ).toInt();
  QString value = element.attribute( "maxOccurs", "1" );
  if ( value == "unbounded" )
    max = UNBOUNDED;
  else
    max = value.toInt();

  cType->addElement( any, type_id, min, max, false, ns );
}

void Parser::addAnyAttribute( const QDomElement &element, ComplexType *cType )
{
  QString ns, anyAttribute( "anyAttribute" );

  ns = element.attribute( "namespace" );

  cType->addAttribute( anyAttribute, XSDType::ANY, false, ns );
}

void Parser::addAttribute( const QDomElement &element, ComplexType *cType )
{
  QString name, fixedVal, defaultVal;
  int type_id = 0;
  bool qualified = false, use = false, added = false;
  QualifiedName refAttribute;

  name = element.attribute( "name" );

  if ( element.hasAttribute( "type" ) ) {
    QualifiedName typeName = element.attribute( "type" );
//    typeName.setNamespace(xParser->getNamespace(typeName.getPrefix()));
    type_id = typeId( typeName, true );
  }

  if ( element.hasAttribute( "form" ) ) {
    if ( element.attribute( "form" ) == "qualified" )
      qualified = true;
    else if ( element.attribute( "form" ) == "unqualified" )
      qualified = false;
  }

  if ( element.hasAttribute( "ref" ) ) {
    refAttribute = element.attribute( "ref" );
//    refAttribute.setNamespace(xParser->getNamespace(refAttribute.getPrefix()));

    Attribute *attribute = 0;
    if ( refAttribute.nameSpace() == mNameSpace )
      attribute = this->attribute( attributeId( refAttribute ) );
          
    if ( attribute == 0 ) {
      added = true;
      mForwardAttributeRef.append( refAttribute );
    } else {
      name = attribute->name();
      type_id = attribute->type();
      qualified = attribute->isQualified();
      defaultVal = attribute->defaultValue();
      fixedVal = attribute->fixedValue();
    }
  }

  defaultVal = element.attribute( "default" );
  fixedVal = element.attribute( "fixed" );

  if ( element.hasAttribute( "use" ) ) {
    if ( element.attribute( "use" ) == "optional" )
      use = false;
    else if ( element.attribute( "use" ) == "required" )
      use = true;
  }

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {

      QualifiedName childName = childElement.tagName();
      if ( childName.localName() == "simpleType" ) {
        XSDType *elemType = parseSimpleType( childElement );

        elemType->setName( name );
        type_id = mTypesTable.addType( elemType );
      } else if ( childName.localName() == "annotation" ) {
        // TKO: we have to pass it to the element here...
        parseAnnotation( childElement );
      }
    }

    node = node.nextSibling();
  }

  if ( !added )
    cType->addAttribute( name, type_id, qualified, defaultVal, fixedVal, use );
  else
    cType->addAttributeRef( refAttribute, qualified, use );
}

XSDType *Parser::parseSimpleType( const QDomElement &element )
{
  SimpleType *st = new SimpleType( mNameSpace );

  int basetype_id = XSDType::INVALID;

  st->setName( element.attribute( "name" ) );

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {

      QualifiedName name = childElement.tagName();
      if ( name.localName() == "restriction" ) {
        st->setSubType( SimpleType::TypeRestriction );

        QualifiedName typeName( childElement.attribute( "base" ) );
//        typeName.setNamespace(xParser->getNamespace(typeName.getPrefix()));
        st->setBaseType( basetype_id = typeId( typeName, true ) );

        parseRestriction( childElement, st );
      } else if ( name.localName() == "union" ) {
        st->setSubType( SimpleType::TypeUnion );
        qDebug( "simpletype::union not supported" );
      } else if ( name.localName() == "list" ) {
        st->setSubType( SimpleType::TypeList );
        if ( childElement.hasAttribute( "itemType" ) ) {
          QualifiedName typeName( childElement.attribute( "itemType" ) );
          int type = typeId( typeName, true );
          st->setListType( type );
        } else {
          // TODO: add support for anonymous types
        }
      } else if ( name.localName() == "annotation" ) {
        parseAnnotation( childElement, st );
      }
    }

    node = node.nextSibling();
  }

  return st;
}

void Parser::parseRestriction( const QDomElement &element, SimpleType *st )
{
  if ( st->baseType() == 0 )
    qDebug( "<restriction>:unkown BaseType" );

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {

      if ( !st->isValidFacet( childElement.tagName() ) ) {
        qDebug( "<restriction>: %s is not a valid facet for the simple type", childElement.tagName().latin1() );
        continue;
      }

      st->setFacetValue( childElement.attribute( "value" ) );
    }

    node = node.nextSibling();
  }
}

void Parser::parseComplexContent( const QDomElement &element, ComplexType *ct )
{
  QualifiedName typeName;

  if ( element.attribute( "mixed" ) == "true" ) {
    qDebug( "<complexContent>: No support for mixed=true" );
    return;
  }

  ct->setContentModel( ct->COMPLEX );

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement childElement = node.toElement();
    if ( !childElement.isNull() ) {
      QualifiedName name = childElement.tagName();

      if ( name.localName() == "restriction" || name.localName() == "extension" ) {
        typeName = childElement.attribute( "base" );
//        typeName.setNamespace(xParser->getNamespace(typeName.getPrefix()));

        ComplexType::Derivation type = ( name.localName() == "restriction" ? ComplexType::Restriction : ComplexType::Extension );
        if ( this->type( typeName ) != 0 ) { // type already known
          ct->setBaseType( typeId( typeName, true ), type, this->type( typeName ) );
        } else {
          ForwardDerivation entry;
          entry.type = ct->qualifiedName();
          entry.baseType = typeName;
          entry.derivation = type;
          mForwardDerivations.append( entry );
        }

        // if the base soapenc:Array, then read only the arrayType attribute and nothing else
        if ( typeName.localName() == "Array" ) {
          QDomElement arrayElement = childElement.firstChild().toElement();
          ct->setIsArray( true );

          QualifiedName arrayType( arrayElement.attribute( "arrayType" ) );
//          arrayType.setNamespace(xParser->getNamespace(arrayType.getPrefix()));
          ct->addElement( "item", typeId( arrayType, true ), 0, UNBOUNDED );
        } else {
          QDomNode childNode = childElement.firstChild();
          while ( !childNode.isNull() ) {
            QDomElement ctElement = childNode.toElement();
            if ( !ctElement.isNull() ) {
              QualifiedName name = ctElement.tagName();

              if ( name.localName() == "all" ) {
                all( ctElement, ct );
              } else if ( name.localName() == "sequence" ) {
                cs( ctElement, ct );
              } else if ( name.localName() == "choice" ) {
                cs( ctElement, ct );
              } else if ( name.localName() == "attribute" ) {
                addAttribute( ctElement, ct );
              } else if ( name.localName() == "anyAttribute" ) {
                addAnyAttribute( ctElement, ct );
              }
            }

            childNode = childNode.nextSibling();
          }
        }
      }
    }

    node = node.nextSibling();
  }
}

void Parser::parseSimpleContent( const QDomElement &element, ComplexType *ct )
{
  ct->setContentModel( ct->SIMPLE );

  const QDomElement childElement = element.firstChild().toElement();

  QualifiedName name = childElement.tagName();
  if ( name.localName() == "restriction" ) {
    SimpleType *st = new SimpleType( mNameSpace );

    if ( childElement.hasAttribute( "base" ) ) {
      int basetype_id = 0;

      QualifiedName typeName( childElement.attribute( "base" ) );
//      typeName.setNamespace(xParser->getNamespace(typeName.getPrefix()));
      st->setBaseType( basetype_id = typeId( typeName, true ) );
    }    

    parseRestriction( childElement, st );
    int typeId = mTypesTable.addType( st );
    if ( typeId == 0 ) {
      qDebug( "Could not add type in types table" );
      return;
    }

    ct->setContentType( typeId );
  } else if ( name.localName() == "extension" ) {
    // This extension does not use the full model that can come in ComplexContent.
    // It uses the simple model. No particle allowed, only attributes

    if ( childElement.hasAttribute( "base" ) ) {
      int basetype_id = 0;

      QualifiedName typeName( childElement.attribute( "base" ) );
//      typeName.setNamespace(xParser->getNamespace(typeName.getPrefix()));
      ct->setContentType( basetype_id = typeId( typeName, true ) );

      QDomNode childNode = childElement.firstChild();
      while ( !childNode.isNull() ) {
        QDomElement ctElement = childNode.toElement();
        if ( !ctElement.isNull() ) {
          QualifiedName name = ctElement.tagName();
          if ( name.localName() == "attribute" )
            addAttribute( ctElement, ct );
        }

        childNode = childNode.nextSibling();
      }
    }
  }
}

bool Parser::isBasicType( int type ) const
{
  if ( type > XSDType::ANYURI )
    return false;
  else
    return true;
}

void Parser::parseElement( const QDomElement &element )
{
  ComplexType *ct = new ComplexType( mNameSpace );
  addElement( element, ct );

  ComplexType *elementType = (ComplexType*)type( ct->element( 0 )->name() );
  if ( elementType ) {
    elementType->setDocumentation( ct->element( 0 )->documentation() );
  }

  Element *elementPtr = new Element();
  *elementPtr = (*ct->element( 0 ));
  delete ct;

  mElements.append( elementPtr );
}

void Parser::parseAttribute( const QDomElement &element )
{
  ComplexType *ct = new ComplexType( mNameSpace );
  addAttribute( element, ct );

  Attribute *attributePtr = new Attribute();
  *attributePtr = (*ct->attribute( 0 ));
  delete ct;

  mAttributes.append( attributePtr );
}

int Parser::addExternalElement( const QString &name, int localTypeId )
{
  Element *element = new Element( name, localTypeId );
  mElements.append( element );

  return mElements.count() - 1;
}

int Parser::typeId( const QualifiedName &type, bool create )
{
  QualifiedName typeName( type );

  QString typens = typeName.nameSpace();
  if ( typens.isEmpty() )
    typeName.setNameSpace( typens = mNameSpace );

  if ( typens == mNameSpace || typens == SchemaUri ) {
    return mTypesTable.typeId( typeName, create );
  } else {
    return mTypesTable.addExternalTypeId( typeName, 0 );
  }
}

QString Parser::typeName( int id ) const
{
  return mTypesTable.typeName( id );
}

bool Parser::finalize()
{
  if ( mTypesTable.detectUndefinedTypes() )
    return false;
  else
    return true;
}

void Parser::resolveForwardElementRefs()
{
  if ( mForwardElementRef.isEmpty() )
    return;

  QualifiedName::List::ConstIterator it;
  for ( it = mForwardElementRef.begin(); it != mForwardElementRef.end(); ++it ) {
    Element *e = element( elementId( *it ) );
    if ( e )
      mTypesTable.resolveForwardElementRefs( (*it).localName(), *e );
    else
     qDebug( "Could not resolve element reference %s ", (*it).localName().latin1() );
  }
}


void Parser::resolveForwardAttributeRefs()
{
  if ( mForwardAttributeRef.isEmpty() )
    return;

  QualifiedName::List::ConstIterator it;
  for ( it = mForwardAttributeRef.begin(); it != mForwardAttributeRef.end(); ++it ) {
    Attribute *a = attribute( attributeId( *it ) );
    if ( a )
      mTypesTable.resolveForwardAttributeRefs( (*it).localName(), *a );
    else
      qDebug( "Could not resolve attribute reference %s ", (*it).localName().latin1() );
  }
}

void Parser::resolveForwardDerivations()
{
  if ( mForwardDerivations.isEmpty() )
    return;

  int id;
  ComplexType *type = 0;

  QValueList<ForwardDerivation>::ConstIterator it;
  for ( it = mForwardDerivations.begin(); it != mForwardDerivations.end(); ++it ) {
    if ( ( id = typeId( (*it).type, false ) ) == 0 )
      continue;
    else
      type = (ComplexType*)mTypesTable.typePtr( id );

    if ( type )
      type->setBaseType( typeId( (*it).baseType, true ), (*it).derivation, this->type( (*it).baseType ) );
  }

  mForwardDerivations.clear();
}

int Parser::elementId( const QualifiedName &type )
{
  QualifiedName typeName( type );

  QString typens = typeName.nameSpace();
  if ( typens.isEmpty() )
    typeName.setNameSpace( typens = mNameSpace );

  int i = 0;

  // check if it is a global element
  for ( i = 0; i < (int)mElements.count(); i++ )
    if ( mElements[ i ]->name() == typeName.localName() )
      return i;

  return -1;
}

int Parser::elementType( const QualifiedName &type )
{
  int id = elementId( type );

  if ( id == -1 )
    return 0;

  Element *e = element( id );
  if ( e != 0 )
    return e->type();
  else
    return 0;
}

Element *Parser::element( const QualifiedName &name ) const
{
  QualifiedName elementName( name );

  QString typens = elementName.nameSpace();
  if ( typens.isEmpty() )
    elementName.setNameSpace( typens = mNameSpace );

  if ( typens == mNameSpace || typens == SchemaUri ) {
    int i = 0;

    // check if it is a global element
    for ( i = 0; i < (int)mElements.count(); i++ )
      if ( mElements[ i ]->name() == elementName.localName() )
        return mElements[ i ];

    return 0;
  }

  return 0;
}

Element *Parser::element( int id ) const
{
  if ( id >= 0 && id < (int)mElements.count() )
    return mElements[ id ];
  else
    return 0;
}

Element::PtrList Parser::elements() const
{
  return mElements;
}

int Parser::attributeId( const QualifiedName &type ) const
{
  QualifiedName typeName( type );

  QString typens = typeName.nameSpace();
  if ( typens.isEmpty() )
    typeName.setNameSpace( typens = mNameSpace );

  if ( typens != mNameSpace && typens != SchemaUri ) {
    qDebug( "Namespace does not match" );
    return -1;
  }

  // check if it is a global attribute
  for ( int i = 0; i < (int)mAttributes.count(); i++ )
    if ( mAttributes[ i ]->name() == typeName.localName() )
      return i;

  return -1;
}

int Parser::attributeType( const QualifiedName &type )
{
  int attId = attributeId( type );
  if ( attId == -1 )
    return 0;

  Attribute *a = attribute( attId );
  if ( a != 0 )
    return a->type();
  else
    return 0;
}

Attribute *Parser::attribute( int id ) const
{
  if ( id >= 0 && id < (int)mAttributes.count() )
    return mAttributes[ id ];
  else
    return 0;
}

Attribute *Parser::attribute( const QualifiedName &name ) const
{
  int id = attributeId( name );
  if ( id != -1 )
    return mAttributes[ id ];
  else
    return 0;
}

QString Parser::targetNamespace() const
{
  return mNameSpace;
}

const XSDType *Parser::type( int id ) const
{
  return (const XSDType *)mTypesTable.typePtr( id );
}

const XSDType *Parser::type( const QualifiedName &type ) 
{
  int id;
  
  if ( ( id = typeId( type, false ) ) == 0 )
    return 0;
  else
    return (const XSDType *)mTypesTable.typePtr( id );
}

int Parser::numTypes() const
{
  return mTypesTable.numTypes();
}

int Parser::numElements() const
{
  return mElements.count();
}

int Parser::numAttributes() const
{
  return mAttributes.count();
}

bool Parser::shouldResolve()
{
  return true;
}

void Parser::importSchema( const QString &location )
{
  FileProvider provider;
  QString fileName;
  QString schemaLocation( location );

  QUrl url( location );
  QDir dir( location );

  if ( (url.protocol().isEmpty() || url.protocol() == "file") && dir.isRelative() )
    schemaLocation = mSchemaBaseUrl + "/" + location;

  if ( provider.get( schemaLocation, fileName ) ) {
    QFile file( fileName );
    if ( !file.open( IO_ReadOnly ) ) {
      qDebug( "Unable to open file %s", file.name().latin1() );
      return;
    }

    QDomDocument doc( "kwsdl" );
    QString errorMsg;
    int errorLine, errorColumn;
    bool ok = doc.setContent( &file, true, &errorMsg, &errorLine, &errorColumn );
    if ( !ok ) {
      qDebug( "Error[%d:%d] %s", errorLine, errorColumn, errorMsg.latin1() );
      return;
    }

    QDomNodeList nodes = doc.elementsByTagName( "schema" );
    if ( nodes.count() > 0 ) {
      QDomElement schemaElement = nodes.item( 0 ).toElement();
      parseSchemaTag( schemaElement );
    } else {
      qDebug( "No schema tag found in schema file" );
    }

    file.close();

    provider.cleanUp();
  }
}
