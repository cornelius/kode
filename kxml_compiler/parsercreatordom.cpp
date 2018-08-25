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

#include "namer.h"
#include "style.h"

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

  c.addHeaderInclude( "QDomElement" );

  parser.addArgument( "const QDomElement &element" );
  parser.addArgument( "bool *ok" );

  KODE::Code code;

  code += "if ( element.tagName() != \"" + e.name() + "\" ) {";
  code.indent();
  code += creator()->errorStream() + " << \"Expected '" + e.name() +
    "', got '\" << element.tagName() << \"'.\";";
  code += "if ( ok ) *ok = false;";
  code += "return " + c.name() + "();";
  code.unindent();
  code += '}';
  code.newLine();

  code += c.name() + " result = " + c.name() + "();";
  code.newLine();

  if ( e.hasElementRelations() ) {
    code += "QDomNode n;";
    code += "for( n = element.firstChild(); !n.isNull();"
                " n = n.nextSibling() ) {";
    code.indent();
    code += "QDomElement e = n.toElement();";

    Schema::Relation::List elementRelations = e.elementRelations();
    Schema::Relation::List::ConstIterator it;
    for( it = elementRelations.constBegin(); it != elementRelations.constEnd(); ++it ) {
      QString condition;
      if ( it != elementRelations.constBegin() ) condition = "else ";
      condition += "if";

      code += condition + " ( e.tagName() == \"" + (*it).target() + "\" ) {";
      code.indent();

      QString className = Namer::getClassName( (*it).target() );

      Schema::Element targetElement =
        creator()->document().element( (*it).target() );

      if ( targetElement.text() && !targetElement.hasAttributeRelations() &&
           !(*it).isList() ) {
        QString data = stringToDataConverter( "e.text()", targetElement.type() );
        code += "result.set" + className + "( " + data + " );";
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

        if ( (*it).isList() ) {
          code += "if ( ok ) result.add" + className + "( o );";
        } else {
          code += "if ( ok ) result.set" + className + "( o );";
        }
      }

      code.unindent();
      code += '}';
    }

    code.unindent();
    code += '}';
    code.newLine();
  }
  
  if ( e.text() ) {
    code += "result.setValue( " + stringToDataConverter( "element.text()", e.type() ) + " );";
  }

  foreach( Schema::Relation r, e.attributeRelations() ) {
    Schema::Attribute a = creator()->document().attribute( r );

    if (a.enumerationValues().count()) {
      QString enumName = Namer::sanitize( a.name() );

      if (!a.required()) { // if not required generate conditions
        code += "if (element.hasAttribute(\"" + a.name() + "\"))  {";
        code.indent();
      }
      code += Namer::getClassName(a.name()) + "Enum" + " " + enumName+ " = " +
              KODE::Style::lowerFirst(Namer::getClassName(a.name())) + "EnumFromString( element.attribute( \"" + a.name() + "\" ), ok  );";
      code += "if (ok && *ok == false) {";
      code.indent();
      code += "qCritical() << \"Invalid string: \\\"\" << element.attribute( \"" + a.name() + "\" ) << \"\\\" in the \\\"" + a.name() + "\\\" element\";";
      code += "return " + c.name() + "();";
      code.unindent();
      code += "} else {";
      code.indent();
      code += "result.set" + Namer::getClassName( a.name() ) +
              "( " + enumName + " );";
      code.unindent();
      code += "}";

      if (!a.required()) {
        code.unindent();
        code += "} else {";
        code.indent();
        code += "result.set" + Namer::getClassName(a.name()) + "(" + KODE::Style::lowerFirst(Namer::getClassName(a.name())) + "EnumFromString(\""+a.defaultValue()+"\"));";
        code.unindent();
        code += "}";
      }
    } else {
      QString data = stringToDataConverter( "element.attribute( \"" + a.name() + "\" )", a.type() );

      code += "result.set" + Namer::getClassName( a.name() ) +
              "( " + data + " );";
    }
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
//   kDebug() <<"Creator::createFileParserDom()";

  QString className = Namer::getClassName( element );

  KODE::Class c;

  if ( creator()->externalParser() ) {
    c = creator()->parserClass();
  } else {
    c = creator()->file().findClass( className );
  }

  if ( creator()->useKde() ) {
    c.addInclude( "kdebug.h" );
  } else {
    c.addInclude( "QtDebug" );
  } 

  KODE::Function parser( "parseFile", className );
  parser.setStatic( true );

  parser.addArgument( "const QString &filename" );
  parser.addArgument( "bool *ok" );

  c.addInclude( "QFile" );
  c.addInclude( "QDomDocument" );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( QIODevice::ReadOnly ) ) {";
  code += "  " + creator()->errorStream() + " << \"Unable to open file '\" << filename << \"'\";";
  code += "  if ( ok ) *ok = false;";
  code += "  return " + className + "();";
  code += '}';
  code += "";
  code += "QString errorMsg;";
  code += "int errorLine, errorCol;";
  code += "QDomDocument doc;";
  code += "if ( !doc.setContent( &file, false, &errorMsg, &errorLine, &errorCol ) ) {";
  code += "  " + creator()->errorStream() + " << errorMsg << \" at \" << errorLine << \",\" << errorCol;";
  code += "  if ( ok ) *ok = false;";
  code += "  return " + className + "();";
  code += '}';

  code.newLine();

  code += "bool documentOk;";
  QString line = className + " c = parseElement";
  if ( creator()->externalParser() ) line += className;
  line += "( doc.documentElement(), &documentOk );";
  code += line;

  code += "if ( ok ) {";
  code += "  *ok = documentOk;";
  code += '}';
  code += "return c;";

  parser.setBody( code );

  c.addFunction( parser );

  if ( creator()->externalParser() ) {
    creator()->setParserClass( c );
  } else {
    creator()->file().insertClass( c );
  }
}

