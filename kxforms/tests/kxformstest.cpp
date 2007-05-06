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
#include <kdebug.h>

void KXFormsTest::initTestCase()
{
  RNG::ParserXsd parser;
  QFile file("simplefeature.xsd");

  if( !file.open( QIODevice::ReadOnly ) ) {
    QFAIL( "The test file 'simplefeature.xsd' could not be loaded" );
    return;
  }

  Schema::Document doc = parser.parse( file.readAll() );


  QFile hintsFile("simplefeature.ugh");

  if( !hintsFile.open( QIODevice::ReadOnly ) ) {
    QFAIL( "The test file 'simplefeature.ugh' could not be loaded" );
    return;
  }

  if( !mHints.parseFile( hintsFile ) ) {
    QFAIL( "The content of the test file is malformed" );
    return;
  }


  KXForms::FormCreator creator;
  creator.mergeHints( mHints );
  QString form = creator.create( doc );
  kDebug() << form << endl;
  if( !mDoc.setContent( form ) ) {
    QFAIL( "The generated kxforms-document is malformed" );
    return;
  }
}

void KXFormsTest::testDocument()
{
  QCOMPARE( mDoc.childNodes().count(), 1 );

  QDomNode kxformsNode = mDoc.firstChild();

  QCOMPARE( kxformsNode.nodeName(), QString( "kxforms" ) );

  QCOMPARE( kxformsNode.childNodes().count(), 15 );

}

void KXFormsTest::testForm()
{
  QDomNode formNode = mDoc.firstChild().firstChild();
  QDomNode n;
  QDomNode child;

  // Test the form element
  QCOMPARE( formNode.nodeName(), QString( "form" ) );

  QCOMPARE( formNode.toElement().attribute( "ref" ), QString("feature") );

  QCOMPARE( formNode.childNodes().count(), 8 );

  n = formNode.firstChild();

  QCOMPARE( n.nodeName(), QString( "xf:label" ) );

  QCOMPARE( n.toElement().text(), QString( "Feature" ) );

  // Test the list
  n = n.nextSibling();

  QCOMPARE( n.childNodes().count(), 3 );

  child = n.firstChild().nextSibling();

  QCOMPARE( child.toElement().attribute( "id" ), QString( "other" ) );

  QCOMPARE( child.toElement().nodeName(), QString( "page" ) );

  QCOMPARE( child.toElement().text(), QString( "Other" ) );


  // Test the list
  n = n.nextSibling();

  QCOMPARE( n.nodeName(), QString( "list" ) );

  QCOMPARE( n.childNodes().count(), 2 );

  child = n.firstChild();

  QCOMPARE( child.nodeName(), QString( "xf:label" ) );

  QCOMPARE( child.toElement().text(), QString( "Categories" ) );

  child = child.nextSibling();

  QCOMPARE( child.nodeName(), QString( "itemclass" ) );

  QCOMPARE( child.toElement().attribute( "ref" ), QString( "/category" ) );

  QCOMPARE( child.childNodes().count(), 1 );

  child = child.firstChild();

  QCOMPARE( child.nodeName(), QString( "itemlabel" ) );

  QCOMPARE( child.childNodes().count(), 1 );

  child = child.firstChild();

  QCOMPARE( child.nodeName(), QString( "itemLabelArg" ) );

  QCOMPARE( child.toElement().attribute( "ref" ), QString( "/.[1]" ) );

  QCOMPARE( child.toElement().attribute( "truncate" ), QString( "40" ) );


  // Test the TextArea section
  n = n.nextSibling();

  QCOMPARE( n.nodeName(), QString( "xf:textarea" ) );

  QCOMPARE( n.toElement().attribute( "ref" ), QString( "/title[1]" ) );

  QCOMPARE( n.childNodes().count(), 1 );

  child = n.firstChild();

  QCOMPARE( child.nodeName(), QString( "xf:label" ) );

  QCOMPARE( child.toElement().text(), QString( "Title" ) );


  // Test the Description section
  n = n.nextSibling();

  QCOMPARE( n.nodeName(), QString( "kxf:section" ) );

  QCOMPARE( n.toElement().attribute( "overrideLabel" ), QString( "Description" ) );

  QCOMPARE( n.toElement().attribute( "ref" ), QString( "/" ) );

  QCOMPARE( n.toElement().attribute( "visible" ), QString( "false" ) );

  QCOMPARE( n.childNodes().count(), 2 );

  child = n.firstChild();

  QCOMPARE( child.nodeName(), QString( "xf:label" ) );

  QCOMPARE( child.toElement().text(), QString( "Description" ) );

  child = child.nextSibling();

  QCOMPARE( child.nodeName(), QString( "xf:textarea" ) );

  QCOMPARE( child.toElement().attribute( "ref" ), QString( "/description[1]/richtext[1]" ) );

  child = child.firstChild();

  QCOMPARE( child.nodeName(), QString( "xf:label" ) );

  QCOMPARE( child.toElement().text(), QString( "Richtext" ) );


  // Test the Partnercontext section
  n = n.nextSibling();

  QCOMPARE( n.nodeName(), QString( "kxf:section" ) );

  QCOMPARE( n.toElement().attribute( "ref" ), QString( "/" ) );

  QCOMPARE( n.childNodes().count(), 4 );

  child = n.firstChild().nextSibling();

  QCOMPARE( child.nodeName(), QString( "properties" ) );

  QCOMPARE( child.childNodes().count(), 1 );

  child = child.firstChild();

  QCOMPARE( child.nodeName(), QString( "layout" ) );

  QCOMPARE( child.childNodes().count(), 1 );

  child = child.firstChild();

  QCOMPARE( child.nodeName(), QString( "pageRef" ) );

  QCOMPARE( child.toElement().text(), QString( "other" ) );
}

void KXFormsTest::testHints()
{
  QCOMPARE( mHints.hints().count(), 9 );

  QCOMPARE( mHints.hint( "version" ).value( KXForms::Hint::ListHeader ), QString( "Productversion" ) );

  QCOMPARE( mHints.hint( "version" ).value( KXForms::Hint::ListItemLabel ), QString( "<itemLabelArg ref=\"product/name\" /> (v%1)" ) );


  QList<QDomElement> pages = mHints.hint( "feature" ).elements( KXForms::Hint::Groups );

  QCOMPARE( pages.size(), 3 );

  QCOMPARE( pages[1].nodeName(), QString( "page" ) );

  QCOMPARE( pages[1].attribute( "id" ), QString( "other" ) );


  QCOMPARE( mHints.hint( "documentationstatus" ).value( KXForms::Hint::GroupReference ), QString( "documentation" ) );

  QCOMPARE( mHints.hint( "documentationstatus" ).value( KXForms::Hint::Appearance ), QString( "full" ) );

  QCOMPARE( mHints.hint( "documentationstatus" ).value( KXForms::Hint::Label ), QString( "Status of the documentation: " ) );


  QCOMPARE( mHints.hint( "organization" ).value( KXForms::Hint::Position ), QString( "1" ) );


  QCOMPARE( mHints.hint( "productcontext" ).value( KXForms::Hint::ListShowHeader ), QString( "true" ) );

}

QTEST_MAIN(KXFormsTest)

#include "kxformstest.moc"
