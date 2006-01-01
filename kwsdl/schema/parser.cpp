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

#include <QDir>
#include <QFile>
#include <QUrl>
#include <QXmlSimpleReader>

#include <common/fileprovider.h>
#include <common/messagehandler.h>
#include <common/nsmanager.h>
#include <common/parsercontext.h>
#include "parser.h"

static const unsigned int UNBOUNDED( 100000 );
static const QString XMLSchemaURI( "http://www.w3.org/2001/XMLSchema" );
static const QString WSDLSchemaURI( "http://schemas.xmlsoap.org/wsdl/" );

using namespace Schema;


Parser::Parser( const QString &nameSpace )
  : mNameSpace( nameSpace )
{
}

Parser::~Parser()
{
  clear();
}

void Parser::clear()
{
  mImportedSchemas.clear();
  mNamespaces.clear();
  mComplexTypes.clear();
  mSimpleTypes.clear();
  mElements.clear();
  mAttributes.clear();
}

bool Parser::parseSchemaTag( ParserContext *context, const QDomElement &root )
{
  QName name = root.tagName();
  if ( name.localName() != "schema" )
    return false;

  NSManager *parentManager = context->namespaceManager();
  NSManager namespaceManager;

  // copy namespaces from wsdl
  if ( parentManager )
    namespaceManager = *parentManager;

  context->setNamespaceManager( &namespaceManager );

  QDomNamedNodeMap attributes = root.attributes();
  for ( int i = 0; i < attributes.count(); ++i ) {
    QDomAttr attribute = attributes.item( i ).toAttr();
    if ( attribute.name().startsWith( "xmlns:" ) ) {
      QString prefix = attribute.name().mid( 6 );
      context->namespaceManager()->setPrefix( prefix, attribute.value() );
    }
  }

  if ( root.hasAttribute( "targetNamespace" ) )
    mNameSpace = root.attribute( "targetNamespace" );

 // mTypesTable.setTargetNamespace( mNameSpace );

  QDomElement element = root.firstChildElement();
  while ( !element.isNull() ) {
    QName name = element.tagName();
    if ( name.localName() == "import" ) {
      parseImport( context, element );
    } else if ( name.localName() == "element" ) {
      parseElement( context, element );
    } else if ( name.localName() == "complexType" ) {
      ComplexType ct = parseComplexType( context, element );
      mComplexTypes.append( ct );
    } else if ( name.localName() == "simpleType" ) {
      SimpleType st = parseSimpleType( context, element );
      mSimpleTypes.append( st );
    } else if ( name.localName() == "attribute" ) {
      parseAttribute( context, element );
    } else if ( name.localName() == "annotation" ) {
      parseAnnotation( context, element );
    } else if ( name.localName() == "import" ) {
      // TODO
    } else if ( name.localName() == "include" ) {
      // TODO
    }

    element = element.nextSiblingElement();
  }

  context->setNamespaceManager( parentManager );
  mNamespaces = joinNamespaces( mNamespaces, namespaceManager.uris() );
  mNamespaces = joinNamespaces( mNamespaces, QStringList( mNameSpace ) );

  resolveForwardDeclarations();

  return true;
}

void Parser::parseImport( ParserContext *context, const QDomElement &element )
{
  QString location = element.attribute( "schemaLocation" );

  if ( location.isEmpty() )
    location = element.attribute( "namespace" );

  if ( !location.isEmpty() ) {
    // don't import a schema twice
    if ( mImportedSchemas.contains( location ) )
      return;
    else
      mImportedSchemas.append( location );

    importSchema( context, location );
  }
}

void Parser::parseAnnotation( ParserContext*, const QDomElement& )
{
}

void Parser::parseAnnotation( ParserContext*, const QDomElement &element, QString &documentation )
{
  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "documentation" )
      documentation = childElement.text().trimmed();

    childElement = childElement.nextSiblingElement();
  }
}

void Parser::parseAnnotation( ParserContext*, const QDomElement &element, ComplexType &complexType )
{
  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "documentation" )
      complexType.setDocumentation( childElement.text().trimmed() );

    childElement = childElement.nextSiblingElement();
  }
}

void Parser::parseAnnotation( ParserContext*, const QDomElement &element, SimpleType &simpleType )
{
  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "documentation" )
      simpleType.setDocumentation( childElement.text().trimmed() );

    childElement = childElement.nextSiblingElement();
  }
}

