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
#include <QDebug>

#include <common/fileprovider.h>
#include <common/messagehandler.h>
#include <common/nsmanager.h>
#include <common/parsercontext.h>
#include "parser.h"

static const QString XMLSchemaURI( "http://www.w3.org/2001/XMLSchema" );
static const QString WSDLSchemaURI( "http://schemas.xmlsoap.org/wsdl/" );

using namespace XSD;


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
  mAttributeGroups.clear();
}

bool Parser::parseFile( ParserContext *context, QFile &file )
{
  QXmlInputSource source( &file );
  QXmlSimpleReader reader;
  reader.setFeature( "http://xml.org/sax/features/namespace-prefixes", true );

  QDomDocument document( "KWSDL" );

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &source, &reader, &errorMsg, &errorLine, &errorCol ) ) {
    qDebug( "%s at (%d,%d)", qPrintable( errorMsg ), errorLine, errorCol );
    return false;
  }

  QDomElement element = doc.documentElement();
  if ( element.tagName() != "xs:schema" ) {
    qDebug( "document element is '%s'", qPrintable( element.tagName() ) );
    return false;
  }

  return parseSchemaTag( context, element );
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
      addGlobalElement( parseElement( context, element, mNameSpace, element ) );
    } else if ( name.localName() == "complexType" ) {
      ComplexType ct = parseComplexType( context, element );
      mComplexTypes.append( ct );
    } else if ( name.localName() == "simpleType" ) {
      SimpleType st = parseSimpleType( context, element );
      mSimpleTypes.append( st );
    } else if ( name.localName() == "attribute" ) {
      addGlobalAttribute( parseAttribute( context, element ) );
    } else if ( name.localName() == "attributeGroup" ) {
      mAttributeGroups.append( parseAttributeGroup( context, element ) );
    } else if ( name.localName() == "annotation" ) {
      parseAnnotation( context, element );
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

  AttributeGroup::List attributeGroups;

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "all" ) {
      all( context, childElement, newType );
    } else if ( name.localName() == "sequence" ) {
      parseCompositor( context, childElement, newType );
    } else if ( name.localName() == "choice" ) {
      parseCompositor( context, childElement, newType );
    } else if ( name.localName() == "attribute" ) {
      newType.addAttribute( parseAttribute( context, childElement ) );
    } else if ( name.localName() == "attributeGroup" ) {
      AttributeGroup g = parseAttributeGroup( context, childElement );
      attributeGroups.append( g );
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

  newType.setAttributeGroups( attributeGroups );

  return newType;
}

void Parser::all( ParserContext *context, const QDomElement &element, ComplexType &ct )
{
  QDomElement childElement = element.firstChildElement();

  while ( !childElement.isNull() ) {
    QName name = childElement.tagName();
    if ( name.localName() == "element" ) {
      ct.addElement( parseElement( context, childElement, ct.nameSpace(),
        childElement ) );
    } else if ( name.localName() == "annotation" ) {
      parseAnnotation( context, childElement, ct );
    }

    childElement = childElement.nextSiblingElement();
  }
}

void Parser::parseCompositor( ParserContext *context,
  const QDomElement &element, ComplexType &ct )
{
  QName name = element.tagName();
  bool isChoice = name.localName() == "choice";
  bool isSequence = name.localName() == "sequence";

  Compositor compositor;
  if ( isChoice ) compositor.setType( Compositor::Choice );
  else if ( isSequence ) compositor.setType( Compositor::Sequence );

  if ( isChoice || isSequence ) {
    Element::List newElements;

    QDomElement childElement = element.firstChildElement();

    while ( !childElement.isNull() ) {
      QName csName = childElement.tagName();
      if ( csName.localName() == "element" ) {
        Element newElement;
        if ( isChoice ) {
          newElement = parseElement( context, childElement,
            ct.nameSpace(), element );
        } else {
          newElement = parseElement( context, childElement,
            ct.nameSpace(), childElement );
        }
        newElements.append( newElement );
        compositor.addChild( csName );
      } else if ( csName.localName() == "any" ) {
        addAny( context, childElement, ct );
      } else if ( isChoice ) {
        parseCompositor( context, childElement, ct );
      } else if ( isSequence ) {
        parseCompositor( context, childElement, ct );
      }

      childElement = childElement.nextSiblingElement();
    }
    
    foreach( Element e, newElements ) {
      e.setCompositor( compositor );
      ct.addElement( e );
    }
  }
}

Element Parser::parseElement( ParserContext *context,
  const QDomElement &element, const QString &nameSpace,
  const QDomElement &occurrenceElement )
{
  Element newElement( nameSpace );

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

  setOccurrenceAttributes( newElement, occurrenceElement );

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

  return newElement;
}

