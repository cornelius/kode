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

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>
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
  accessor.addBodyLine( "return " + v.name() + ";" );
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
      createTextElementParserCustom( c, targetElement );
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
    adder.addArgument( className + " *v" );

    KODE::Code code;
    code += "m" + upperFirst( name ) + ".append( v );";

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

  QString tag = "<" + element.name();

  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );

    tag += " " + a.name() + "=\\\"\" + " + a.name() + "() + \"\\\"";
  }

  if ( element.isEmpty() ) {
    tag += "/";
  }

  tag += ">\\n";

  code += "xml += indent() + \"" + tag + "\";";

  if ( !element.isEmpty() ) {
    code += "indent( 2 );";

    foreach( Schema::Relation r, element.elementRelations() ) {
      QString type = upperFirst( r.target() );
      if ( r.isList() ) {
        code += "foreach( " + type + " *e, " + r.target() + "List() ) {";
        code.indent();
        code += "xml += e->writeElement();";
        code.unindent();
        code += "}";
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
  switch ( mXmlParserType ) {
    case XmlParserDom:
    case XmlParserDomExternal:
      {
      ParserCreatorDom parserCreator( this );
      parserCreator.createElementParser( c, e );
      break;
      }
    case XmlParserCustomExternal:
      createElementParserCustom( c, e );
      break;
  }
}

void Creator::createTextElementParserCustom( KODE::Class &,
  const Schema::Element &e )
{
  KODE::Function parser( "parseElement" + upperFirst( e.name() ), "QString" );

  KODE::Code code;

  code += "QString result;";
  code.newLine();

  KODE::StateMachine sm;

  KODE::Code stateCode;

  stateCode += "if ( c == '<' ) {";
  stateCode += "  state = TAG;";
  stateCode += "  tagStart = mRunning;";
  stateCode += "} else if ( c == '&' ) {";
  stateCode += "  entityStart = mRunning + 1;";
  stateCode += "} else if ( entityStart >= 0 && c == ';' ) {";
  stateCode += "  QString entity = mBuffer.mid( entityStart, mRunning - entityStart );";
  stateCode += "  if ( entity == \"quot\" ) result += '\"';";
  stateCode += "  entityStart = -1;";
  stateCode += "} else if ( entityStart < 0 ) {";
  stateCode += "  result += c;";
  stateCode += "}";

  sm.setState( "TEXT", stateCode );

  stateCode.clear();
  stateCode += "if ( c == '/' ) {";
  stateCode += "  state = ENDTAG;";
  stateCode += "} else {";
  stateCode += "  state = STARTTAG;";
  stateCode += "}";

  sm.setState( "TAG", stateCode );

  stateCode.clear();
  stateCode += "if ( c == '>' ) {";
  stateCode += "  state = TEXT;";
  stateCode += "  result += mBuffer.mid( tagStart, mRunning - tagStart + 1 );";
  stateCode += "}";

  sm.setState( "STARTTAG", stateCode );

  stateCode.clear();
  stateCode += "if ( c == '>' ) {";
  stateCode += "  state = TEXT;";
  stateCode += "  result += mBuffer.mid( tagStart, mRunning - tagStart + 1 );";
  stateCode += "} else if ( foundText" + upperFirst( e.name() ) + "() ) {";
  stateCode += "  return result;";
  stateCode += "}";

  sm.setState( "ENDTAG", stateCode );

  sm.setInitialState( "STARTTAG" );

  code.addBlock( sm.stateDefinition() );
  code.newLine();
  code += "int tagStart = -1;";
  code += "int entityStart = -1;";
  code.newLine();
  code += "while ( mRunning < mBuffer.length() ) {";
  code.indent();
  code += "QChar c = mBuffer[ mRunning ];";
  code.addBlock( sm.transitionLogic() );
  code += "++mRunning;";
  code.unindent();
  code += "}";
  code.newLine();
  code += "return result;";

  parser.setBody( code );

  mParserClass.addFunction( parser );
}

void Creator::createElementParserCustom( KODE::Class &c,
  const Schema::Element &e )
{
  KODE::Function parser( "parseElement" + upperFirst( e.name() ),
                         c.name() + " *" );

  KODE::Code code;

  code += c.name() + " *result = new " + c.name() + "();";
  code.newLine();

  KODE::StateMachine sm;

  if ( !e.isEmpty() ) {
    KODE::Code stateCode;
    stateCode += "if ( c == '<' ) state = TAG;";

    sm.setState( "WHITESPACE", stateCode );

    stateCode.clear();
    stateCode += "if ( c == '/' ) {";
    stateCode += "  state = ENDTAG;";
    stateCode += "} else {";
    stateCode += "  state = STARTTAG;";
    stateCode += "}";

    sm.setState( "TAG", stateCode );

    stateCode.clear();
    if ( e.attributeRelations().isEmpty() ) {
      stateCode += " if ( c == '/' ) {";
      stateCode += "    return result;";
      stateCode += " }";
    }
    stateCode += "if ( c == '>' ) {";
    stateCode += "  state = WHITESPACE;";
    
    foreach( Schema::Relation r, e.elementRelations() ) {
      Schema::Element element = mDocument.element( r );
      createFoundTextFunction( element.name() );

      QString eName = upperFirst( element.name() );
      stateCode += "} else if ( foundText" + eName + "() ) {";
      QString line = "  result->";
      if ( element.text() ) line += "set";
      else line += "add";
      line += eName + "( parseElement" + eName + "() );";
      stateCode += line;
      stateCode += "  state = WHITESPACE;";
    }
    stateCode += "}";

    sm.setState( "STARTTAG", stateCode );

    stateCode.clear();
    stateCode += "if ( c == '>' ) {";
    stateCode += "  state = WHITESPACE;";
    stateCode += "} else if ( foundText" + c.name() + "() ) {";
    stateCode += "  return result;";
    stateCode += "}";

    sm.setState( "ENDTAG", stateCode );

    Schema::Relation::List attributeRelations = e.attributeRelations();
    if ( !attributeRelations.isEmpty() ) {
      stateCode.clear();
      stateCode += "if ( c == '>' ) {";
      stateCode += "  state = WHITESPACE;";
      stateCode += "}";

      Schema::Relation::List::ConstIterator it2;
      for( it2 = attributeRelations.begin(); it2 != attributeRelations.end();
        ++it2 ) {
        bool first = it2 == attributeRelations.begin();
        Schema::Attribute a = mDocument.attribute( *it2 );
        stateCode.addBlock( createAttributeScanner( a, first ) );
      }
      stateCode += "} else if ( c =='/' ) {";
      stateCode += "  return result;";
      stateCode += "}";

      sm.setState( "ATTRIBUTES", stateCode );

      sm.setInitialState( "ATTRIBUTES" );
    } else {
      sm.setInitialState( "STARTTAG" );
    }

    code.addBlock( sm.stateDefinition() );
    code.newLine();
  }

  if ( !e.attributeRelations().isEmpty() ) {
    foreach( Schema::Relation r, e.attributeRelations() ) {
      Schema::Attribute a = mDocument.attribute( r );
      code += "bool found" + upperFirst( a.name() ) + " = false;";
    }
    code.newLine();
    code += "int attrValueStart = -1;";
    code.newLine();
  }

  code += "while ( mRunning < mBuffer.length() ) {";
  code.indent();
  code += "QChar c = mBuffer[ mRunning ];";

  if ( e.isEmpty() ) {
    code += "if ( c == '>' ) {";
    code += "  return result;";
    code += "}";

    Schema::Relation::List attributeRelations = e.attributeRelations();
    if ( !attributeRelations.isEmpty() ) {
      Schema::Relation::List::ConstIterator it2;
      for( it2 = attributeRelations.begin(); it2 != attributeRelations.end();
        ++it2 ) {
        Schema::Attribute a = mDocument.attribute( *it2 );
        code.addBlock( createAttributeScanner( a,
          it2 == attributeRelations.begin() ) );
      }
      code += "}";
    }
  } else {
    code.addBlock( sm.transitionLogic() );
  }

  code += "++mRunning;";
  code.unindent();
  code += "}";
  code.newLine();
  code += "return result;";

  parser.setBody( code );

  mParserClass.addFunction( parser );
}

KODE::Code Creator::createAttributeScanner( const Schema::Attribute &a,
  bool firstAttribute )
{
  KODE::Code code;

  QString aName = upperFirst( a.name() );

  createFoundTextFunction( a.name() );

  QString line;
  if ( !firstAttribute ) line = "} else ";
  line += "if ( foundText" + aName + "() ) {";
  code += line;
  code += "  found" + aName + "= true;";
  code += "} else if ( found" + aName + " && c == '\"' ) {";
  code += "  if ( attrValueStart < 0 ) {";
  code += "    attrValueStart = mRunning + 1;";
  code += "  } else {";
  code += "    result->set" + aName + "( mBuffer.mid( attrValueStart,";
  code += "                                  mRunning - attrValueStart ) );";
  code += "    attrValueStart = -1;";
  code += "    found" + aName + " = false;";
  code += "  }";

  return code;
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
    c.addTypedef( KODE::Typedef( "QList<" + *it + " *>", "List" ) );
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

void Creator::createFileWriter( const Schema::Element &element,
  const QString &dtd )
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
  code += "}";
  code += "";
  code += "QTextStream ts( &file );";

  code += "ts << \"<?xml version=\\\"1.0\\\" encoding=\\\"UTF-8\\\"?>\\n\";";
  code += "ts << \"<!DOCTYPE features SYSTEM \\\"" + dtd + "\\\">\\n\";";

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
  switch ( mXmlParserType ) {
    case XmlParserDom:
    case XmlParserDomExternal:
      {
      ParserCreatorDom parserCreator( this );
      parserCreator.createFileParser( element );
      break;
      }
    case XmlParserCustomExternal:
      createFileParserCustom( element );
      break;
  }
}

void Creator::createFileParserCustom( const Schema::Element &element )
{
  kDebug() << "Creator::createFileParserCustom()" << endl;

  QString className = upperFirst( element.name() );

  KODE::Function parser( "parseFile", className + " *" );

  parser.addArgument( "const QString &filename" );

  mParserClass.addInclude( "qfile.h" );
  mParserClass.addInclude( "kdebug.h" );
  mParserClass.addInclude( "QTextStream" );

  mParserClass.addMemberVariable( KODE::MemberVariable( "mBuffer",
                                                        "QString" ) );
  mParserClass.addMemberVariable( KODE::MemberVariable( "mRunning",
                                                        "int" ) );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( QIODevice::ReadOnly ) ) {";
  code += "  kError() << \"Unable to open file '\" << filename << \"'\" << endl;";
  code += "  return 0;";
  code += "}";
  code += "";
  code += "QTextStream ts( &file );";
  code += "mBuffer = ts.read();";
  code += "";
  code += "mRunning = 0;";
  code.newLine();

  KODE::StateMachine sm;

  KODE::Code stateCode;

  stateCode += "if ( c == '<' ) state = TAG;";

  sm.setState( "WHITESPACE", stateCode );

  stateCode.clear();

  stateCode += "if ( c == '>' ) {";
  stateCode += "  state = WHITESPACE;";
  stateCode += "} else if ( foundText" + className + "() ) {";
  stateCode += "  " + element.name() + " = parseElement" + className + "();";
  stateCode += "  state = WHITESPACE;";
  stateCode += "}";

  createFoundTextFunction( element.name() );

  sm.setState( "TAG", stateCode );

  code.addBlock( sm.stateDefinition() );
  code.newLine();

  code += className + " *" + element.name() + " = 0;";
  code.newLine();

  code += "while ( mRunning < mBuffer.length() ) {";
  code.indent();
  code += "QChar c = mBuffer[ mRunning ];";
  code.addBlock( sm.transitionLogic() );
  code += "++mRunning;";
  code.unindent();
  code += "}";
  code.newLine();

  code += "return " + element.name() + ";";

  parser.setBody( code );

  mParserClass.addFunction( parser );
}

void Creator::createFoundTextFunction( const QString &text )
{
  QString functionName = "foundText" + upperFirst( text );

  if ( mParserClass.hasFunction( functionName ) ) return;

  KODE::Function f( functionName, "bool" );

  KODE::Code code;

  code += "if ( mBuffer[ mRunning ] != '" + text.right( 1 ) + "' ) return false;";
  code += "";
  code += "return mBuffer.mid( mRunning - " +
          QString::number( text.length() - 1 ) + ", " +
          QString::number( text.length() ) + " ) == \"" + text + "\";";

  f.setBody( code );

  mParserClass.addFunction( f );
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