ComplexType Parser::parseComplexType( ParserContext *context, const QDomElement &element )
{
  ComplexType newType( mNameSpace );

  newType.setName( element.attribute( "name" ) );

  if ( element.hasAttribute( "mixed" ) )
    newType.setContentModel( XSDType::MIXED );

  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "all" ) {
      all( context, childElement, newType );
    } else if ( name.localName() == "sequence" ) {
      cs( context, childElement, newType );
    } else if ( name.localName() == "choice" ) {
      cs( context, childElement, newType );
    } else if ( name.localName() == "attribute" ) {
      addAttribute( context, childElement, newType );
    } else if ( name.localName() == "anyAttribute" ) {
      addAnyAttribute( context, childElement, newType );
    } else if ( name.localName() == "complexContent" ) {
      parseComplexContent( context, childElement, newType );
    } else if ( name.localName() == "simpleContent" ) {
      parseSimpleContent( context, childElement, newType );
    } else if ( name.localName() == "annotation" ) {
      parseAnnotation( context, childElement, newType );
    }

    childElement = childElement.nextSiblingElement();
  }

  return newType;
}

void Parser::all( ParserContext *context, const QDomElement &element, ComplexType &ct )
{
  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "element" ) {
      addElement( context, childElement, ct );
    } else if ( name.localName() == "annotation" ) {
      parseAnnotation( context, childElement, ct );
    }

    childElement = childElement.nextSiblingElement();
  }
}


void Parser::cs( ParserContext *context, const QDomElement &element, ComplexType &ct )
{
  QName name = element.tagName();
  if ( name.localName() == "choice" || name.localName() == "sequence" ) {
    QDomElement childElement = element.firstChildElement();

    while ( !childElement.isNull() ) {
      QName csName = childElement.tagName();
      if ( csName.localName() == "element" )
        addElement( context, childElement, ct );
      else if ( csName.localName() == "any" )
        addAny( context, childElement, ct );
      else if ( csName.localName() == "choice" )
        cs( context, childElement, ct );
      else if ( csName.localName() == "sequence" )
        cs( context, childElement, ct );

      childElement = childElement.nextSiblingElement();
    }
  }
}

void Parser::addElement( ParserContext *context, const QDomElement &element, ComplexType &complexType )
{
  Element newElement( complexType.nameSpace() );

  newElement.setName( element.attribute( "name" ) );

  if ( element.hasAttribute( "form" ) ) {
    if ( element.attribute( "form" ) == "qualified" )
      newElement.setIsQualified( true );
    else if ( element.attribute( "form" ) == "unqualified" )
      newElement.setIsQualified( false );
    else
      newElement.setIsQualified( false );
  }

  if ( element.hasAttribute( "ref" ) ) {
    QName reference = element.attribute( "ref" );
    reference.setNameSpace( context->namespaceManager()->uri( reference.prefix() ) );

    newElement.setReference( reference );
  }

  newElement.setMinOccurs( element.attribute( "minOccurs", "1" ).toInt() );
  QString value = element.attribute( "maxOccurs", "1" );
  if ( value == "unbounded" )
    newElement.setMaxOccurs( UNBOUNDED );
  else
    newElement.setMaxOccurs( value.toInt() );

  newElement.setDefaultValue( element.attribute( "default" ) );
  newElement.setFixedValue( element.attribute( "fixed" ) );

  bool nill = false;
  if ( element.hasAttribute( "nillable" ) )
    nill = true;

  QName anyType( "http://www.w3.org/2001/XMLSchema", "any" );


  if ( element.hasAttribute( "type" ) ) {
    QName typeName = element.attribute( "type" );
    typeName.setNameSpace( context->namespaceManager()->uri( typeName.prefix() ) );
    newElement.setType( typeName );
  } else {
    QDomElement childElement = element.firstChildElement();

    while ( !childElement.isNull() ) {
      QName childName = childElement.tagName();
      if ( childName.localName() == "complexType" ) {
        ComplexType ct = parseComplexType( context, childElement );

        ct.setName( newElement.name() + "Anonymous" );
        mComplexTypes.append( ct );

        newElement.setType( ct.qualifiedName() );
      } else if ( childName.localName() == "simpleType" ) {
        SimpleType st = parseSimpleType( context, childElement );

        st.setName( newElement.name() + "Anonymous" );
        mSimpleTypes.append( st );

        newElement.setType( st.qualifiedName() );
      } else if ( childName.localName() == "annotation" ) {
        QString documentation;
        parseAnnotation( context, childElement, documentation );
        newElement.setDocumentation( documentation );
      }

      childElement = childElement.nextSiblingElement();
    }
  }

  complexType.addElement( newElement );
}

