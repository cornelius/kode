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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "parserrelaxng.h"
#include "parserxsd.h"
#include "parserxml.h"
#include "creator.h"
#include "schema.h"

#include <libkode/code.h>
#include <libkode/printer.h>
#include <libkode/typedef.h>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QDebug>

#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QMap>
#include <QList>
#include <QFileInfo>

#include <iostream>

using namespace KXML;

int main( int argc, char **argv )
{
  QCoreApplication app( argc, argv );
  QCoreApplication::setApplicationName("kxml_compiler");
  QCoreApplication::setOrganizationName("kode");

  QCommandLineParser cmdLine;
  cmdLine.setApplicationDescription("KDE xml compiler");
  cmdLine.addHelpOption();
  cmdLine.addVersionOption();

  QCommandLineOption dirOption(
              QStringList() << "d" << "directory",
              QCoreApplication::translate("main", "Directory to generate files in"),
              "directory",
              ".");
  cmdLine.addOption(dirOption);

  QCommandLineOption verboseOption(
              "verbose",
              QCoreApplication::translate("main", "Generate debug output"));
  cmdLine.addOption(verboseOption);

  QCommandLineOption schemaOption(
              "schema",
              QCoreApplication::translate("main", "Schema of XML file"));
  cmdLine.addOption(schemaOption);

  QCommandLineOption extParserOption(
              "external-parser",
              QCoreApplication::translate("main", "Generate parser in separate source file"));
  cmdLine.addOption(extParserOption);

  QCommandLineOption xsdOption(
              "xsd",
              QCoreApplication::translate("main", "Schema is XML Schema"));
  cmdLine.addOption(xsdOption);

  QCommandLineOption rngOption(
              "rng",
              QCoreApplication::translate("main", "Schema is RelaxNG"));
  cmdLine.addOption(rngOption);

  QCommandLineOption xmlOption(
              "xml",
              QCoreApplication::translate("main", "Schema is example XML"));
  cmdLine.addOption(xmlOption);

  QCommandLineOption useKdeOption(
              "use-kde",
              QCoreApplication::translate("main", "Use KDE classes"));
  cmdLine.addOption(useKdeOption);

  QCommandLineOption licenseOption(
              "license",
              QCoreApplication::translate("main", "License of generated files. Possible values: gpl, bsd, lgpl"),
              "license");
  cmdLine.addOption(licenseOption);

  QCommandLineOption namespaceOption(
              "namespace",
              QCoreApplication::translate("main", "Namespace for generated classes"),
              "namespace");
  cmdLine.addOption(namespaceOption);

  QCommandLineOption exportOption(
              "export",
              QCoreApplication::translate("main", "Export declaration for generated classes"),
              "export");
  cmdLine.addOption(exportOption);

  QCommandLineOption createCRUDFunctionsOption(
              "create-crud-functions",
              QCoreApplication::translate("main", "Create functions for dealing with data suitable for CRUD model"));
  cmdLine.addOption(createCRUDFunctionsOption);

  QCommandLineOption outputFileName(
              QStringLiteral( "output-filename" ),
              QCoreApplication::translate( "main", "Set the filename of the output files.\n"
                                                  "If this options is set the basename of the generated files will be the <output-filename> \n"
                                                  "instead of the basename of the source XML/XSD/RNG file.)" ),
              QStringLiteral( "output-filename" ),
              QString());
  cmdLine.addOption( outputFileName );

  if (!cmdLine.parse(QCoreApplication::arguments())) {
    qDebug() << cmdLine.errorText();
    return -1;
  }

  if (cmdLine.positionalArguments().count() < 1) {
    qDebug() << "No filename argument passed";
    return -1;
  }

  QString baseDir = cmdLine.value(dirOption);
  if ( !baseDir.endsWith( QDir::separator()) ) baseDir.append( QDir::separator() );

  QString schemaFilename = cmdLine.positionalArguments().at(0);
  QFileInfo fi(schemaFilename);
  QString baseName;
  if ( cmdLine.isSet( outputFileName ) ) {
    baseName = cmdLine.value( outputFileName );
  } else {
    baseName = fi.baseName();
    baseName.remove( "_" );
  }

  QFile schemaFile( schemaFilename );
  if ( !schemaFile.open( QIODevice::ReadOnly ) ) {
    qDebug() <<"Unable to open '" << schemaFilename <<"'";
    return 1;
  }

  if ( cmdLine.isSet(verboseOption) ) {
    qDebug() <<"Begin parsing";
  }

  Schema::Document schemaDocument;
  bool verbose = cmdLine.isSet(verboseOption);
  fi.setFile( schemaFile );
  if ( cmdLine.isSet( xsdOption ) || fi.suffix() == "xsd" ) {
    RNG::ParserXsd p;
    p.setVerbose( cmdLine.isSet(verboseOption) );

    schemaDocument = p.parse( schemaFile );

    if ( schemaDocument.isEmpty() ) {
      qDebug() <<"Error parsing schema '" << schemaFilename <<"'";
      return 1;
    }
  } else if ( cmdLine.isSet( rngOption ) || fi.suffix() == "rng" ) {
    QString errorMsg;
    int errorLine, errorCol;
    QDomDocument doc;
    if ( !doc.setContent( &schemaFile, false, &errorMsg, &errorLine, &errorCol ) ) {
      qDebug() << errorMsg <<" at" << errorLine <<"," << errorCol;
      return 1;
    }

    RNG::ParserRelaxng p;
    p.setVerbose( verbose );
    RNG::Element *start = p.parse( doc.documentElement() );
    if ( !start ) {
      qDebug() <<"Could not find start element";
      return 1;
    }

    if ( verbose ) {
      p.dumpDefinitionMap();
    }

  //  return 0;

    p.substituteReferences( start );

  #if 1
    if ( verbose ) {
      std::cout << "--- TREE:" << std::endl;
      p.dumpTree( start );
    }
  #endif

    schemaDocument = p.convertToSchema( start );
  } else if ( cmdLine.isSet( "xml" ) || fi.suffix() == "xml" ) {
    ParserXml schemaParser;
    schemaParser.setVerbose( verbose );
    schemaDocument = schemaParser.parse( schemaFile );
  } else {
    qDebug() <<"Unable to determine schema type.";
    return 1;
  }

  if ( verbose ) {
    std::cout << "--- SCHEMA:" << std::endl;
    schemaDocument.dump();

    qDebug() <<"Begin creating code";
  }

  Creator::XmlParserType pt;
  if ( cmdLine.isSet( "external-parser" ) ) {
    pt = Creator::XmlParserDomExternal;
  } else {
    pt = Creator::XmlParserDom;
  }

  Creator c( schemaDocument, pt );
  c.setVerbose( verbose );
  c.setUseKde( cmdLine.isSet( "use-kde" ) );
  c.setCreateCrudFunctions( cmdLine.isSet( "create-crud-functions" ) );
  if ( cmdLine.isSet( "namespace" ) ) {
    c.file().setNameSpace( cmdLine.value( "namespace" ) );
  }
  if ( cmdLine.isSet( "export" ) ) {
    c.setExportDeclaration( cmdLine.value( "export" ) );
  }

  if ( cmdLine.isSet( "license" ) ) {
    QString l = cmdLine.value( "license" );
    if ( l == "gpl" ) {
      c.setLicense( KODE::License( KODE::License::GPL ) );
    } else if ( l == "bsd" ) {
      c.setLicense( KODE::License( KODE::License::BSD ) );
    } else if ( l == "lgpl" ) {
      c.setLicense( KODE::License( KODE::License::LGPL ) );
    }
  }

  if ( verbose ) {
    qDebug() <<"Create classes";
  }
  foreach( Schema::Element e, schemaDocument.usedElements() ) {
    if ( !e.text() ) {
      c.createClass( e );
    }
  }
  if ( verbose ) {
    qDebug() <<"Create parser";
  }
  c.create();

  if ( verbose ) {
    qDebug() <<"Begin printing code";
  }
  c.setFilename( baseName );

  KODE::Printer printer;
  printer.setCreationWarning( true );
  printer.setGenerator( QCoreApplication::applicationName() );
  printer.setOutputDirectory( baseDir );
  printer.setSourceFile( cmdLine.positionalArguments().at(0) );

  c.printFiles( printer );

  if ( verbose ) {
    qDebug() <<"Finished.";
  }
}
