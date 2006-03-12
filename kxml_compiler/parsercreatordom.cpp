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

ParserCreatorDom::ParserCreatorDom( Creator *c )
  : ParserCreator( c )
{
}

void ParserCreatorDom::createElementParser( KODE::Class &c,
  const Schema::Element &e )
{
  QString functionName;
  if ( creator()->externalParser() ) functionName = "parseElement" + c.name();
  else functionName = "parseElement";

  KODE::Function parser( functionName, c.name() );
  parser.setStatic( true );
  parser.setDocs( "Parse XML object from DOM element." );

  parser.addArgument( "const QDomElement &element" );
  parser.addArgument( "bool *ok" );

  KODE::Code code;

  code += "if ( element.tagName() != \"" + e.name() + "\" ) {";
  code.indent();
  code += "kError() << \"Expected '" + e.name() + "', got '\" << " +
          "element.tagName() << \"'.\" << endl;";
  code += "if ( ok ) *ok = false;";
  code += "return " + c.name() + "();";
  code.unindent();
  code += "}";
  code.newLine();

  code += c.name() + " result = " + c.name() + "();";
  code.newLine();

  code += "QDomNode n;";
  code += "for( n = element.firstChild(); !n.isNull();"
              " n = n.nextSibling() ) {";
  code.indent();
  code += "QDomElement e = n.toElement();";

  Schema::Relation::List elementRelations = e.elementRelations();
  Schema::Relation::List::ConstIterator it;
  for( it = elementRelations.begin(); it != elementRelations.end(); ++it ) {
    QString condition;
    if ( it != elementRelations.begin() ) condition = "else ";
    condition += "if";

    code += condition + " ( e.tagName() == \"" + (*it).target() + "\" ) {";
    code.indent();

    QString className = creator()->upperFirst( (*it).target() );

    Schema::Element targetElement =
      creator()->document().element( (*it).target() );

    if ( targetElement.text() ) {
      code += "result.set" + className + "( e.text() );";
    } else {
      code += "bool ok;";
      QString line = className + " o = ";
      if ( creator()->externalParser() ) {
        line += "parseElement" + className;
      } else {
        line += className + "::parseElement";
      }
      line += "( e, &ok );";
      code += line;

      code += "if ( ok ) result.add" + className + "( o );";
    }

    code.unindent();
    code += "}";
  }

  code.unindent();
  code += "}";
  code.newLine();

  foreach( Schema::Relation r, e.attributeRelations() ) {
    Schema::Attribute a = creator()->document().attribute( r );
    code += "result.set" + creator()->upperFirst( a.name() ) +
            "( element.attribute( \"" + a.name() + "\" ) );";
  }
  code.newLine();

  code += "if ( ok ) *ok = true;";
  code += "return result;";

  parser.setBody( code );

  if ( creator()->externalParser() ) {
    creator()->parserClass().addFunction( parser );
    creator()->parserClass().addHeaderInclude( "qdom.h" );
  } else {
    c.addFunction( parser );
  }
}

void ParserCreatorDom::createFileParser( const Schema::Element &element )
{
  kDebug() << "Creator::createFileParserDom()" << endl;

  QString className = creator()->upperFirst( element.name() );

  KODE::Class c;

  if ( creator()->externalParser() ) {
    c = creator()->parserClass();
  } else {
    c = creator()->file().findClass( className );
  }

  KODE::Function parser( "parseFile", className );
  parser.setStatic( true );

  parser.addArgument( "const QString &filename" );
  parser.addArgument( "bool *ok" );

  c.addInclude( "qfile.h" );
  c.addInclude( "qdom.h" );
  c.addInclude( "kdebug.h" );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( QIODevice::ReadOnly ) ) {";
  code += "  kError() << \"Unable to open file '\" << filename << \"'\" << endl;";
  code += "  if ( ok ) *ok = false;";
  code += "  return " + className + "();";
  code += "}";
  code += "";
  code += "QString errorMsg;";
  code += "int errorLine, errorCol;";
  code += "QDomDocument doc;";
  code += "if ( !doc.setContent( &file, false, &errorMsg, &errorLine, &errorCol ) ) {";
  code += "  kError() << errorMsg << \" at \" << errorLine << \",\" << errorCol << endl;";
  code += "  if ( ok ) *ok = false;";
  code += "  return " + className + "();";
  code += "}";
  code += "";
  code += "kDebug() << \"CONTENT:\" << doc.toString() << endl;";

  code.newLine();

  code += "bool documentOk;";
  QString line = className + " c = parseElement";
  if ( creator()->externalParser() ) line += className;
  line += "( doc.documentElement(), &documentOk );";
  code += line;

  code += "if ( ok ) {";
  code += "  *ok = documentOk;";
  code += "}";
  code += "return c;";

  parser.setBody( code );

  c.addFunction( parser );

  if ( creator()->externalParser() ) {
    creator()->setParserClass( c );
  } else {
    creator()->file().insertClass( c );
  }
}