void Parser::addAny( ParserContext*, const QDomElement &element, ComplexType &complexType )
{
  Element newElement( complexType.nameSpace() );
  newElement.setName( "any" );
  newElement.setMinOccurs( element.attribute( "minOccurs", "1" ).toInt() );

  QString value = element.attribute( "maxOccurs", "1" );
  if ( value == "unbounded" )
    newElement.setMaxOccurs( UNBOUNDED );
  else
    newElement.setMaxOccurs( value.toInt() );

  complexType.addElement( newElement );
}

void Parser::addAnyAttribute( ParserContext*, const QDomElement &element, ComplexType &complexType )
{
  Attribute newAttribute;
  newAttribute.setName( "anyAttribute" );

  newAttribute.setNameSpace( element.attribute( "namespace" ) );

  complexType.addAttribute( newAttribute );
}

void Parser::addAttribute( ParserContext *context, const QDomElement &element, ComplexType &complexType )
{
  Attribute newAttribute;

  newAttribute.setName( element.attribute( "name" ) );

  if ( element.hasAttribute( "type" ) ) {
    QName typeName = element.attribute( "type" );
    typeName.setNameSpace( context->namespaceManager()->uri( typeName.prefix() ) );
    newAttribute.setType( typeName );
  }

  if ( element.hasAttribute( "form" ) ) {
    if ( element.attribute( "form" ) == "qualified" )
      newAttribute.setIsQualified( true );
    else if ( element.attribute( "form" ) == "unqualified" )
      newAttribute.setIsQualified( false );
    else
      newAttribute.setIsQualified( false );
  }

  if ( element.hasAttribute( "ref" ) ) {
    QName reference;
    reference = element.attribute( "ref" );
    reference.setNameSpace( context->namespaceManager()->uri( reference.prefix() ) );

    newAttribute.setReference( reference );
  }

  newAttribute.setDefaultValue( element.attribute( "default" ) );
  newAttribute.setFixedValue( element.attribute( "fixed" ) );

  if ( element.hasAttribute( "use" ) ) {
    if ( element.attribute( "use" ) == "optional" )
      newAttribute.setIsUsed( false );
    else if ( element.attribute( "use" ) == "required" )
      newAttribute.setIsUsed( true );
    else
      newAttribute.setIsUsed( false );
  }

  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName childName = childElement.tagName();
    if ( childName.localName() == "simpleType" ) {
      SimpleType st = parseSimpleType( context, childElement );
      st.setName( newAttribute.name() );
      mSimpleTypes.append( st );

      newAttribute.setType( st.qualifiedName() );
    } else if ( childName.localName() == "annotation" ) {
      QString documentation;
      parseAnnotation( context, childElement, documentation );
      newAttribute.setDocumentation( documentation );
    }

    childElement = childElement.nextSiblingElement();
  }

  complexType.addAttribute( newAttribute );
}

SimpleType Parser::parseSimpleType( ParserContext *context, const QDomElement &element )
{
  SimpleType st( mNameSpace );

  st.setName( element.attribute( "name" ) );

  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "restriction" ) {
      st.setSubType( SimpleType::TypeRestriction );

      QName typeName( childElement.attribute( "base" ) );
      typeName.setNameSpace( context->namespaceManager()->uri( typeName.prefix() ) );
      st.setBaseTypeName( typeName );

      parseRestriction( context, childElement, st );
    } else if ( name.localName() == "union" ) {
      st.setSubType( SimpleType::TypeUnion );
      qDebug( "simpletype::union not supported" );
    } else if ( name.localName() == "list" ) {
      st.setSubType( SimpleType::TypeList );
      if ( childElement.hasAttribute( "itemType" ) ) {
        QName typeName( childElement.attribute( "itemType" ) );
        if ( !typeName.prefix().isEmpty() )
          typeName.setNameSpace( context->namespaceManager()->uri( typeName.prefix() ) );
        else
          typeName.setNameSpace( st.nameSpace() );
        st.setListTypeName( typeName );
      } else {
        // TODO: add support for anonymous types
      }
    } else if ( name.localName() == "annotation" ) {
      parseAnnotation( context, childElement, st );
    }

    childElement = childElement.nextSiblingElement();
  }

  return st;
}

