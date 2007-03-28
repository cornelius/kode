/*
    This file is part of KXForms.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "formcreator.h"

#include "xmlbuilder.h"

#include <QDebug>

using namespace KXForms;

FormCreator::FormCreator()
{
}

QString FormCreator::create( const Schema::Document &schemaDocument )
{
  qDebug() << "---FormCreator::create()";

  mDocument = schemaDocument;

  mCollapsedForms.clear();

  XmlBuilder xml( "kxforms" );

  createForm( &xml, schemaDocument.startElement() );

  foreach ( Schema::Element element, schemaDocument.usedElements() ) {
    if ( element.identifier() != schemaDocument.startElement().identifier() ) {
      createForm( &xml, element );
    }
  }

  qDebug() << "---FormCreator::create() done";

  return xml.print();
}

void FormCreator::createForm( XmlBuilder *xml, const Schema::Element &element )
{
  if ( mCollapsedForms.contains( element.name() ) ) return;

  qDebug() << "ELEMENT" << element.name();
  XmlBuilder *form = xml->tag( "form" )->attribute( "ref", element.name() );

  form->tag( "xf:label", humanizeString( element.name() ) );

  parseAttributes( element, form );

  parseElement( element, form );
}

void FormCreator::parseAttributes( const Schema::Element &element, XmlBuilder *xml  )
{
  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );

    qDebug() << "  ATTRIBUTE: " << a.identifier();

    if ( a.type() == Schema::Attribute::String ) {
      XmlBuilder *input = xml->tag( "xf:input" );
      input->attribute( "ref", a.ref() );
      createLabel( input, a );
    } else if ( a.type() == Schema::Attribute::Enumeration ) {
      XmlBuilder *select1 = xml->tag( "xf:select1" );
      select1->attribute( "ref", a.ref() );
      createLabel( select1, a );
      foreach( QString value, a.enumerationValues() ) {
        XmlBuilder *item = select1->tag( "xf:item" );
        QString itemLabel;
        Hint hint = mHints.hint( element.identifier() + '/' + a.ref() );
        if ( hint.isValid() ) itemLabel = hint.enumValue( value );
        if ( itemLabel.isEmpty() ) itemLabel = humanizeString( value );
        item->tag( "xf:label", itemLabel );
        item->tag( "xf:value", value );
      }
    } else {
      qDebug() << "Unsupported type: " << a.type();
    }
  }
}

void FormCreator::parseElement( const Schema::Element &element, XmlBuilder *xml )
{
  if ( element.type() == Schema::Node::String ) {
    XmlBuilder *textArea = xml->tag( "xf:textarea" );
    textArea->attribute( "ref", "." );
    createLabel( textArea, element );
  } else if ( element.type() == Schema::Node::NormalizedString ||
              element.type() == Schema::Node::Token ) {
    XmlBuilder *input = xml->tag( "xf:input" );
    input->attribute( "ref", "." );
    createLabel( input, element );
  } else if ( element.type() == Schema::Node::ComplexType ) {
    parseComplexType( element, xml, true, Reference() );
  } else {
    qDebug() << "Unsupported type: " << element.type();
  }
}

void FormCreator::parseComplexType( const Schema::Element &element, XmlBuilder *xml, bool topLevel, Reference path )
{
  QString currentChoice;

  XmlBuilder *section;

  bool choiceOnly = true;
  foreach( Schema::Relation r, element.elementRelations() ) {
    if( r.choice().isEmpty() )
      choiceOnly = false;
  }
  qDebug() << path.segments().size();
  qDebug() << path.toString();
  qDebug() << element.name();
  if( !topLevel && 
      !element.mixed() && 
      !choiceOnly) {
    section = xml->tag( "kxf:section" );
    section->attribute( "ref", path.toString() );
    path = path + Reference( element.name() );
    createLabel( section, element );
    if(  element.elementRelations().size() <= 1 ) {
      section->attribute( "visible", "false" );
      section->attribute( "overrideLabel", getLabel( element.ref(), element.name() ) );
    }
  } else {
    section = xml;
  }

  XmlBuilder *list = 0;
  XmlBuilder *choice = 0;

  if( element.mixed() ) {
    XmlBuilder *textInput = 0;
    textInput = section->tag( "xf:textarea" );
    textInput->attribute( "ref", "." );
    createLabel( textInput, element );
    mCollapsedForms.append( element.name() );
  }
  else {
    foreach( Schema::Relation r, element.elementRelations() ) {
      qDebug() << "  CHILD ELEMENT" << r.target();
      qDebug() << "    CHOICE" << r.choice();
      if ( r.isList() && r.choice().isEmpty()) {
        bool isMixedList = r.choice().contains( "+" );
        if ( !list || r.choice().isEmpty() || currentChoice != r.choice() ) {
          list = section->tag( "list" );
          Hint hint = mHints.hint( r.target() );
          if ( hint.isValid() ) {
            if( !hint.value( Hint::ListShowHeader ).isEmpty() )
              list->attribute( "showHeader", hint.value( Hint::ListShowHeader ) );

            QList<QDomElement> elements = hint.elements( Hint::ListVisibleElements );
            if( !elements.isEmpty() ) {
              XmlBuilder *items = list->tag( "visibleElements" );
              foreach( QDomElement e, elements ) {
                Reference r;
                r.fromString( e.text() );
                QString label = e.attribute( "label" );
                if( label.isEmpty() ) {
                  label = humanizeString( r.segments().last().name() );
                }
                XmlBuilder *item = items->tag( "visibleElement", label );
                item->attribute( "ref", r.toString() );
              }
            }
          }

          QString label;
          if ( isMixedList ) {
            label = "Item";
          } else {
            label = getLabel( element.identifier() + '[' + r.target() + ']' );
            if ( label.isEmpty() ) {
              label = humanizeString( r.target(), true );
            }
          }
          list->tag( "xf:label", label );
        }
        XmlBuilder *item = list->tag( "itemclass" );
        item->attribute( "ref", path.toString() + r.target() );
        item->tag( "itemlabel", createListItemLabel( element, r, isMixedList ) );
        currentChoice = r.choice();
      } else if( !r.choice().isEmpty() ) {
        if( !choice ) {
          if( r.isList() )
            choice = section->tag( "xf:select" );
          else
            choice = section->tag( "xf:select1" );
          choice->tag( "xf:label",  getLabel( element.ref(), element.name() ) );
          choice->attribute( "ref", (path + Reference( element.name() ) ).toString() );
        }
        Schema::Element choiceElement = mDocument.element( r );
        XmlBuilder *item = choice->tag( "xf:item" );
        QString value = choiceElement.name();
        QString itemLabel = getLabel( choiceElement.ref(), choiceElement.name() );
        item->tag( "xf:label", itemLabel );
        item->tag( "xf:value", value );
      } else{
        Schema::Element textElement = mDocument.element( r.target() );
        if( textElement.type() == Schema::Node::ComplexType && !textElement.mixed() ) {
          parseComplexType( textElement, section, false, path );
        } else {
          XmlBuilder *textInput = 0;
          if ( textElement.type() == Schema::Node::NormalizedString ) {
            textInput = section->tag( "xf:input" );
          } else {
            textInput = section->tag( "xf:textarea" );
          }
          textInput->attribute( "ref", (path + Reference( textElement.name() ) ).toString() );
          createLabel( textInput, textElement );
          mCollapsedForms.append( r.target() );
        }
      }
    }
  }
  if( element.elementRelations().size() == 0 ) {
    XmlBuilder *textInput = 0;
    textInput = section->tag( "xf:textarea" );
    textInput->attribute( "ref", (path + Reference( element.name() ) ).toString() );
    createLabel( textInput, element );
  }
}

QString FormCreator::humanizeString( const QString &str, bool pluralize )
{
  if ( str.isEmpty() ) return str;
  QString result = str[0].toUpper() + str.mid( 1 );
  if ( pluralize ) {
    if ( result.endsWith( "y" ) ) {
      result = result.left( str.length() - 1 ) + "ies";
    } else {
      result += 's';
    }
  }

  return result;
}

void FormCreator::setHints( const Hints &hints )
{
  mHints = hints;
}

void FormCreator::mergeHints( const Hints &hints )
{
  foreach( Hint h, hints.hints() ) {
    mHints.insertHint( h );
  }
}

void FormCreator::createLabel( XmlBuilder *parent, const Schema::Node &node )
{
  parent->tag( "xf:label", getLabel( node.identifier(), node.name() ) );
}

QString FormCreator::createListItemLabel( const Schema::Element &element, const Schema::Relation &r, bool isMixedList )
{
  QString itemLabel;
  itemLabel = getLabel( element.identifier() + '/' + r.target() );

  Schema::Element itemElement = mDocument.element( r );

  if ( itemLabel.isEmpty() ) {
    Hint hint = mHints.hint( r.target() );
    if ( hint.isValid() && !hint.value( Hint::ListItemLabelRef ).isEmpty() ) {
      itemLabel += QString("<arg ref=\"%1\"/>").arg( hint.value( Hint::ListItemLabelRef ) );
    }
  }

  if ( itemLabel.isEmpty() ) {
    // Try to guess a suitable item label.
    foreach( Schema::Relation r2, itemElement.attributeRelations() ) {
      if ( r2.target() == "name" ) {
        if ( isMixedList ) {
          itemLabel = humanizeString( itemElement.name() ) + ": ";
        }
        itemLabel += "<arg ref=\"@name\"/>";
       break;
      }
    }
  }

  if ( itemLabel.isEmpty() && itemElement.elementRelations().count() > 0 && !itemElement.mixed() ) {
    Schema::Element e2 = mDocument.element( itemElement.elementRelations().first() );
    QString ref;
    ref += QString("/%1").arg( e2.name() );
    while( e2.elementRelations().count() > 0 ) {
      ref += QString("/%1").arg( e2.elementRelations().first().target() );
      e2 = mDocument.element( e2.elementRelations().first().target() );
    }
    itemLabel += QString("<arg ref=\"%1\"/>").arg( ref );
  }
  if ( itemLabel.isEmpty() ) {
    if ( itemElement.type() == Schema::Node::String ) {
      itemLabel += "<arg ref=\".\" truncate=\"40\"/>";
    } else if ( itemElement.type() == Schema::Node::NormalizedString ||
         itemElement.type() == Schema::Node::Token ||
         (itemElement.type() == Schema::Node::ComplexType && itemElement.mixed() ) ) {
      itemLabel += "<arg ref=\".\"/>";
    }
  }

  if ( itemLabel.isEmpty() ) itemLabel = humanizeString( r.target() );
  return itemLabel;
}

QString FormCreator::getLabel( const QString &ref, const QString &fallback,
  bool pluralize )
{
//  qDebug() << "GETLABEL: " << ref;

  QString label;

  Hint hint = mHints.hint( ref );

  if ( hint.isValid() ) label = hint.value( Hint::Label );

  if ( label.isEmpty() ) label = humanizeString( fallback, pluralize );

  return label;
}

Hints FormCreator::hints() const
{
  return mHints;
}
