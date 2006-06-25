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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "code.h"

#include <kdebug.h>

#include <QFile>
#include <QTextStream>

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
  mText.clear();
}

bool Code::isEmpty() const
{
  return mText.isEmpty();
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

void Code::addLine( const char c )
{
  mText += spaces( mIndent );
  mText += c;
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
  QStringList lines = block.split( "\n" );
  if ( !lines.isEmpty() && lines.last().isEmpty() ) {
    lines.pop_back();
  }
  QStringList::ConstIterator it;
  for( it = lines.begin(); it != lines.end(); ++it ) {
    if ( !(*it).isEmpty() ) mText += spaces( mIndent );
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
  int pos = 0;
  while ( pos < txt.length() ) {
    QString line = txt.mid( pos, maxWidth );
    addLine( line );
    pos += maxWidth;
  }
}

void Code::addFormattedText( const QString &text )
{
  int maxWidth = 80 - mIndent;
  int lineLength = 0;

  QString line;
  const QStringList words = text.split( ' ', QString::SkipEmptyParts );

  QStringList::ConstIterator it;
  for ( it = words.begin(); it != words.end(); ++it ) {
    if ( (int)(*it).length() + lineLength >= maxWidth ) {
      line = line.trimmed();
      addLine( line );
      line.truncate( 0 );
      lineLength = 0;
    }

    int pos = (*it).indexOf( "\n" );
    if ( pos != -1 ) {
      line += (*it).left( pos );
      line = line.trimmed();
      addLine( line );

      line = (*it).mid( pos + 1 ) + ' ';
      lineLength = (*it).length() - pos;
    } else {
      line += *it + ' ';
      lineLength += (*it).length() + 1;
    }
  }

  line = line.trimmed();
  addLine( line );
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

Code &Code::operator+=( const char c )
{
  addLine( c );
  return *this;
}

Code &Code::operator+=( const Code &code )
{
  mText += code.mText;
  return *this;
}
