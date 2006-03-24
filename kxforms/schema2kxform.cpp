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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "formcreator.h"

#include <kxml_compiler/schema.h>
#include <kxml_compiler/parserxsd.h>

#include <common/nsmanager.h>
#include <common/messagehandler.h>
#include <common/parsercontext.h>

#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>
#include <kurl.h>

#include <QDir>
#include <QByteArray>
#include <QDebug>

#include <iostream>

static const KCmdLineOptions options[] =
{
  { "+schema", I18N_NOOP("Schema of XML file"), 0 },
  KCmdLineLastOption
};

int main(int argc, char **argv)
{
  KApplication::disableAutoDcopRegistration();

  KAboutData about("schema2kxform", I18N_NOOP("Schema to KXForms Converter"),
    "0.1", I18N_NOOP("Schema to KXForms Converter"),
    KAboutData::License_GPL, "(C) 2006 Cornelius Schumacher", 0, 0,
    "schumacher@kde.org");
  about.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );

  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app( false );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() < 1 ) {
    std::cerr << "Too few arguments." << std::endl;
    return 1;
  }
  if ( args->count() > 1 ) {
    std::cerr << "Too many arguments." << std::endl;
    return 1;
  }

  QString schemaFilename = args->url( 0 ).path();

  QFile schemaFile( schemaFilename );
  if ( !schemaFile.open( QIODevice::ReadOnly ) ) {
    kError() << "Unable to open '" << schemaFilename << "'" << endl;
    return 1;
  }

  ParserXsd parser;
  Schema::Document schemaDocument = parser.parse( schemaFile );

  KXForms::FormCreator creator;
  
  QString form = creator.create( schemaDocument );

  std::cout << form.toLocal8Bit().data();
}
