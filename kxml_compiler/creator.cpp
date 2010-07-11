/*
    This file is part of KDE.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>

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

#include "creator.h"

#include "namer.h"

#include "parsercreatordom.h"
#include "parsercreatorcustom.h"
#include "writercreator.h"

#include <libkode/code.h>
#include <libkode/printer.h>
#include <libkode/typedef.h>
#include <libkode/statemachine.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <kconfig.h>
#include <kstandarddirs.h>

#include <QFile>
#include <QTextStream>
#include <qdom.h>
#include <QRegExp>
#include <QMap>
#include <QList>

#include <iostream>

Creator::ClassFlags::ClassFlags( const Schema::Element &element )
{
  m_hasId = element.hasRelation( "id" );
  m_hasUpdatedTimestamp = element.hasRelation( "updated_at" );
}
    
bool Creator::ClassFlags::hasUpdatedTimestamp() const
{
  return m_hasUpdatedTimestamp;
}

bool Creator::ClassFlags::hasId() const
{
  return m_hasId;
}


Creator::Creator( const Schema::Document &document, XmlParserType p,
  XmlWriterType w )
  : mDocument( document ), mXmlParserType( p ), mXmlWriterType( w ),
    mVerbose( false ), mUseKde( false )
{
  setExternalClassNames();
}

// FIXME: Handle creation options via flags enum.

void Creator::setVerbose( bool verbose ) 
{
  mVerbose = verbose;
}

void Creator::setUseKde( bool useKde )
{
  mUseKde = useKde;
}

bool Creator::useKde() const
{
  return mUseKde;
}

void Creator::setCreateCrudFunctions( bool enabled )
{
  mCreateCrudFunctions = enabled;
}

void Creator::setLicense( const KODE::License &l )
{
  mFile.setLicense( l );
}

void Creator::setExportDeclaration( const QString &name )
{
  mExportDeclaration = name;
}

void Creator::setExternalClassPrefix( const QString &prefix )
{
  mExternalClassPrefix = prefix;

  setExternalClassNames();
}

void Creator::setExternalClassNames()
{
  mParserClass.setName( mExternalClassPrefix + "Parser" );
  mWriterClass.setName( mExternalClassPrefix + "Writer" );
}

KODE::File &Creator::file()
{
  return mFile;
}

void Creator::createProperty( KODE::Class &c, const QString &type,
  const QString &name )
{
  if ( type.startsWith( "Q" ) ) {
    c.addHeaderInclude( type );
  }

  KODE::MemberVariable v( Namer::getClassName( name ), type );
  c.addMemberVariable( v );

  KODE::Function mutator( Namer::getMutator( name ), "void" );
  if ( type == "int" ) {
    mutator.addArgument( type + " v" );
  } else {
    mutator.addArgument( "const " + type + " &v" );
  }
  mutator.addBodyLine( v.name() + " = v;" );
  c.addFunction( mutator );

  KODE::Function accessor( Namer::getAccessor( name ), type );
  accessor.setConst( true );
  accessor.addBodyLine( "return " + v.name() + ';' );
  c.addFunction( accessor );
}

void Creator::createElementFunctions( KODE::Class &c, const Schema::Element &e,
  const Schema::Relation &r )
{
  Schema::Element targetElement = mDocument.element( r );

  QString className = Namer::getClassName( targetElement );

#if 1
  if ( mVerbose ) {
    kDebug() <<"Creator::createElementFunctions()";
    kDebug() <<"ELEMENT" << e.identifier();
    kDebug() <<"RELATION:" << r.asString();
    kDebug() << "TARGETELEMENT" << targetElement.name();
    foreach( Schema::Relation r, targetElement.attributeRelations() ) {
      kDebug() << "  ATTR" << r.target();
    }
  }
#endif

  if ( targetElement.text() && !targetElement.hasAttributeRelations() ) {
    if ( mVerbose ) {
      kDebug() << "  FLATTEN";
    }
    if ( targetElement.type() == Schema::Element::Integer ) {
      createProperty( c, "int", className );
    } else if ( targetElement.type() == Schema::Element::Date ) {
      createProperty( c, "QDate", className );
      c.addHeaderInclude( "QDate" );
    } else {
      createProperty( c, "QString", className );
      c.addHeaderInclude( "QString" );
      if ( mXmlParserType == XmlParserCustomExternal ) {
        ParserCreatorCustom parserCreatorCustom( this );
        parserCreatorCustom.createTextElementParser( c, targetElement );
      }
    }

    if ( mCreateCrudFunctions && targetElement.name() == "id" ) {
      KODE::Function isValid( "isValid", "bool" );
      isValid.setConst( true );
      KODE::Code code;
      code += "return !mId.isEmpty();";
      isValid.setBody( code );
      c.addFunction( isValid );
    }

    return;
  }

  if ( !mFile.hasClass( className ) ) {
    createClass( targetElement );
  } else {
  }

  QString name = KODE::Style::lowerFirst( className );

  if ( r.isList() ) {
    registerListTypedef( className );

    c.addHeaderInclude( "QList" );
    name = name + "List";

    KODE::Function adder( "add" + className, "void" );
    adder.addArgument( "const " + className + " &v" );

    KODE::Code code;
    code += 'm' + KODE::Style::upperFirst( name ) + ".append( v );";

    adder.setBody( code );

    c.addFunction( adder );

    createProperty( c, className + "::List", name );

    ClassFlags targetClassFlags( targetElement );

    if ( mCreateCrudFunctions && targetClassFlags.hasId() ) {
      createCrudFunctions( c, className );
    }
  } else {
    createProperty( c, className, name );
  }
}

void Creator::createCrudFunctions( KODE::Class &c, const QString &type )
{
  if ( !c.hasEnum( "Flags" ) ) {
    QStringList enumValues;
    enumValues << "None" << "AutoCreate";
    KODE::Enum flags( "Flags", enumValues );
    c.addEnum( flags );
  }

  KODE::Function finder( "find" + type, type );

  finder.addArgument( "const QString &id" );
  finder.addArgument( KODE::Function::Argument( "Flags flags", "None" ) );

  QString listMember = "m" + type + "List";

  KODE::Code code;
  code += "foreach( " + type + " v, " + listMember + " ) {";
  code += "  if ( v.id() == id ) return v;";
  code += "}";
  code += type + " v;";
  code += "if ( flags == AutoCreate ) {";
  code += "  v.setId( id );";
  code += "}";
  code += "return v;";

  finder.setBody( code );

  c.addFunction( finder );

  KODE::Function inserter( "insert", "bool" );

  inserter.addArgument( "const " + type + " &v" );

  code.clear();

  code += "int i = 0;";
  code += "for( ; i < " + listMember + ".size(); ++i ) {";
  code += "  if ( " + listMember + "[i].id() == v.id() ) {";
  code += "    " + listMember + "[i] = v;";
  code += "    return true;";
  code += "  }";
  code += "}";
  code += "if ( i == " + listMember + ".size() ) {";
  code += "  add" + type + "( v );";
  code += "}";
  code += "return true;";

  inserter.setBody( code );

  c.addFunction( inserter );

  KODE::Function remover( "remove", "bool" );

  remover.addArgument( "const " + type + " &v" );

  code.clear();

  code += type + "::List::Iterator it;";
  code += "for( it = " + listMember + ".begin(); it != " + listMember +
    ".end(); ++it ) {";
  code += "  if ( (*it).id() == v.id() ) break;";
  code += "}";
  code += "if ( it != " + listMember + ".end() ) {";
  code += "  " + listMember + ".erase( it );";
  code += "}";
  code += "return true;";

  remover.setBody( code );

  c.addFunction( remover );
}

void Creator::createClass( const Schema::Element &element )
{
  QString className = Namer::getClassName( element  );

  if ( mVerbose ) {
    kDebug() <<"Creator::createClass()" << element.identifier() << className;
    foreach( Schema::Relation r, element.elementRelations() ) {
      kDebug() << "  SUBELEMENTS" << r.target();
    }  
  }

  if ( mProcessedClasses.contains( className ) ) {
    if ( mVerbose ) {
      kDebug() << "  ALREADY DONE";
    }
    return;
  }

  KODE::Class c( className );

  if ( !mExportDeclaration.isEmpty() ) {
    c.setExportDeclaration( mExportDeclaration );
  }

  mProcessedClasses.append( className );

  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );
    if ( a.type() == Schema::Element::DateTime ) {
      createProperty( c, "QDateTime", a.name() );
    } else {
      createProperty( c, "QString", a.name() );
    }
  }

  if ( element.text() ) {
    if ( element.type() == Schema::Element::Date ) {
      createProperty( c, "QDate", "date" );
    } else {
      createProperty( c, "QString", "text" );
    }
  }

  foreach( Schema::Relation r, element.elementRelations() ) {
    createElementFunctions( c, element, r );
  }

  createElementParser( c, element );
  
  WriterCreator writerCreator( mFile, mDocument, mDtd );
  writerCreator.createElementWriter( c, element );

  mFile.insertClass( c );
}

void Creator::createElementParser( KODE::Class &c, const Schema::Element &e )
{
  ParserCreator *parserCreator = 0;

  switch ( mXmlParserType ) {
    case XmlParserDom:
    case XmlParserDomExternal:
      parserCreator = new ParserCreatorDom( this );
      break;
    case XmlParserCustomExternal:
      parserCreator = new ParserCreatorCustom( this );
      break;
  }

  parserCreator->createElementParser( c, e );

  delete parserCreator;
}

void Creator::registerListTypedef( const QString &type )
{
  if ( !mListTypedefs.contains( type ) ) mListTypedefs.append( type );
}

void Creator::createListTypedefs()
{
  QStringList::ConstIterator it;
  for( it = mListTypedefs.constBegin(); it != mListTypedefs.constEnd(); ++it ) {
    KODE::Class c = mFile.findClass( *it );
    if ( !c.isValid() ) continue;
    c.addTypedef( KODE::Typedef( "QList<" + *it + '>', "List" ) );
    mFile.insertClass( c );
  }
}

void Creator::setDtd( const QString &dtd )
{
  mDtd = dtd;
}

void Creator::createFileWriter( const Schema::Element &element )
{
  WriterCreator writerCreator( mFile, mDocument, mDtd );
  writerCreator.createFileWriter( Namer::getClassName( element ),
    errorStream() );
}

void Creator::createFileParser( const Schema::Element &element )
{
  ParserCreator *parserCreator = 0;

  switch ( mXmlParserType ) {
    case XmlParserDom:
    case XmlParserDomExternal:
      parserCreator = new ParserCreatorDom( this );
      break;
    case XmlParserCustomExternal:
      parserCreator = new ParserCreatorCustom( this );
      break;
  }

  parserCreator->createFileParser( element );
  parserCreator->createStringParser( element );

  delete parserCreator;
}

void Creator::printFiles( KODE::Printer &printer )
{
  if ( externalParser() ) {
    KODE::File parserFile( file() );
    parserFile.setFilename( mBaseName + "_parser" );

    parserFile.clearCode();

    mParserClass.addHeaderInclude( file().filenameHeader() );
    parserFile.insertClass( mParserClass );

    if ( mVerbose ) {
      kDebug() <<"Print external parser header" << parserFile.filenameHeader();
    }
    printer.printHeader( parserFile );
    if ( mVerbose ) {
      kDebug() <<"Print external parser implementation"
        << parserFile.filenameImplementation();
    }
    printer.printImplementation( parserFile );
  }

  if ( mVerbose ) {
    kDebug() <<"Print header" << file().filenameHeader();
  }
  printer.printHeader( file() );

  if ( mVerbose ) {
    kDebug() <<"Print implementation" << file().filenameImplementation();
  }
  printer.printImplementation( file() );

}

bool Creator::externalParser() const
{
  return mXmlParserType == XmlParserDomExternal ||
         mXmlParserType == XmlParserCustomExternal;
}

bool Creator::externalWriter() const
{
  return mXmlWriterType == XmlWriterCustomExternal;
}

const Schema::Document &Creator::document() const
{
  return mDocument;
}

void Creator::setParserClass( const KODE::Class &c )
{
  mParserClass = c;
}

KODE::Class &Creator::parserClass()
{
  return mParserClass;
}

QString Creator::errorStream() const
{
  if ( useKde() ) {
    return "kError()";
  } else {
    return "qCritical()";
  }
}

QString Creator::debugStream() const
{
  if ( useKde() ) {
    return "kDebug()";
  } else {
    return "qDebug()";
  }
}

void Creator::create()
{
  Schema::Element startElement = mDocument.startElement();
  setExternalClassPrefix( KODE::Style::upperFirst( startElement.name() ) );
  createFileParser( startElement );
//  setDtd( schemaFilename.replace( "rng", "dtd" ) );
  createFileWriter( startElement );

  createListTypedefs();
}

void Creator::setFilename( const QString& baseName )
{
  mFile.setFilename( baseName );
  mBaseName = baseName;
}


ParserCreator::ParserCreator( Creator *c )
  : mCreator( c )
{
}

ParserCreator::~ParserCreator()
{
}

Creator *ParserCreator::creator() const
{
  return mCreator;
}
