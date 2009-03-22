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

#include "parsercreatordom.h"
#include "parsercreatorcustom.h"

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

Creator::Creator( const Schema::Document &document, XmlParserType p,
  XmlWriterType w )
  : mDocument( document ), mXmlParserType( p ), mXmlWriterType( w ),
    mVerbose( false ), mUseKde( false )
{
  setExternalClassNames();
}

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

QString Creator::upperFirst( const QString &str ) const
{
  return KODE::Style::upperFirst( str );
}

QString Creator::lowerFirst( const QString &str ) const
{
  return KODE::Style::lowerFirst( str );
}

void Creator::createProperty( KODE::Class &c, const QString &type,
  const QString &name )
{
  KODE::MemberVariable v( name, type );
  c.addMemberVariable( v );

  KODE::Function mutator( "set" + upperFirst( name ), "void" );
  if ( type == "int" ) {
    mutator.addArgument( type + " v" );
  } else {
    mutator.addArgument( "const " + type + " &v" );
  }
  mutator.addBodyLine( v.name() + " = v;" );
  c.addFunction( mutator );

  KODE::Function accessor( getAccessor( name ), type );
  accessor.setConst( true );
  accessor.addBodyLine( "return " + v.name() + ';' );
  c.addFunction( accessor );
}

void Creator::createElementFunctions( KODE::Class &c, const Schema::Element &e,
  const Schema::Relation &r )
{
#if 0
  if ( mVerbose ) {
    kDebug() <<"Creator::createElementFunctions()";
    kDebug() <<"ELEMENT" << e.identifier();
    kDebug() <<"RELATION:" << r.asString();
  }
#endif

  Schema::Element targetElement = mDocument.element( r );

  if ( targetElement.text() ) {
    if ( targetElement.type() == Schema::Element::Integer ) {
      createProperty( c, "int", getClassName( targetElement ) );
    } else if ( targetElement.type() == Schema::Element::Date ) {
      createProperty( c, "QDate", getClassName( targetElement ) );
      c.addHeaderInclude( "QDate" );
    } else {
      createProperty( c, "QString", getClassName( targetElement ) );
      c.addHeaderInclude( "QString" );
      if ( mXmlParserType == XmlParserCustomExternal ) {
        ParserCreatorCustom parserCreatorCustom( this );
        parserCreatorCustom.createTextElementParser( c, targetElement );
      }
    }
    return;
  }

  QString type = getClassName( targetElement );

  if ( !mFile.hasClass( type ) ) {
    createClass( e );
  }

  QString name = lowerFirst( type );

  if ( r.isList() ) {
    registerListTypedef( type );

    QString className = type;
    type = type += "::List";

    c.addHeaderInclude( "QList" );
    name = name + "List";

    KODE::Function adder( "add" + className, "void" );
    adder.addArgument( "const " + className + " &v" );

    KODE::Code code;
    code += 'm' + upperFirst( name ) + ".append( v );";

    adder.setBody( code );

    c.addFunction( adder );
  }

  createProperty( c, type, name );
}

void Creator::createClass( const Schema::Element &element )
{
  QString className = getClassName( element  );

  if ( mVerbose ) {
    kDebug() <<"Creator::createClass()" << element.identifier() << className;
  }

  if ( mProcessedClasses.contains( className ) )
    return;

  KODE::Class c( className );

  mProcessedClasses.append( className );

  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );
    createProperty( c, "QString", a.name() );
  }

  foreach( Schema::Relation r, element.elementRelations() ) {
    createElementFunctions( c, element, r );
  }

  createElementParser( c, element );
  createElementWriter( c, element );

  mFile.insertClass( c );
}

