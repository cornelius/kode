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

#include "style.h"

#include <kdebug.h>

#include <qfile.h>
#include <qtextstream.h>

using namespace KODE;

Style::Style()
{
}

Style::~Style()
{
}

QString Style::className( const QString &str )
{
  return upperFirst( str );
}

QString Style::upperFirst( const QString &str )
{
  if ( str.isEmpty() ) return str;
  return str[ 0 ].upper() + str.mid( 1 );
}

QString Style::lowerFirst( const QString &str )
{
  if ( str.isEmpty() ) return str;
  return str[ 0 ].lower() + str.mid( 1 );
}
