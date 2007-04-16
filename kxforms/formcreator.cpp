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

  Hint hint = mHints.hint( Reference( element.name() ) );
  if ( hint.isValid() ) {
    QList<QDomElement> elements = hint.elements( Hint::Pages );
    if( !elements.isEmpty() ) {
      XmlBuilder *items = form->tag( "pages" );
      foreach( QDomElement e, elements ) {
        XmlBuilder *page = items->tag( "page", e.text() );
        page->attribute( "id", e.attribute( "id") );
      }
    }
  }

  parseAttributes( element, form );

  parseElement( element, form );
}

void FormCreator::parseAttributes( const Schema::Element &element, XmlBuilder *xml, Reference path  )
{
  if( element.attributeRelations().size() == 0 )
    return;

  XmlBuilder *attributes = xml->tag( "attributes" );
  foreach( Schema::Relation r, element.attributeRelations() ) {
    Schema::Attribute a = mDocument.attribute( r );

    qDebug() << "  ATTRIBUTE: " << a.identifier();

    if ( a.type() == Schema::Attribute::String ) {
      XmlBuilder *input = attributes->tag( "xf:input" );
      input->attribute( "ref", (path + Reference( a.ref() )).toString() );
      createLabel( input, a );
    } else if ( a.type() == Schema::Attribute::Enumeration ) {
      XmlBuilder *select1 = attributes->tag( "xf:select1" );
      select1->attribute( "ref", (path + Reference( a.ref() )).toString() );
      createLabel( select1, a );
      foreach( QString value, a.enumerationValues() ) {
        XmlBuilder *item = select1->tag( "xf:item" );
        QString itemLabel;
        Hint hint = mHints.hint( Reference( element.identifier() + '/' + a.ref() ) );
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
    applyCommonHints( textArea, element.name() );
  } else if ( element.type() == Schema::Node::NormalizedString ||
              element.type() == Schema::Node::Token ) {
    XmlBuilder *input = xml->tag( "xf:input" );
    input->attribute( "ref", "." );
    createLabel( input, element );
    applyCommonHints( input, element.name() );
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
    applyCommonHints( section, element.name() );
    if(  element.elementRelations().size() <= 1 ) {
      section->attribute( "visible", "false" );
      section->attribute( "overrideLabel", getLabel( Reference( element.ref() ), element.name() ) );
    }
  } else {
    section = xml;
  }

  XmlBuilder *list = 0;
  QStringList listTitles;
  bool listHeaderSuppressed = false;
  XmlBuilder *choice = 0;

  if( element.text() ) {
    XmlBuilder *textInput = 0;
    textInput = section->tag( "xf:textarea" );
    textInput->attribute( "ref", (path + Reference( element.name() ) ).toString() );
    createLabel( textInput, element );
    applyCommonHints( textInput, element.name() );
    mCollapsedForms.append( element.name() );
  } else if( element.baseType() != Schema::Node::None ) {
    XmlBuilder *textInput = 0;
    if ( element.baseType() == Schema::Node::NormalizedString ) {
      textInput = section->tag( "xf:input" );
    } else {
      textInput = section->tag( "xf:textarea" );
    }
    textInput->attribute( "ref", (path + Reference( element.name() ) ).toString() );
    createLabel( textInput, element );
    applyCommonHints( textInput, element.name() );
    parseAttributes( element, textInput, path + Reference( element.name() ) );
    mCollapsedForms.append( element.name() );
  }
  else {
    foreach( Schema::Relation r, element.elementRelations() ) {
      qDebug() << "  CHILD ELEMENT" << r.target();
      qDebug() << "    CHOICE" << r.choice();
      if ( r.isList()) {
        bool isMixedList = r.choice().contains( "+" );
        if ( !list || r.choice().isEmpty() || currentChoice != r.choice() ) {
          list = section->tag( "list" );
          applyCommonHints( list, r.target() );
          Hint hint = mHints.hint( path + Reference( element.name() ) );
          if ( hint.isValid() ) {
            if( !hint.value( Hint::ListShowHeader ).isEmpty() ) {
              list->attribute( "showHeader", hint.value( Hint::ListShowHeader ) );
              listHeaderSuppressed = (hint.value( Hint::ListShowHeader ) == "false");
            }
            if( !hint.value( Hint::ListShowSearch ).isEmpty() ) {
              list->attribute( "showSearch", hint.value( Hint::ListShowSearch ) );
            }
          }

          QString label;
          if ( isMixedList ) {
            label = "Item";
          } else {
            label = getLabel( Reference( element.identifier() + '[' + r.target() + ']' ) );
            if ( label.isEmpty() ) {
              label = humanizeString( r.target(), true );
            }
          }
          list->tag( "xf:label", label );
        }
        QString listItemType;
        if( isMixedList )
          listItemType = humanizeString( r.target(), false );

        XmlBuilder *item = list->tag( "itemclass" );
        item->attribute( "ref", path.toString() + r.target() );
        qDebug() << path.toString() + r.target();
        Schema::Element listElement = mDocument.element( r.target() );
            qDebug() << listElement.name();

        Hint hint = mHints.hint( Reference( r.target() ) );
        if( hint.isValid() && !hint.value( Hint::ListItemList ).isEmpty() )
          item->attribute( "list", hint.value( Hint::ListItemList ) );

        Reference listPath = path;
        if( listElement.elementRelations().size() == 1 && 
            listElement.attributeRelations().size() == 0 && 
            mDocument.element( listElement.elementRelations().first() ).type() == Schema::Node::ComplexType ) {
          listElement = mDocument.element( listElement.elementRelations().first() );
          listPath = listPath + Reference( listElement.identifier() );
        }

            qDebug() << listElement.name();
        int subElementCnt = 0;
        foreach( Schema::Relation r2, listElement.elementRelations() ) {
          Schema::Element subElement = mDocument.element( r2.target() );
          if( subElement.type() != Schema::Node::ComplexType )
            subElementCnt++;
        }
        subElementCnt += listElement.attributeRelations().size();
        if( subElementCnt <= 3 &&
            subElementCnt > 1 ) {
          if( !listHeaderSuppressed == true )
            list->attribute( "showHeader", "true" );
          QStringList titles;
          foreach( Schema::Relation r2, listElement.elementRelations() ) {
            Schema::Element subElement = mDocument.element( r2.target() );
            qDebug() << subElement.name();
            if( subElement.type() != Schema::Node::ComplexType ||
                subElement.mixed() ) {
              item->tag( "itemlabel", createListItemLabel( r2, listPath, listItemType ) );
              listItemType = QString::null;
              titles.append( createListHeader( listPath + Reference( r2.target() ) ) );
            }
          }
          foreach( Schema::Relation r2, listElement.attributeRelations() ) {
            item->tag( "itemlabel", createListItemLabel( r2, listPath, listItemType, true ) );
            listItemType = QString::null;
            titles.append( createListHeader( listPath + Reference( "@" + r2.target() ) ) );
          }
          if( titles.size() > listTitles.size() )
            listTitles = titles;
        } else {
          Schema::Element subElement = mDocument.element( r.target() );
          if( subElement.elementRelations().size() == 0 &&
              subElement.attributeRelations().size() == 0 )
            item->tag( "itemlabel", createListItemLabel( Schema::Relation("."), path, listItemType ) );
          else
            item->tag( "itemlabel", createListItemLabel( r, path, listItemType ) );
          listItemType = QString::null;
        }
        currentChoice = r.choice();
      } else if( !r.choice().isEmpty() ) {
        if( !choice ) {
          if( r.isList() )
            choice = section->tag( "xf:select" );
          else
            choice = section->tag( "xf:select1" );
          applyCommonHints( choice, element.ref() );
          choice->tag( "xf:label",  getLabel( Reference( element.ref() ), element.name() ) );
          choice->attribute( "ref", (path + Reference( element.name() ) ).toString() );
        }
        Schema::Element choiceElement = mDocument.element( r );
        XmlBuilder *item = choice->tag( "xf:item" );
        QString value = choiceElement.name();
        QString itemLabel = getLabel( Reference( choiceElement.ref() ), choiceElement.name() );
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
          applyCommonHints( textInput, r.target() );
          textInput->attribute( "ref", (path + Reference( textElement.name() ) ).toString() );
          createLabel( textInput, textElement );
          mCollapsedForms.append( r.target() );
        }
      }
    }
    if( list && listTitles.size() > 0 ) {
      XmlBuilder *header = list->tag( "headers" );
      foreach( QString s, listTitles ) {
        header->tag( "header", s );
      }
    }
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
  parent->tag( "xf:label", getLabel( Reference( node.identifier() ), node.name() ) );
}

QString FormCreator::createListHeader( const Reference &r )
{
  QString header;
  Hint hint = mHints.hint( r );
  if ( hint.isValid() && !hint.value( Hint::ListHeader ).isEmpty() ) {
    header = hint.value( Hint::ListHeader );
  }

  if( header.isEmpty() )
    header = getLabel( r, humanizeString( r.lastSegment().name() ) );

  return header;
}

QString FormCreator::createListItemLabel( const Schema::Relation &r, const Reference &path, const QString &itemType, bool attribute )
{
  QString itemLabel;
  Schema::Element itemElement = mDocument.element( r );

  Reference itemRef;
  if( attribute )
    itemRef = Reference( "@" + r.target() );
  else
    itemRef = Reference( r.target() );

  QString typeString;
  if( !itemType.isEmpty() )
    typeString = QString( "%1: " ).arg( itemType );


  Hint hint = mHints.hint( path + Reference( r.target() ) );
  if ( hint.isValid() && !hint.value( Hint::ListItemLabel ).isEmpty() ) {
    itemLabel = QString( hint.value( Hint::ListItemLabel ) );
    if( itemLabel.contains( "%1" ) ) {
      if ( itemElement.type() == Schema::Node::String ||
           (itemElement.type() == Schema::Node::ComplexType && itemElement.mixed() )) {
        itemLabel.replace( "%1", QString( "<itemLabelArg ref=\"%1\" truncate=\"40\"/>" ).arg( (path + itemRef).toString() ) );
      } else if ( itemElement.type() == Schema::Node::NormalizedString ||
           itemElement.type() == Schema::Node::Token ) {
        itemLabel.replace( "%1", QString( "<itemLabelArg ref=\"%1\"/>" ).arg( (path + itemRef).toString() ) );
      }
    }
  }

  if ( itemLabel.isEmpty() ) {
    // Try to guess a suitable item label.
    foreach( Schema::Relation r2, itemElement.attributeRelations() ) {
      if ( r2.target() == "name" ) {
        itemLabel += typeString;
        itemLabel += "<itemLabelArg ref=\"@name\"/>";
       break;
      }
    }
  }

  if ( itemLabel.isEmpty() ) {
    if ( itemElement.type() == Schema::Node::String ||
         (itemElement.type() == Schema::Node::ComplexType && itemElement.mixed() )) {
      itemLabel += typeString;
      itemLabel += QString( "<itemLabelArg ref=\"%1\" truncate=\"40\"/>" ).arg( (path + itemRef).toString() );
    } else if ( itemElement.type() == Schema::Node::NormalizedString ||
         itemElement.type() == Schema::Node::Token ) {
      itemLabel += typeString;
      itemLabel += QString( "<itemLabelArg ref=\"%1\"/>" ).arg( (path + itemRef).toString() );
    }
  }
  return itemLabel;
}

QString FormCreator::getLabel( const Reference &ref, const QString &fallback,
  bool pluralize )
{
//  qDebug() << "GETLABEL: " << ref;

  QString label;

  Hint hint = mHints.hint( ref );

  if ( hint.isValid() ) label = hint.value( Hint::Label );

  if ( label.isEmpty() ) label = humanizeString( fallback, pluralize );

  return label;
}

void FormCreator::applyCommonHints( XmlBuilder *xml, const Reference &ref )
{
  Hint hint = mHints.hint( ref );
  if( !hint.isValid() )
    return;

  if( hint.hasValue( Hint::PageReference ) ) {
    XmlBuilder *layout = xml->tag( "properties" )->tag( "layout" );
    layout->tag( "pageRef", hint.value( Hint::PageReference ) );
  }

  if( hint.hasValue( Hint::Appearance ) ) {
    XmlBuilder *layout = xml->tag( "properties" )->tag( "layout" );
    layout->tag( "appearance", hint.value( Hint::Appearance ) );
  }

  if( hint.hasValue( Hint::Position ) ) {
    XmlBuilder *layout = xml->tag( "properties" )->tag( "layout" );
    layout->tag( "position", hint.value( Hint::Position ) );
  }
}

Hints FormCreator::hints() const
{
  return mHints;
}