void Creator::createElementWriter( KODE::Class &c,
  const Schema::Element &element )
{
  KODE::Function writer( "writeElement", "QString" );

  KODE::Code code;

  code += "QString xml;";

  QString tag = '<' + element.name();

  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );

    tag += " " + a.name() + "=\\\"\" + " + a.name() + "() + \"\\\"";
  }

  if ( element.isEmpty() ) {
    tag += '/';
  }

  tag += ">\\n";

  code += "xml += indent() + \"" + tag + "\";";

  if ( !element.isEmpty() ) {
    code += "indent( 2 );";

    foreach( Schema::Relation r, element.elementRelations() ) {
      QString type = getClassName( r.target() );
      if ( r.isList() ) {
        code += "foreach( " + type + " e, " + getAccessor( r.target() ) + "List() ) {";
        code.indent();
        code += "xml += e.writeElement();";
        code.unindent();
        code += '}';
      } else {
        Schema::Element e = mDocument.element( r );
        QString data;
        QString accessor = getAccessor( e ) + "()";
        if ( e.type() == Schema::Element::Integer ) {
          data = "QString::number( " + accessor + " )";
        } else if ( e.type() == Schema::Element::Date ) {
          data = accessor + ".toString( Qt::ISODate )";
        } else {
          data = accessor;
        }
        if ( e.text() ) {
          code += "xml += indent() + \"<" + e.name() + ">\" + " +
            data +
            " + \"</" + e.name() + ">\\n\";";
        } else {
          code += "xml += " + type + "().writeElement();";
        }
      }
    }

    code += "indent( -2 );";

    code += "xml += indent() + \"</" + element.name() + ">\\n\";";
  }

  code += "return xml;";

  writer.setBody( code );

  c.addFunction( writer );
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

void Creator::createIndenter( KODE::File &file )
{
  KODE::Function indenter( "indent", "QString" );
  indenter.addArgument( "int n = 0" );

  KODE::Code code;

  code += "static int i = 0;";
  code += "i += n;";
  code += "QString space;";
  code += "return space.fill( ' ', i );";

  indenter.setBody( code );

  file.addFileFunction( indenter );
}

void Creator::setDtd( const QString &dtd )
{
  mDtd = dtd;
}

void Creator::createFileWriter( const Schema::Element &element )
{
  QString className = getClassName( element );

  KODE::Class c = mFile.findClass( className );

  c.addInclude( "QTextStream" );
  c.addInclude( "QFile" );

  if ( !externalWriter() ) {
    createIndenter( mFile );
  }

  KODE::Function writer( "writeFile", "bool" );

  writer.addArgument( "const QString &filename" );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( QIODevice::WriteOnly ) ) {";
  code += "  " + errorStream() + " << \"Unable to open file '\" << filename << \"'\";";
  code += "  return false;";
  code += '}';
  code += "";
  code += "QTextStream ts( &file );";

  code += "ts << \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\"?>\\n\";";
  if ( !mDtd.isEmpty() ) {
    code += "ts << \"<!DOCTYPE features SYSTEM \\\"" + mDtd + "\\\">\\n\";";
  }

  code += "ts << writeElement();";
  code += "file.close();";
  code += "";
  code += "return true;";

  writer.setBody( code );

  c.addFunction( writer );

  mFile.insertClass( c );
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

  delete parserCreator;
}

void Creator::printFiles( KODE::Printer &printer )
{
  if ( externalParser() ) {
    KODE::File parserFile( file() );
    parserFile.setFilename( file().filename() + "_parser" );

    parserFile.clearCode();

    mParserClass.addHeaderInclude( file().filename() + ".h" );
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

QString Creator::getClassName( const Schema::Element &element ) const
{
  return getClassName( element.name() );
}

QString Creator::getClassName( const QString &elementName ) const
{
  QString name;
  QStringList parts = elementName.split( "_" );
  foreach( QString part, parts ) {
    name += upperFirst( part );
  }
  return name;
}

QString Creator::getAccessor( const Schema::Element &element ) const
{
  return getAccessor( element.name() );
}

QString Creator::getAccessor( const QString &elementName ) const
{
  return lowerFirst( getClassName( elementName ) );
}

void Creator::create()
{
  Schema::Element startElement = mDocument.startElement();
  setExternalClassPrefix( upperFirst( startElement.name() ) );
  createFileParser( startElement );
//  setDtd( schemaFilename.replace( "rng", "dtd" ) );
  createFileWriter( startElement );

  createListTypedefs();
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

