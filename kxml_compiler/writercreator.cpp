/*
    This file is part of KDE.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#include "writercreator.h"

#include "namer.h"
#include "style.h"

#include <QDebug>

WriterCreator::WriterCreator( KODE::File &file, Schema::Document &document,
  const QString &dtd )
  : mFile( file ), mDocument( document ), mDtd( dtd )
{
}

void WriterCreator::createFileWriter( const QString &className, const QString &errorStream )
{
  KODE::Class c = mFile.findClass( className );

  c.addHeaderInclude( "QtXml/QXmlStreamWriter" );
  c.addInclude( "QtCore/QtDebug" );
  c.addInclude( "QtCore/QFile" );

  KODE::Function writer( "writeFile", "bool" );
  writer.setConst( true );

  writer.addArgument( "const QString &filename" );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( QIODevice::WriteOnly ) ) {";
  code += "  " + errorStream + " << \"Unable to open file '\" << filename << \"'\";";
  code += "  return false;";
  code += '}';
  code += "";

  code += "QXmlStreamWriter xml( &file );";
  code += "xml.setAutoFormatting( true );";
  code += "xml.setAutoFormattingIndent( 2 );";

  code += "xml.writeStartDocument( \"1.0\" );";
  if ( !mDtd.isEmpty() ) {
    code += "xml.writeDTD( \\\"" + mDtd + "\\\" );";
  }

  code += "writeElement( xml );";
  code += "xml.writeEndDocument();";
  code += "file.close();";
  code += "";
  code += "return true;";

  writer.setBody( code );

  c.addFunction( writer );

  mFile.insertClass( c );
}

void WriterCreator::createElementWriter( KODE::Class &c,
  const Schema::Element &element )
{
  KODE::Function writer( "writeElement", "void" );
  writer.setConst( true );

  writer.addArgument( "QXmlStreamWriter &xml" );

  KODE::Code code;

  QString tag = element.name();

  if ( element.isEmpty() ) {
    code += "xml.writeEmptyElement( \"" + tag + "\" );";
  } else if ( element.text() ) {
    if ( element.type() == Schema::Element::Date ) {
      code += "if ( value().isValid() ) {";
    } else {
      code += "if ( !value().isEmpty() ) {";
    }
    code += "  xml.writeStartElement( \"" + tag + "\" );";
    
    code += createAttributeWriter( element );

    QString data = dataToStringConverter( "value()", element.type() );
    code += "  xml.writeCharacters( " + data + " );";
    code += "  xml.writeEndElement();";
    code += "}";
  } else {
    bool pureList = true;
    if ( !element.attributeRelations().isEmpty() ) {
      pureList = false;
    } else {
      if ( element.elementRelations().isEmpty() ) {
        pureList = false;
      } else {
        foreach( Schema::Relation r, element.elementRelations() ) {
          if ( !r.isList() ) {
            pureList = false;
            break;
          }
        }
      }
    }

    if ( pureList ) {
      QStringList conditions;
      foreach( Schema::Relation r, element.elementRelations() ) {
        if ( r.isList() ) {
          conditions.append( "!" +
            Namer::getListAccessor( r.target() ) + "().isEmpty()" );
        }
      }
      code += "if ( " + conditions.join( " || " ) + " ) {";
      code.indent();
    }
    
    code += "xml.writeStartElement( \"" + tag + "\" );";

    code += createAttributeWriter( element );

    foreach( Schema::Relation r, element.elementRelations() ) {
      QString type = Namer::getClassName( r.target() );
      if ( r.isList() ) {
        code += "foreach( " + type + " e, " +
          Namer::getListAccessor( r.target() ) + "() ) {";
        code.indent();
        code += "e.writeElement( xml );";
        code.unindent();
        code += '}';
      } else {
        Schema::Element e = mDocument.element( r );
        QString accessor = Namer::getAccessor( e ) + "()";
        QString data = dataToStringConverter( accessor, e.type() );
        if ( e.text() && !e.hasAttributeRelations() ) {
          if ( e.type() == Schema::Element::String ) {
            code += "if ( !" + data + ".isEmpty() ) {";
            code.indent();
          }
          code += "xml.writeTextElement(  \"" + e.name() + "\", " + data +
            " );";
          if ( e.type() == Schema::Element::String ) {
            code.unindent();
            code += "}";
          }
        } else {
          code += Namer::getAccessor( r.target() ) + "().writeElement( xml );";
        }
      }
    }
    code += "xml.writeEndElement();";
    
    if ( pureList ) {
      code.unindent();
      code += "}";
    }
  }

  writer.setBody( code );

  c.addFunction( writer );
}

// FIXME: Collect in class with other type specific functions from parsercreator
// and creator
QString WriterCreator::dataToStringConverter( const QString &data,
  Schema::Node::Type type )
{
  QString converter;

  if ( type == Schema::Element::Integer || type == Schema::Element::Decimal ) {
    converter = "QString::number( " + data + " )";
  } else if ( type == Schema::Element::Boolean ) {
    converter = data + " ? \"true\" : \"false\"";
  } else if ( type == Schema::Element::Date ) {
    converter = data + ".toString( \"yyyyMMdd\" )";
  } else if ( type == Schema::Element::DateTime ) {
    converter = data + ".toString( \"yyyyMMddThhmmssZ\" )";
  } else {
    converter = data;
  }
  
  return converter;
}

KODE::Code WriterCreator::createAttributeWriter( const Schema::Element &element )
{
  KODE::Code code;

  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );

    QString data = Namer::getAccessor( a ) + "()";
    if ( a.type() != Schema::Node::Enumeration ) {
      code.addLine("xml.writeAttribute(\"" + a.name() + "\", " +
          dataToStringConverter( data, a.type() ) + " );");
    } else if ( a.type() == Schema::Node::Enumeration ) {
      code.addLine("xml.writeAttribute(\"" + a.name() + "\", " +
                   KODE::Style::lowerFirst(Namer::getClassName(a.name())) + "EnumToString( " +
                   KODE::Style::lowerFirst(Namer::getClassName(a.name())) + "() "
                                                                            "));");
    }
  }
  
  return code;
}
