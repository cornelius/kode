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
#include <ksimpleconfig.h>
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
  : mDocument( document ), mXmlParserType( p ), mXmlWriterType( w )
{
  setExternalClassNames();
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

QString Creator::upperFirst( const QString &str )
{
  return KODE::Style::upperFirst( str );
}

QString Creator::lowerFirst( const QString &str )
{
  return KODE::Style::lowerFirst( str );
}

void Creator::createProperty( KODE::Class &c, const QString &type,
  const QString &name )
{
  KODE::MemberVariable v( name, type );
  c.addMemberVariable( v );

  KODE::Function mutator( "set" + upperFirst( name ), "void" );
  mutator.addArgument( "const " + type + " &v" );
  mutator.addBodyLine( v.name() + " = v;" );
  c.addFunction( mutator );

  KODE::Function accessor( name, type );
  accessor.setConst( true );
  accessor.addBodyLine( "return " + v.name() + ';' );
  c.addFunction( accessor );
}

void Creator::createElementFunctions( KODE::Class &c, const Schema::Element &e,
  const Schema::Relation &r )
{
#if 0
  kDebug() << "Creator::createElementFunctions()" << endl;
  kDebug() << "ELEMENT " << e.identifier() << endl;
  kDebug() << "RELATION: " << r.asString() << endl;
#endif

  Schema::Element targetElement = mDocument.element( r );

  if ( targetElement.text() ) {
    createProperty( c, "QString", targetElement.name() );
    c.addHeaderInclude( "QString" );
    if ( mXmlParserType == XmlParserCustomExternal ) {
      ParserCreatorCustom parserCreatorCustom( this );
      parserCreatorCustom.createTextElementParser( c, targetElement );
    }
    return;
  }

  QString type = upperFirst( targetElement.name() );

  if ( !mFile.hasClass( type ) ) {
    createClass( e );
  }

  QString name = lowerFirst( targetElement.name() );

  if ( r.isList() ) {
    registerListTypedef( type );

    c.addHeaderInclude( "QList" );
    type = type + "::List";
    QString className = upperFirst( name );
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
  kDebug() << "Creator::createClass() " << element.identifier() << endl;

  QString className = upperFirst( element.name() );

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
      QString type = upperFirst( r.target() );
      if ( r.isList() ) {
        code += "foreach( " + type + " e, " + r.target() + "List() ) {";
        code.indent();
        code += "xml += e.writeElement();";
        code.unindent();
        code += '}';
      } else {
        Schema::Element e = mDocument.element( r );
        if ( e.text() ) {
          code += "xml += indent() + \"<" + e.name() + ">\" + " + e.name() +
            "() + \"</" + e.name() + ">\\n\";";
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
  for( it = mListTypedefs.begin(); it != mListTypedefs.end(); ++it ) {
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
  QString className = upperFirst( element.name() );

  KODE::Class c = mFile.findClass( className );

  c.addInclude( "kdebug.h" );
  c.addInclude( "qtextstream.h" );
  c.addInclude( "qfile.h" );

  if ( !externalWriter() ) {
    createIndenter( mFile );
  }

  KODE::Function writer( "writeFile", "bool" );

  writer.addArgument( "const QString &filename" );

  c.addInclude( "qfile.h" );
  c.addInclude( "QTextStream" );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( QIODevice::WriteOnly ) ) {";
  code += "  kError() << \"Unable to open file '\" << filename << \"'\" << endl;";
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

    kDebug() << "Print external parser." << endl;
    printer.printHeader( parserFile );
    printer.printImplementation( parserFile );
  }

  kDebug() << "Print header" << endl;
  printer.printHeader( file() );

  kDebug() << "Print implementation" << endl;
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
