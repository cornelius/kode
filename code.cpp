/*
    This file is part of kdepim.

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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "code.h"

#include <kdebug.h>

#include <qfile.h>
#include <qtextstream.h>

using namespace KODE;

Code::Code()
  : mIndent( 0 )
{
}

Code::Code( int indent )
  : mIndent( indent )
{
}

void Code::clear()
{
  mIndent = 0;
  mText = QString::null;
}

void Code::setIndent( int indent )
{
  mIndent = indent;
}

void Code::indent()
{
  mIndent += 2;
}

void Code::unindent()
{
  mIndent -= 2;
  if ( mIndent < 0 ) mIndent = 0;
}

void Code::addLine( const QString &line )
{
  mText += spaces( mIndent );
  mText += line;
  mText += '\n';
}

void Code::newLine()
{
  mText += '\n';
}

QString Code::spaces( int count )
{
  QString str;
  for( int i = 0; i < count; ++i ) {
    str += ' ';
  }
  return str;
}

void Code::addBlock( const QString &block )
{
  QStringList lines = QStringList::split( "\n", block, true );
  if ( !lines.isEmpty() && lines.last().isEmpty() ) {
    lines.pop_back();
  }
  QStringList::ConstIterator it;
  for( it = lines.begin(); it != lines.end(); ++it ) {
    mText += spaces( mIndent );
    mText += *it;
    mText += '\n';
  }
}

void Code::addBlock( const QString &block, int indent )
{
  int tmp = mIndent;
  mIndent = indent;
  addBlock( block );
  mIndent = tmp;
}

void Code::addBlock( const Code &c )
{
  addBlock( c.text() );
}

void Code::addWrappedText( const QString &txt )
{
  int maxWidth = 80 - mIndent;
  unsigned int pos = 0;
  while ( pos < txt.length() ) {
    QString line = txt.mid( pos, maxWidth );
    addLine( line );
    pos += maxWidth;
  }
}

Code &Code::operator+=( const QString &str )
{
  addLine( str );

  return *this;
}

Code &Code::operator+=( const char *str )
{
  addLine( QString::fromLocal8Bit( str ) );
  return *this;
}
