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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "creator.h"

#include <kode/kode.h>
#include <kode/printer.h>
#include <kode/typedef.h>

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

#include <iostream>

Creator::Creator()
{
}

KODE::File &Creator::file() { return mFile; }

QString Creator::upperFirst( const QString &str )
{
  if ( str.isEmpty() ) return str;
  return str[ 0 ].upper() + str.mid( 1 );
}

QString Creator::lowerFirst( const QString &str )
{
  if ( str.isEmpty() ) return str;
  return str[ 0 ].lower() + str.mid( 1 );
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

void Creator::createElementFunctions( KODE::Class &c, Element *e )
{
  if ( e->hasText ) {
    createProperty( c, "QString", e->name );
    return;
  }

  QString type = upperFirst( e->name );

  if ( !mFile.hasClass( type ) ) {
    createClass( e );
  }

  QString name = lowerFirst( e->name );

  if ( e->pattern.oneOrMore || e->pattern.zeroOrMore ) {
    registerListTypedef( type );

    c.addInclude( "qvaluelist.h" );
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

void Creator::createClass( Element *element )
{
  QString className = upperFirst( element->name );

  if ( mProcessedClasses.find( className ) != mProcessedClasses.end() ) {
    return;
  }

  KODE::Class c( className );

  mProcessedClasses.append( className );

  QValueList<Attribute *>::ConstIterator itA;
  for( itA = element->attributes.begin();
       itA != element->attributes.end(); ++itA ) {
    Attribute *a = *itA;

    createProperty( c, "QString", a->name );
  }

  QValueList<Element *>::ConstIterator itE;
  for( itE = element->elements.begin(); itE != element->elements.end();
       ++itE ) {
    createElementFunctions( c, *itE );
  }

  QValueList<Reference *>::ConstIterator itR;
  for( itR = element->references.begin(); itR != element->references.end();
       ++itR ) {
    Element e;
    e.name = (*itR)->name;
    e.pattern = (*itR)->pattern;
    createElementFunctions( c, &e );
  }

  createElementParser( c, element );
  createElementWriter( c, element );

  mFile.insertClass( c );
}

void Creator::createElementWriter( KODE::Class &c, Element *element )
{
  KODE::Function writer( "writeElement", "QString" );
  
  KODE::Code code;
  
  code += "QString xml;";
  
  QString tag = "<" + element->name;

  QValueList<Attribute *>::ConstIterator it3;
  for( it3 = element->attributes.begin(); it3 != element->attributes.end();
        ++it3 ) {
    tag += " " + (*it3)->name + "=\\\"\" + " + (*it3)->name + "() + \"\\\"";
  }

  if ( element->isEmpty ) {
    tag += "/";
  }

  tag += ">\\n";

  code += "xml += indent() + \"" + tag + "\";";

  if ( !element->isEmpty ) {
    code += "indent( 2 );";

    QValueList<Element *>::ConstIterator it;
    for( it = element->elements.begin(); it != element->elements.end(); ++it ) {
      Element *e = *it;
      QString type = upperFirst( e->name );
      if ( e->pattern.oneOrMore || e->pattern.zeroOrMore ) {
        code += type + "::List list = " + e->name + "List();";
        code += type + "::List::ConstIterator it;";
        code += "for( it = list.begin(); it != list.end(); ++it ) {";
        code.indent();
        code += "xml += (*it)->writeElement();";
        code.unindent();
        code += "}";
      } else {
        if ( e->hasText ) {
          code += "xml += indent() + \"<" + e->name + ">\" + " + e->name + "() + \"</" +
                  e->name + ">\\n\";";
        } else {
          code += "xml += " + type + "()->writeElement()";
        }
      }
    }

    QValueList<Reference *>::ConstIterator it2;
    for( it2 = element->references.begin(); it2 != element->references.end();
         ++it2 ) {
      Reference *r = *it2;
      QString type = upperFirst( r->name );
      if ( r->pattern.oneOrMore || r->pattern.zeroOrMore ) {
        code += type + "::List list2 = " + r->name + "List();";
        code += type + "::List::ConstIterator it2;";
        code += "for( it2 = list2.begin(); it2 != list2.end(); ++it2 ) {";
        code.indent();
        code += "xml += (*it2)->writeElement();";
        code.unindent();
        code += "}";
      } else {
        code += "xml += " + type + "()->writeElement()";
      }
    }

    code += "indent( -2 );";

    code += "xml += indent() + \"</" + element->name + ">\\n\";";
  }

  code += "return xml;";

  writer.setBody( code );
  
  c.addFunction( writer );
}

void Creator::createElementParser( KODE::Class &c, Element *e )
{
  KODE::Function parser( "parseElement", c.name() + " *" );
  parser.setStatic( true );
  parser.setDocs( "Parse XML object from DOM element." );

  parser.addArgument( "const QDomElement &element" );

  KODE::Code code;

  code += "if ( element.tagName() != \"" + e->name + "\" ) {";
  code.indent();
  code += "kdError() << \"Expected '" + e->name + "', got '\" << " +
          "element.tagName() << \"'.\" << endl;";
  code += "return 0;";
  code.unindent();
  code += "}";
  code.newLine();

  code += c.name() + " *result = new " + c.name() + "();";
  code.newLine();

  code += "QDomNode n;";
  code += "for( n = element.firstChild(); !n.isNull();"
              " n = n.nextSibling() ) {";
  code.indent();
  code += "QDomElement e = n.toElement();";

  QValueList<Element *>::ConstIterator it;
  for( it = e->elements.begin(); it != e->elements.end(); ++it ) {
    QString condition;
    if ( it != e->elements.begin() ) condition = "else ";
    condition += "if";

    code += condition + " ( e.tagName() == \"" + (*it)->name + "\" ) {";
    code.indent();

    QString className = upperFirst( (*it)->name );

    if ( (*it)->hasText ) {
      code += "result->set" + className + "( e.text() );"; 
    } else {
      code += className + " *o = " + className + "::parseElement( e );";
      code += "if ( o ) result->add" + className + "( o );";
    }

    code.unindent();
    code += "}";
  }

  code.newLine();

  QValueList<Reference *>::ConstIterator it3;
  for( it3 = e->references.begin(); it3 != e->references.end(); ++it3 ) {
    QString condition;
    if ( it3 != e->references.begin() ) condition = "else ";
    condition += "if";

    code += condition + " ( e.tagName() == \"" + (*it3)->name + "\" ) {";
    code.indent();

    QString className = upperFirst( (*it3)->name );

    code += className + " *o = " + className + "::parseElement( e );";
    code += "if ( o ) result->add" + className + "( o );";

    code.unindent();
    code += "}";
  }

  code.unindent();
  code += "}";
  code.newLine();

  QValueList<Attribute *>::ConstIterator it2;
  for( it2 = e->attributes.begin(); it2 != e->attributes.end(); ++it2 ) {
    code += "result->set" + upperFirst( (*it2)->name ) +
            "( element.attribute( \"" + (*it2)->name + "\" ) );";
  }
  code.newLine();

  code += "return result;";

  parser.setBody( code );

  c.addFunction( parser );
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
    c.addTypedef( KODE::Typedef( "QValueList<" + *it + " *>", "List" ) );
    mFile.insertClass( c );
  }
}

void Creator::createFileWriter( Element *element, const QString &dtd )
{
  QString className = upperFirst( element->name );

  KODE::Class c = mFile.findClass( className );

  KODE::Function indenter( "indent", "QString" );
  indenter.addArgument( "int n = 0" );
  
  KODE::Code code;

  code += "static int i = 0;";
  code += "i += n;";
  code += "QString space;";
  code += "return space.fill( ' ', i );"; 

  indenter.setBody( code );

  mFile.addFileFunction( indenter );


  KODE::Function writer( "writeFile", "bool" );

  writer.addArgument( "const QString &filename" );

  c.addInclude( "qfile.h" );

  code.clear();

  code += "QFile file( filename );";
  code += "if ( !file.open( IO_WriteOnly ) ) {";
  code += "  kdError() << \"Unable to open file '\" << filename << \"'\" << endl;";
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

void Creator::createFileParser( Element *element )
{
  QString className = upperFirst( element->name );

  KODE::Class c = mFile.findClass( className );

  KODE::Function parser( "parseFile", className + " *" );
  parser.setStatic( true );

  parser.addArgument( "const QString &filename" );

  c.addInclude( "qfile.h" );
  c.addInclude( "qdom.h" );
  c.addInclude( "kdebug.h" );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( IO_ReadOnly ) ) {";
  code += "  kdError() << \"Unable to open file '\" << filename << \"'\" << endl;";
  code += "  return 0;";
  code += "}";
  code += "";
  code += "QString errorMsg;";
  code += "int errorLine, errorCol;";
  code += "QDomDocument doc;";
  code += "if ( !doc.setContent( &file, false, &errorMsg, &errorLine, &errorCol ) ) {";
  code += "  kdError() << errorMsg << \" at \" << errorLine << \",\" << errorCol << endl;";
  code += "  return 0;";
  code += "}";
  code += "";
  code += "kdDebug() << \"CONTENT:\" << doc.toString() << endl;";

  code += "";
  code += className + " *c = parseElement( doc.documentElement() );";
  code += "return c;";

  parser.setBody( code );

  c.addFunction( parser );

  mFile.insertClass( c );
}