void ParserCreatorDom::createStringParser( const Schema::Element &element )
{
  QString className = Namer::getClassName( element );

  KODE::Class c;

  if ( creator()->externalParser() ) {
    c = creator()->parserClass();
  } else {
    c = creator()->file().findClass( className );
  }

  if ( creator()->useKde() ) {
    c.addInclude( "kdebug.h" );
  } else {
    c.addInclude( "QtDebug" );
  } 

  KODE::Function parser( "parseString", className );
  parser.setStatic( true );

  parser.addArgument( "const QString &xml" );
  parser.addArgument( "bool *ok" );

  c.addInclude( "QFile" );
  c.addInclude( "QDomDocument" );

  KODE::Code code;

  code += "QString errorMsg;";
  code += "int errorLine, errorCol;";
  code += "QDomDocument doc;";
  code += "if ( !doc.setContent( xml, false, &errorMsg, &errorLine, &errorCol ) ) {";
  code += "  " + creator()->errorStream() + " << errorMsg << \" at \" << errorLine << \",\" << errorCol;";
  code += "  if ( ok ) *ok = false;";
  code += "  return " + className + "();";
  code += '}';

  code.newLine();

  code += "bool documentOk;";
  QString line = className + " c = parseElement";
  if ( creator()->externalParser() ) line += className;
  line += "( doc.documentElement(), &documentOk );";
  code += line;

  code += "if ( ok ) {";
  code += "  *ok = documentOk;";
  code += '}';
  code += "return c;";

  parser.setBody( code );

  c.addFunction( parser );

  if ( creator()->externalParser() ) {
    creator()->setParserClass( c );
  } else {
    creator()->file().insertClass( c );
  }
}

QString ParserCreatorDom::stringToDataConverter( const QString &data,
  Schema::Node::Type type )
{
  QString converter;
  if ( type == Schema::Element::Integer ) {
    converter = data + ".toInt()";
  } else if ( type == Schema::Element::Decimal ) {
    converter = data + ".toDouble()";
  } else if ( type == Schema::Element::Date ) {
    converter = "QDate::fromString( " + data + ", \"yyyyMMdd\" )";
  } else if ( type == Schema::Element::DateTime ) {
    converter = "QDateTime::fromString( " + data + ", \"yyyyMMddThhmmssZ\" )";
  } else {
    converter = data;
  }
  return converter;
}