void Parser::parseRestriction( ParserContext*, const QDomElement &element, SimpleType &st )
{
  if ( st.baseTypeName().isEmpty() )
    qDebug( "<restriction>:unkown BaseType" );

  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName tagName = childElement.tagName();
    if ( !st.isValidFacet( tagName.localName() ) ) {
      qDebug( "<restriction>: %s is not a valid facet for the simple type", qPrintable( childElement.tagName() ) );
      childElement = childElement.nextSiblingElement();
      continue;
    }

    st.setFacetValue( childElement.attribute( "value" ) );

    childElement = childElement.nextSiblingElement();
  }
}

void Parser::parseComplexContent( ParserContext *context, const QDomElement &element, ComplexType &complexType )
{
  QName typeName;

  if ( element.attribute( "mixed" ) == "true" ) {
    qDebug( "<complexContent>: No support for mixed=true" );
    return;
  }

  complexType.setContentModel( XSDType::COMPLEX );

  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();

    if ( name.localName() == "restriction" || name.localName() == "extension" ) {
      typeName = childElement.attribute( "base" );
      typeName.setNameSpace( context->namespaceManager()->uri( typeName.prefix() ) );

      complexType.setBaseTypeName( typeName );

      // if the base soapenc:Array, then read only the arrayType attribute and nothing else
      if ( typeName.localName() == "Array" ) {
        complexType.setIsArray( true );

        QDomElement arrayElement = childElement.firstChildElement();
        if ( !arrayElement.isNull() ) {
          QString prefix = context->namespaceManager()->prefix( WSDLSchemaURI );
          QString attributeName = ( prefix.isEmpty() ? "arrayType" : prefix + ":arrayType" );

          QString typeStr = arrayElement.attribute( attributeName );
          if ( typeStr.endsWith( "[]" ) )
            typeStr.truncate( typeStr.length() - 2 );

          QName arrayType( typeStr );
          arrayType.setNameSpace( context->namespaceManager()->uri( arrayType.prefix() ) );

          Attribute attr( complexType.nameSpace() );
          attr.setName( "items" );
          attr.setType( QName( "arrayType" ) );
          attr.setArrayType( arrayType );

          complexType.addAttribute( attr );
        }
      } else {
        QDomElement ctElement = childElement.firstChildElement();
        while ( !ctElement.isNull() ) {
          QName name = ctElement.tagName();

          if ( name.localName() == "all" ) {
            all( context, ctElement, complexType );
          } else if ( name.localName() == "sequence" ) {
            cs( context, ctElement, complexType );
          } else if ( name.localName() == "choice" ) {
            cs( context, ctElement, complexType );
          } else if ( name.localName() == "attribute" ) {
            addAttribute( context, ctElement, complexType );
          } else if ( name.localName() == "anyAttribute" ) {
            addAnyAttribute( context, ctElement, complexType );
          }

          ctElement = ctElement.nextSiblingElement();
        }
      }
    }

    childElement = childElement.nextSiblingElement();
  }
}

void Parser::parseSimpleContent( ParserContext *context, const QDomElement &element, ComplexType &complexType )
{
  complexType.setContentModel( XSDType::SIMPLE );

  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "restriction" ) {
      SimpleType st( mNameSpace );

      if ( childElement.hasAttribute( "base" ) ) {
        QName typeName( childElement.attribute( "base" ) );
        typeName.setNameSpace( context->namespaceManager()->uri( typeName.prefix() ) );
        st.setBaseTypeName( typeName );
      }

      parseRestriction( context, childElement, st );
    } else if ( name.localName() == "extension" ) {
      // This extension does not use the full model that can come in ComplexContent.
      // It uses the simple model. No particle allowed, only attributes

      if ( childElement.hasAttribute( "base" ) ) {
        QName typeName( childElement.attribute( "base" ) );
        typeName.setNameSpace( context->namespaceManager()->uri( typeName.prefix() ) );
        complexType.setBaseTypeName( typeName );

        QDomElement ctElement = childElement.firstChildElement();
        while ( !ctElement.isNull() ) {
          QName name = ctElement.tagName();
          if ( name.localName() == "attribute" )
            addAttribute( context, ctElement, complexType );

          ctElement = ctElement.nextSiblingElement();
        }
      }
    }

    childElement = childElement.nextSiblingElement();
  }
}

void Parser::parseElement( ParserContext *context, const QDomElement &element )
{
  ComplexType complexType( mNameSpace );
  addElement( context, element, complexType );

  // don't add elements twice
  Element newElement = complexType.elements().first();
  bool found = false;
  for ( int i = 0; i < mElements.count(); ++i ) {
    if ( mElements[ i ].qualifiedName() == newElement.qualifiedName() ) {
      found = true;
      break;
    }
  }

  if ( !found )
    mElements.append( newElement );
}

