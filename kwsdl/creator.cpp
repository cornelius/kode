/*
    This file is part of KDE.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#include <kode/file.h>
#include <kode/printer.h>

#include "creator.h"

using namespace KWSDL;

Creator::Creator()
  : mOutputDirectory( "output/" )
{
}

void Creator::create( const KODE::Class::List &list )
{
  KODE::Printer printer;
  printer.setOutputDirectory( mOutputDirectory );

  QStringList cppFiles;

  KODE::Class::List::ConstIterator it;
  for ( it = list.begin(); it != list.end(); ++it ) {
    KODE::File file;

    if ( !mNameSpace.isEmpty() )
      file.setNameSpace( mNameSpace );

    file.setFilename( (*it).name().lower() );
    file.addCopyright( 2005, "Tobias Koenig", "tokoe@kde.org" );
    file.setLicense( KODE::License( KODE::License::GPL ) );

    file.insertClass( *it );

    printer.printHeader( file );
    printer.printImplementation( file );

    cppFiles.append( file.filename() + ".cpp" );
  }

  KODE::AutoMakefile::Target libTarget( "bin_PROGRAMS", "kwsdl" );
  libTarget.setSources( "main.cc " + cppFiles.join( " " ) );
  libTarget.setLdFlags( "$(all_libraries) $(KDE_RPATH)" );
  libTarget.setLdAdd( "-lkdecore -lkio" );

  KODE::AutoMakefile makefile;
  makefile.addTarget( libTarget );
  makefile.addEntry( "INCLUDES", "-I$(top_srcdir) -I$(top_srcdir)/libkdepim $(all_includes)" );
  makefile.addEntry( "METASOURCES", "AUTO" );

  printer.printAutoMakefile( makefile );
}
