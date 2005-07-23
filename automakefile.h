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
#ifndef KODE_AUTOMAKEFILE_H
#define KODE_AUTOMAKEFILE_H

#include <qvaluelist.h>
#include <qstring.h>
#include <qmap.h>
#include <qstringlist.h>

#include <kdepimmacros.h>

namespace KODE {

class KDE_EXPORT AutoMakefile
{
  public:
    class KDE_EXPORT Target
    {
      public:
        typedef QValueList<Target> List;

        Target() {}
        Target( const QString &type, const QString &name );

        void setType( const QString &type ) { mType = type; }
        QString type() const { return mType; }

        void setName( const QString &name ) { mName = name; }
        QString name() const { return mName; }

        void setSources( const QString &sources ) { mSources = sources; }
        QString sources() const { return mSources; }

        void setLibAdd( const QString &libAdd ) { mLibAdd = libAdd; }
        QString libAdd() const { return mLibAdd; }

        void setLdAdd( const QString &ldAdd ) { mLdAdd = ldAdd; }
        QString ldAdd() const { return mLdAdd; }

        void setLdFlags( const QString &ldFlags ) { mLdFlags = ldFlags; }
        QString ldFlags() const { return mLdFlags; }

      private:
        QString mType;
        QString mName;

        QString mSources;
        QString mLibAdd;
        QString mLdAdd;
        QString mLdFlags;
    };
  
    AutoMakefile();
  
    void addTarget( const Target &t );
    Target::List targets() const { return mTargets; }

    void addEntry( const QString &variable,
                   const QString &value = QString::null );

    void newLine();

    QString text() const;

  private:
    Target::List mTargets;
    QStringList mTargetTypes;

    QStringList mEntries;
    QMap<QString,QString> mValues;
};

}

#endif
