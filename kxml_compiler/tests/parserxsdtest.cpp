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

#include "parserxsdtest.h"

#include "../parserxsd.h"

#include <QFile>
#include <QDebug>

void ParserXsdTest::initTestCase()
{
  RNG::ParserXsd parser;
  QFile file("simple.xsd");

  if( !file.open( QIODevice::ReadOnly ) ) {
    QFAIL( "The test file 'simple.xsd' could not be loaded" );
    return;
  }

  mDoc = parser.parse( file.readAll() );
}

void ParserXsdTest::testElementParsing()
{
  QCOMPARE( mDoc.elements().size(), 7 );

  QCOMPARE( mDoc.usedElements().size(), 6 );

  QCOMPARE( mDoc.startElement().name(), QString("person") );
}

void ParserXsdTest::testAttributeParsing()
{
  QCOMPARE( mDoc.attributes().size(), 2);

  QCOMPARE( mDoc.attributes().first().name(), QString("id") );
}

void ParserXsdTest::testTypeParsing()
{
  QCOMPARE( mDoc.startElement().type(), Schema::Node::ComplexType );

  QCOMPARE( mDoc.element( "firstname" ).type(), Schema::Node::String );

  QCOMPARE( mDoc.element( "age" ).type(), Schema::Node::ComplexType );

  QCOMPARE( mDoc.element( "active" ).type(), Schema::Node::Boolean );
}

void ParserXsdTest::testRelationParsing()
{
  QCOMPARE( mDoc.startElement().elementRelations().size(), 5 );

  QCOMPARE( mDoc.startElement().attributeRelations().size(), 2 );

//   foreach( Schema::Relation e, doc.startElement().attributeRelations() ) {
//     if( e.target() == QString("id") )
//       QCOMPARE( e.isRequired(), true );
//     else if( e.target() == QString("cc") )
//       QCOMPARE( e.isRequired(), false );
//   }
}

QTEST_MAIN(ParserXsdTest)

#include "parserxsdtest.moc"