void Parser::parseAttribute( ParserContext *context, const QDomElement &element )
{
  ComplexType complexType( mNameSpace );
  addAttribute( context, element, complexType );

  // don't add attributes twice
  Attribute newAttribute = complexType.attributes().first();
  bool found = false;
  for ( int i = 0; i < mAttributes.count(); ++i ) {
    if ( mAttributes[ i ].qualifiedName() == newAttribute.qualifiedName() ) {
      found = true;
      break;
    }
  }

  if ( !found )
    mAttributes.append( newAttribute );
}

QString Parser::targetNamespace() const
{
  return mNameSpace;
}

void Parser::importSchema( ParserContext *context, const QString &location )
{
  FileProvider provider;
  QString fileName;
  QString schemaLocation( location );

  QUrl url( location );
  QDir dir( location );

  if ( (url.scheme().isEmpty() || url.scheme() == "file") && dir.isRelative() )
    schemaLocation = context->documentBaseUrl() + "/" + location;

  qDebug( "loading schema at %s", qPrintable( schemaLocation ) );

  if ( provider.get( schemaLocation, fileName ) ) {
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly ) ) {
      qDebug( "Unable to open file %s", qPrintable( file.fileName() ) );
      return;
    }

    QXmlInputSource source( &file );
    QXmlSimpleReader reader;
    reader.setFeature( "http://xml.org/sax/features/namespace-prefixes", true );

    QDomDocument doc( "kwsdl" );
    QString errorMsg;
    int errorLine, errorColumn;
    bool ok = doc.setContent( &source, &reader, &errorMsg, &errorLine, &errorColumn );
    if ( !ok ) {
      qDebug( "Error[%d:%d] %s", errorLine, errorColumn, qPrintable( errorMsg ) );
      return;
    }

    NSManager *parentManager = context->namespaceManager();
    NSManager namespaceManager;
    context->setNamespaceManager( &namespaceManager );

    QDomElement node = doc.documentElement();
    QName tagName = node.tagName();
    if ( tagName.localName() == "schema" ) {
      parseSchemaTag( context, node );
    } else {
      qDebug( "No schema tag found in schema file" );
    }

    mNamespaces = joinNamespaces( mNamespaces, namespaceManager.uris() );
    context->setNamespaceManager( parentManager );

    file.close();

    provider.cleanUp();
  }
}

QString Parser::schemaUri()
{
  return XMLSchemaURI;
}

QStringList Parser::joinNamespaces( const QStringList &list, const QStringList &namespaces )
{
  QStringList retval( list );

  for ( int i = 0; i < namespaces.count(); ++i ) {
    if ( !retval.contains( namespaces[ i ] ) )
      retval.append( namespaces[ i ] );
  }

  return retval;
}

Element Parser::findElement( const QName &name )
{
  for ( int i = 0; i < mElements.count(); ++i ) {
    if ( mElements[ i ].nameSpace() == name.nameSpace() && mElements[ i ].name() == name.localName() )
      return mElements[ i ];
  }

  return Element();
}

Attribute Parser::findAttribute( const QName &name )
{
  for ( int i = 0; i < mAttributes.count(); ++i ) {
    if ( mAttributes[ i ].nameSpace() == name.nameSpace() && mAttributes[ i ].name() == name.localName() )
      return mAttributes[ i ];
  }

  return Attribute();
}

void Parser::resolveForwardDeclarations()
{
  for ( int i = 0; i < mComplexTypes.count(); ++i ) {
    Element::List elements = mComplexTypes[ i ].elements();
    for ( int j = 0; j < elements.count(); ++j ) {
      if ( !elements[ j ].isResolved() ) {
        Element refElement = findElement( elements[ j ].reference() );
        elements[ j ] = refElement;
      }
    }
    mComplexTypes[ i ].setElements( elements );

    Attribute::List attributes = mComplexTypes[ i ].attributes();
    for ( int j = 0; j < attributes.count(); ++j ) {
      if ( !attributes[ j ].isResolved() ) {
        Attribute refAttribute = findAttribute( attributes[ j ].reference() );
        attributes[ j ] = refAttribute;
      }
    }
    mComplexTypes[ i ].setAttributes( attributes );
  }
}

Types Parser::types() const
{
  Types types;

  types.setSimpleTypes( mSimpleTypes );
  types.setComplexTypes( mComplexTypes );
  types.setElements( mElements );
  types.setAttributes( mAttributes );
  types.setNamespaces( mNamespaces );

  return types;
}

