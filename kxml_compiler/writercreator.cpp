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

  writer.addArgument( "QXmlStreamWriter &xml" );

  KODE::Code code;

  QString tag = element.name();

  if ( element.isEmpty() ) {
    code += "xml.writeEmptyElement( \"" + tag + "\" );";
  } else if ( element.text() ) {
    if ( element.type() == Schema::Element::Date ) {
      code += "if ( date().isValid() ) {";
    } else {
      code += "if ( !text().isEmpty() ) {";
    }
    code += "  xml.writeStartElement( \"" + tag + "\" );";
    foreach( Schema::Relation r, element.attributeRelations() ) {
      Schema::Attribute a = mDocument.attribute( r );
      code += "  xml.writeAttribute( \"" + a.name() + "\", " +
        Namer::getAccessor( a ) + "() );";
    }
    if ( element.type() == Schema::Element::Date ) {
      code += "  xml.writeCharacters( date().toString( \"yyyyMMdd\" ) );";
    } else {
      code += "  xml.writeCharacters( text() );";
    }
    code += "  xml.writeEndElement();";
    code += "}";
  } else {
    code += "xml.writeStartElement( \"" + tag + "\" );";

    foreach( Schema::Relation r, element.attributeRelations() ) {
      Schema::Attribute a = mDocument.attribute( r );
      code += "xml.writeAttribute( \"" + a.name() + "\", " +
        Namer::getAccessor( a ) + "() );";
    }

    foreach( Schema::Relation r, element.elementRelations() ) {
      QString type = Namer::getClassName( r.target() );
      if ( r.isList() ) {
        code += "foreach( " + type + " e, " +
          Namer::getAccessor( r.target() ) + "List() ) {";
        code.indent();
        code += "e.writeElement( xml );";
        code.unindent();
        code += '}';
      } else {
        Schema::Element e = mDocument.element( r );
        QString data;
        QString accessor = Namer::getAccessor( e ) + "()";
        if ( e.type() == Schema::Element::Integer ) {
          data = "QString::number( " + accessor + " )";
        } else if ( e.type() == Schema::Element::Date ) {
          data = accessor + ".toString( Qt::ISODate )";
        } else {
          data = accessor;
        }
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
  }

  writer.setBody( code );

  c.addFunction( writer );
}
