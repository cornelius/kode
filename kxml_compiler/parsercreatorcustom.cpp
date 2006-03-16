/*
    This file is part of KDE.

    Copyright (c) 2004-2006 Cornelius Schumacher <schumacher@kde.org>

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

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>
#include <QList>

#include <iostream>

ParserCreatorCustom::ParserCreatorCustom( Creator *c )
  : ParserCreator( c )
{
}

void ParserCreatorCustom::createTextElementParser( KODE::Class &,
  const Schema::Element &e )
{
  KODE::Function parser( "parseElement" + creator()->upperFirst( e.name() ),
    "QString" );

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
  stateCode += "} else if ( foundText" + creator()->upperFirst( e.name() ) +
    "() ) {";
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

  creator()->parserClass().addFunction( parser );
}

void ParserCreatorCustom::createElementParser( KODE::Class &c,
  const Schema::Element &e )
{
  KODE::Function parser( "parseElement" + creator()->upperFirst( e.name() ),
                         c.name() );

  KODE::Code code;

  code += c.name() + " result;";
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
      Schema::Element element = creator()->document().element( r );
      createFoundTextFunction( element.name() );

      QString eName = creator()->upperFirst( element.name() );
      stateCode += "} else if ( foundText" + eName + "() ) {";
      QString line = "  result.";
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
        Schema::Attribute a = creator()->document().attribute( *it2 );
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
      Schema::Attribute a = creator()->document().attribute( r );
      code += "bool found" + creator()->upperFirst( a.name() ) + " = false;";
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
        Schema::Attribute a = creator()->document().attribute( *it2 );
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

  creator()->parserClass().addFunction( parser );
}

KODE::Code ParserCreatorCustom::createAttributeScanner( const Schema::Attribute &a,
  bool firstAttribute )
{
  KODE::Code code;

  QString aName = creator()->upperFirst( a.name() );

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
  code += "    result.set" + aName + "( mBuffer.mid( attrValueStart,";
  code += "                                  mRunning - attrValueStart ) );";
  code += "    attrValueStart = -1;";
  code += "    found" + aName + " = false;";
  code += "  }";

  return code;
}

void ParserCreatorCustom::createFileParser( const Schema::Element &element )
{
  kDebug() << "Creator::createFileParserCustom()" << endl;

  QString className = creator()->upperFirst( element.name() );

  KODE::Function parser( "parseFile", className );

  parser.addArgument( "const QString &filename" );

  creator()->parserClass().addInclude( "qfile.h" );
  creator()->parserClass().addInclude( "kdebug.h" );
  creator()->parserClass().addInclude( "QTextStream" );

  creator()->parserClass().addMemberVariable( KODE::MemberVariable( "mBuffer",
                                                        "QString" ) );
  creator()->parserClass().addMemberVariable( KODE::MemberVariable( "mRunning",
                                                        "int" ) );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( QIODevice::ReadOnly ) ) {";
  code += "  kError() << \"Unable to open file '\" << filename << \"'\" << endl;";
  code += "  return " + className + "();";
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

  code += className + " " + element.name() + ";";
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

  creator()->parserClass().addFunction( parser );
}

void ParserCreatorCustom::createFoundTextFunction( const QString &text )
{
  QString functionName = "foundText" + creator()->upperFirst( text );

  if ( creator()->parserClass().hasFunction( functionName ) ) return;

  KODE::Function f( functionName, "bool" );

  KODE::Code code;

  code += "if ( mBuffer[ mRunning ] != '" + text.right( 1 ) + "' ) return false;";
  code += "";
  code += "return mBuffer.mid( mRunning - " +
          QString::number( text.length() - 1 ) + ", " +
          QString::number( text.length() ) + " ) == \"" + text + "\";";

  f.setBody( code );

  creator()->parserClass().addFunction( f );
}