void Parser::addAny( ParserContext*, const QDomElement &element, ComplexType &complexType )
{
  Element newElement( complexType.nameSpace() );
  newElement.setName( "any" );
  setOccurrenceAttributes( newElement, element );

  complexType.addElement( newElement );
}

void Parser::setOccurrenceAttributes( Element &newElement,
  const QDomElement &element )
{
  newElement.setMinOccurs( element.attribute( "minOccurs", "1" ).toInt() );

  QString value = element.attribute( "maxOccurs", "1" );
  if ( value == "unbounded" )
    newElement.setMaxOccurs( UNBOUNDED );
  else
    newElement.setMaxOccurs( value.toInt() );
}

void Parser::addAnyAttribute( ParserContext*, const QDomElement &element, ComplexType &complexType )
{
  Attribute newAttribute;
  newAttribute.setName( "anyAttribute" );

  newAttribute.setNameSpace( element.attribute( "namespace" ) );

  complexType.addAttribute( newAttribute );
}

Attribute Parser::parseAttribute( ParserContext *context,
  const QDomElement &element )
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

  return newAttribute;
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
            parseCompositor( context, ctElement, complexType );
          } else if ( name.localName() == "choice" ) {
            parseCompositor( context, ctElement, complexType );
          } else if ( name.localName() == "attribute" ) {
            complexType.addAttribute( parseAttribute( context, ctElement ) );
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
            complexType.addAttribute( parseAttribute( context, ctElement ) );

          ctElement = ctElement.nextSiblingElement();
        }
      }
    }

    childElement = childElement.nextSiblingElement();
  }
}

void Parser::addGlobalElement( const Element &newElement )
{
  // don't add elements twice
  bool found = false;
  for ( int i = 0; i < mElements.count(); ++i ) {
    if ( mElements[ i ].qualifiedName() == newElement.qualifiedName() ) {
      found = true;
      break;
    }
  }

  if ( !found ) {
    mElements.append( newElement );
  }
}

void Parser::addGlobalAttribute( const Attribute &newAttribute )
{
  // don't add attributes twice
  bool found = false;
  for ( int i = 0; i < mAttributes.count(); ++i ) {
    if ( mAttributes[ i ].qualifiedName() == newAttribute.qualifiedName() ) {
      found = true;
      break;
    }
  }

  if ( !found ) {
    mAttributes.append( newAttribute );
  }
}

AttributeGroup Parser::parseAttributeGroup( ParserContext *context,
  const QDomElement &element )
{
  Attribute::List attributes;

  AttributeGroup group;

  if ( element.hasAttribute( "ref" ) ) {
    QName reference;
    reference = element.attribute( "ref" );
    reference.setNameSpace(
      context->namespaceManager()->uri( reference.prefix() ) );
    group.setReference( reference );

    return group;
  }

  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    QName childName = QName( e.tagName() );
    if ( childName.localName() == "attribute" ) {
      Attribute a = parseAttribute( context, e );
      addGlobalAttribute( a );
      attributes.append( a );
    }
  }

  group.setName( element.attribute( "name" ) );
  group.setAttributes( attributes );
  
  return group;
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

AttributeGroup Parser::findAttributeGroup( const QName &name )
{
  foreach ( AttributeGroup g, mAttributeGroups ) {
    if ( g.nameSpace() == name.nameSpace() && g.name() == name.localName() ) {
      return g;
    }
  }

  return AttributeGroup();
}

void Parser::resolveForwardDeclarations()
{
  for ( int i = 0; i < mComplexTypes.count(); ++i ) {

    Element::List elements = mComplexTypes[ i ].elements();
    for ( int j = 0; j < elements.count(); ++j ) {
      Element element = elements[ j ];
      if ( !element.isResolved() ) {
        Element refElement = findElement( element.reference() );
        Element resolvedElement = refElement;
        resolvedElement.setMinOccurs( element.minOccurs() );
        resolvedElement.setMaxOccurs( element.maxOccurs() );
        resolvedElement.setCompositor( element.compositor() );
        elements[ j ] = resolvedElement;
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
    
    AttributeGroup::List attributeGroups =
      mComplexTypes[ i ].attributeGroups();
    foreach( AttributeGroup group, attributeGroups ) {
      if ( !group.isResolved() ) {
        AttributeGroup refAttributeGroup =
          findAttributeGroup( group.reference() );
        Attribute::List groupAttributes = refAttributeGroup.attributes();
        foreach ( Attribute ga, groupAttributes ) {
          attributes.append( ga );
        }
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
  types.setAttributeGroups( mAttributeGroups );
  types.setNamespaces( mNamespaces );

  return types;
}

