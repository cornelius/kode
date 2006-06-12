/*
    This file is part of KXForms.

    Copyright (c) 2005,2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "mainwindow.h"

#include "reference.h"

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>

using namespace KXForms;

static const char description[] =
    I18N_NOOP("References Test Program");

static KCmdLineOptions options[] =
{
  { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
  { "kxform <URL>", I18N_NOOP( "Form description" ), 0 },
  { "schema <URL>", I18N_NOOP( "XML Schema" ), 0 },
  KCmdLineLastOption
};

void checkReference( const QString &str )
{
  Reference ref( str );

  QString s = ref.toString();

  if ( s == str ) {
    kDebug() << "OK  REF: " << str << endl;
  } else {
    kDebug() << "ERR REF: " << str << " GOT: " << s << endl;
  }
}

int main(int argc, char **argv)
{
  // KApplication::disableAutoDcopRegistration();

  KAboutData about("testreferences", I18N_NOOP("TestReferences"), "0.1",
    description,
    KAboutData::License_GPL, "(C) 2005 Cornelius Schumacher", 0, 0,
    "schumacher@kde.org");
  about.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );
  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);

  KApplication app( false );

  checkReference( "/one/two/three" );
  checkReference( "relative" );
  checkReference( "/one/two[2]" );
  checkReference( "/one/two[2]/three[2]" );
  checkReference( "/one/@attribute" );
  checkReference( "relative/more/more" );

  Reference r1 = "one";
  kDebug() << "R1: " << r1.toString() << endl;
  Reference r2 = "two";
  Reference r3 = r1 + r2;
  kDebug() << "R3: " << r3.toString() << endl;
  
  Reference r4 = "one/twox";
  
  if ( r3 == r4 ) kDebug() << "YES" << endl;
  else kDebug() << "NO" << endl;
}

