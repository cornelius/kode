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

#include <QtCore/QMap>
#include <QtCore/QStringList>

#include "automakefile.h"

using namespace KODE;

class AutoMakefile::Target::Private
{
  public:
    QString mType;
    QString mName;

    QString mSources;
    QString mLibAdd;
    QString mLdAdd;
    QString mLdFlags;
};

AutoMakefile::Target::Target()
  : d( new Private )
{
}

AutoMakefile::Target::Target( const AutoMakefile::Target &other )
  : d( new Private )
{
  *d = *other.d;
}

AutoMakefile::Target::Target( const QString &type, const QString &name )
  : d( new Private )
{
  d->mType = type;
  d->mName = name;
}

AutoMakefile::Target::~Target()
{
  delete d;
}

AutoMakefile::Target& AutoMakefile::Target::operator=( const AutoMakefile::Target &other )
{
  if ( this == &other )
    return *this;

  *d = *other.d;

  return *this;
}

void AutoMakefile::Target::setType( const QString &type )
{
  d->mType = type;
}

QString AutoMakefile::Target::type() const
{
  return d->mType;
}

void AutoMakefile::Target::setName( const QString &name )
{
  d->mName = name;
}

QString AutoMakefile::Target::name() const
{
  return d->mName;
}

void AutoMakefile::Target::setSources( const QString &sources )
{
  d->mSources = sources;
}

QString AutoMakefile::Target::sources() const
{
  return d->mSources;
}

void AutoMakefile::Target::setLibAdd( const QString &libAdd )
{
  d->mLibAdd = libAdd;
}

QString AutoMakefile::Target::libAdd() const
{
  return d->mLibAdd;
}

void AutoMakefile::Target::setLdAdd( const QString &ldAdd )
{
  d->mLdAdd = ldAdd;
}

QString AutoMakefile::Target::ldAdd() const
{
  return d->mLdAdd;
}

void AutoMakefile::Target::setLdFlags( const QString &ldFlags )
{
  d->mLdFlags = ldFlags;
}

QString AutoMakefile::Target::ldFlags() const
{
  return d->mLdFlags;
}


class AutoMakefile::Private
{
  public:
    Target::List mTargets;
    QStringList mTargetTypes;

    QStringList mEntries;
    QMap<QString,QString> mValues;

};

AutoMakefile::AutoMakefile()
  : d( new Private )
{
}

AutoMakefile::AutoMakefile( const AutoMakefile &other )
  : d( new Private )
{
  *d = *other.d;
}

AutoMakefile::~AutoMakefile()
{
  delete d;
}

AutoMakefile& AutoMakefile::operator=( const AutoMakefile &other )
{
  if ( this == &other )
    return *this;

  *d = *other.d;

  return *this;
}

void AutoMakefile::addTarget( const Target &target )
{
  d->mTargets.append( target );

  if ( !d->mTargetTypes.contains( target.type() ) )
    d->mTargetTypes.append( target.type() );
}

AutoMakefile::Target::List AutoMakefile::targets() const
{
  return d->mTargets;
}

void AutoMakefile::addEntry( const QString &variable, const QString &value )
{
  if ( variable.isEmpty() ) {
    d->mEntries.append( variable );
    return;
  }

  if ( !d->mEntries.contains( variable ) ) {
    d->mEntries.append( variable );
    QMap<QString,QString>::Iterator it = d->mValues.find( variable );
    if ( it == d->mValues.end() ) {
      d->mValues.insert( variable, value );
    } else {
      d->mValues[ variable ].append( ' ' + value );
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
  for ( it = d->mEntries.constBegin(); it != d->mEntries.constEnd(); ++it ) {
    QString variable = *it;
    if ( variable.isEmpty() ) {
      out += '\n';
    } else {
      out += variable + " = " + d->mValues[ variable ] + '\n';
    }
  }
  out += '\n';

  for ( it = d->mTargetTypes.constBegin(); it != d->mTargetTypes.constEnd(); ++it ) {
    QString targetType = *it;

    out += targetType + " = ";

    Target::List::ConstIterator it2;
    for ( it2 = d->mTargets.constBegin(); it2 != d->mTargets.constEnd(); ++it2 ) {
      Target target = *it2;
      if ( target.type() != targetType ) continue;

      out += ' ' + target.name();
    }
    out += "\n\n";

    for ( it2 = d->mTargets.constBegin(); it2 != d->mTargets.constEnd(); ++it2 ) {
      Target target = *it2;
      if ( target.type() != targetType ) continue;

      QString name = target.name();
      name.replace( '.', '_' );

      out += name + "_SOURCES = " + target.sources() + '\n';
      if ( !target.libAdd().isEmpty() )
        out += name + "_LIBADD = " + target.libAdd() + '\n';
      else
        out += name + "_LDADD = " + target.ldAdd() + '\n';
      out += name + "_LDFLAGS = " + target.ldFlags() + '\n';
    }
    out += '\n';

  }

  return out;
}
