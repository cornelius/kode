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

void WriterCreator::createFileWriter( const QString &className, bool externalWriter, const QString &errorStream )
{
  KODE::Class c = mFile.findClass( className );

  c.addInclude( "QtCore/QTextStream" );
  c.addInclude( "QtCore/QtDebug" );
  c.addInclude( "QtCore/QFile" );

  if ( externalWriter ) {
    createIndenter( mFile );
  }

  KODE::Function writer( "writeFile", "bool" );

  writer.addArgument( "const QString &filename" );

  KODE::Code code;

  code += "QFile file( filename );";
  code += "if ( !file.open( QIODevice::WriteOnly ) ) {";
  code += "  " + errorStream + " << \"Unable to open file '\" << filename << \"'\";";
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

void WriterCreator::createElementWriter( KODE::Class &c,
  const Schema::Element &element )
{
  KODE::Function writer( "writeElement", "QString" );

  KODE::Code code;

  code += "QString xml;";

  QString tag = '<' + element.name();

  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );

    tag += " " + a.name() + "=\\\"\" + " + Namer::getAccessor( a ) +
      "() + \"\\\"";
  }

  if ( element.isEmpty() ) {
    tag += '/';
  }

  tag += ">";

  if ( element.isEmpty() ) {
    code += "xml += indent() + \"" + tag + "\\n\";";
  } else if ( element.text() ) {
    code += "if ( !text().isEmpty() ) {";
    code += "  xml += indent() + \"" + tag + "\" + text() + \"</" +
      element.name() + ">\\n\";";
    code += "}";
  } else {
    code += "xml += indent() + \"" + tag + "\\n\";";
    code += "indent( 2 );";

    foreach( Schema::Relation r, element.elementRelations() ) {
      QString type = Namer::getClassName( r.target() );
      if ( r.isList() ) {
        code += "foreach( " + type + " e, " +
          Namer::getAccessor( r.target() ) + "List() ) {";
        code.indent();
        code += "xml += e.writeElement();";
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
          code += "xml += indent() + \"<" + e.name() + ">\" + " +
            data +
            " + \"</" + e.name() + ">\\n\";";
          if ( e.type() == Schema::Element::String ) {
            code.unindent();
            code += "}";
          }
        } else {
          code += "xml += " + Namer::getAccessor( r.target() ) +
            "().writeElement();";
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

void WriterCreator::createIndenter( KODE::File &file )
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

