/*
    This file is part of KXForms.

    Copyright (c) 2007 Andre Duffeck <aduffeck@suse.de>

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

#include "kxformstest.h"

#include "../formcreator.h"
#include <kxml_compiler/parserxsd.h>

#include <QFile>
#include <QDebug>

void KXFormsTest::initTestCase()
{
  RNG::ParserXsd parser;
  QFile file("formcreatortest.xsd");

  if( !file.open( QIODevice::ReadOnly ) ) {
    QFAIL( "The test file 'formcreatortest.xsd' could not be loaded" );
    return;
  }

  Schema::Document doc = parser.parse( file.readAll() );

  KXForms::FormCreator creator;
  QString form = creator.create( doc );
  qDebug() << form;
  if( !mDoc.setContent( form ) ) {
    QFAIL( "The generated kxforms-document is malformed" );
    return;
  }

  QFile hintsFile("hintstest.ugh");

  if( !hintsFile.open( QIODevice::ReadOnly ) ) {
    QFAIL( "The test file 'hintstest.ugh' could not be loaded" );
    return;
  }

  if( !mHints.parseFile( hintsFile ) ) {
    QFAIL( "The content of the test file is malformed" );
    return;
  }
}

void KXFormsTest::testDocument()
{
  QCOMPARE( mDoc.childNodes().count(), 1 );

  QDomNode kxformsNode = mDoc.firstChild();

  QCOMPARE( kxformsNode.nodeName(), QString( "kxforms" ) );

  QCOMPARE( kxformsNode.childNodes().count(), 5 );

}

void KXFormsTest::testForm()
{
  QDomNode formNode = mDoc.firstChild().firstChild();
  QDomNode n;
  QDomNode listNode;

  // Test the form element
  QCOMPARE( formNode.nodeName(), QString( "form" ) );

  QCOMPARE( formNode.toElement().attribute( "ref" ), QString("person") );

  QCOMPARE( formNode.childNodes().count(), 4 );

  QDomNode child = formNode.firstChild();

  QCOMPARE( child.nodeName(), QString( "xf:label" ) );

  QCOMPARE( child.toElement().text(), QString( "Person" ) );


  // Test the list
  listNode = child.nextSibling();

  QCOMPARE( listNode.nodeName(), QString( "list" ) );

  QCOMPARE( listNode.childNodes().count(), 2 );

  n = listNode.firstChild();

  QCOMPARE( n.nodeName(), QString( "xf:label" ) );

  QCOMPARE( n.toElement().text(), QString( "FirstNames" ) );

  n = n.nextSibling();

  QCOMPARE( n.nodeName(), QString( "itemclass" ) );

  QCOMPARE( n.toElement().attribute( "ref" ), QString( "/firstName" ) );

  QCOMPARE( n.childNodes().count(), 1 );

  n = n.firstChild();

  QCOMPARE( n.nodeName(), QString( "itemlabel" ) );

  QCOMPARE( n.childNodes().count(), 1 );

  n = n.firstChild();

  QCOMPARE( n.nodeName(), QString( "arg" ) );

  QCOMPARE( n.toElement().attribute( "ref" ), QString( "." ) );

  QCOMPARE( n.toElement().attribute( "truncate" ), QString( "40" ) );

  // Test the Name section
  n = listNode.nextSibling();

  QCOMPARE( n.nodeName(), QString( "kxf:section" ) );

  QCOMPARE( n.toElement().attribute( "overrideLabel" ), QString( "Name" ) );

  QCOMPARE( n.toElement().attribute( "visible" ), QString( "false" ) );

  QCOMPARE( n.childNodes().count(), 2 );

  n = n.firstChild();

  QCOMPARE( n.nodeName(), QString( "xf:label" ) );

  QCOMPARE( n.toElement().text(), QString( "Name" ) );

  n = n.nextSibling();

  QCOMPARE( n.nodeName(), QString( "xf:textarea" ) );

  QCOMPARE( n.toElement().attribute( "ref" ), QString( "/name[1]/richtext[1]" ) );

  n = n.firstChild();

  QCOMPARE( n.nodeName(), QString( "xf:label" ) );

  QCOMPARE( n.toElement().text(), QString( "Richtext" ) );
}

void KXFormsTest::testHints()
{
  QCOMPARE( mHints.hints().count(), 3 );

  QCOMPARE( mHints.hint( "name" ).value( KXForms::Hint::Label ), QString( "Title" ) );

  QCOMPARE( mHints.hint( "documentationstatus" ).value( KXForms::Hint::Label ), QString( "Status of the documentation: " ) );

  KXForms::Hint hint = mHints.hint( "productcontext" );

  QCOMPARE( hint.value( KXForms::Hint::ListShowHeader ), QString( "true" ) );

  QCOMPARE( hint.value( KXForms::Hint::ListItemLabelRef ), QString( "/product/productid" ) );

  QCOMPARE( hint.elements( KXForms::Hint::ListVisibleElements ).size(), 2 );

  QDomElement e = hint.elements( KXForms::Hint::ListVisibleElements ).first().toElement();

  QCOMPARE( e.tagName(), QString( "listVisibleElement" ) );

  QCOMPARE( e.text(), QString( "product/productid" ) );

  QCOMPARE( e.attribute("label"), QString( "Id" ) );

  e = hint.elements( KXForms::Hint::ListVisibleElements ).at( 1 );

  QCOMPARE( e.tagName(), QString( "listVisibleElement" ) );

  QCOMPARE( e.text(), QString( "product/version" ) );

  QCOMPARE( e.attribute("label").isEmpty(), true );
}

QTEST_MAIN(KXFormsTest)

#include "kxformstest.moc"
