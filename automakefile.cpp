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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "automakefile.h"

using namespace KODE;

AutoMakefile::Target::Target( const QString &type, const QString &name )
  : type( type ), name( name )
{
}

AutoMakefile::AutoMakefile()
{
}

void AutoMakefile::addTarget( const Target &t )
{
  mTargets.append( t );
  if ( mTargetTypes.find( t.type ) == mTargetTypes.end() ) {
    mTargetTypes.append( t.type );
  }
}

void AutoMakefile::addEntry( const QString &variable, const QString &value )
{
  if ( variable.isEmpty() ) {
    mEntries.append( variable );
    return;
  }
  
  QStringList::ConstIterator it = mEntries.find( variable );
  if ( it == mEntries.end() ) {
    mEntries.append( variable );
    QMap<QString,QString>::Iterator it = mValues.find( variable );
    if ( it == mValues.end() ) {
      mValues.insert( variable, value );
    } else {
      mValues[ variable ].append( " " + value );
    }
  }
}

void AutoMakefile::newLine()
{
  addEntry( "" );
}

QString AutoMakefile::text() const
{
  QString out;

  QStringList::ConstIterator it;
  for( it = mEntries.begin(); it != mEntries.end(); ++it ) {
    QString variable = *it;
    if ( variable.isEmpty() ) {
      out += '\n';
    } else {
      out += variable + " = " + mValues[ variable ] + '\n';
    }
  }
  out += '\n';
  
  for( it = mTargetTypes.begin(); it != mTargetTypes.end(); ++it ) {
    QString targetType = *it;
    
    out += targetType + " = ";

    Target::List::ConstIterator it2;
    for( it2 = mTargets.begin(); it2 != mTargets.end(); ++it2 ) {
      Target t = *it2;
      if ( t.type != targetType ) continue;
      
      out += " " + t.name;
    }
    out += "\n\n";
  
    for( it2 = mTargets.begin(); it2 != mTargets.end(); ++it2 ) {
      Target t = *it2;
      if ( t.type != targetType ) continue;

      QString name = t.name;
      name.replace( '.', '_' );
      
      out += name + "_SOURCES = " + t.sources + '\n';
      out += name + "_LIBADD = " + t.libadd + '\n';
      out += name + "_LDFLAGS = " + t.ldflags + '\n';
    }
    out += '\n';
  
  }

  return out;
}
