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
#ifndef KODE_AUTOMAKEFILE_H
#define KODE_AUTOMAKEFILE_H

#include <qvaluelist.h>
#include <qstring.h>
#include <qmap.h>
#include <qstringlist.h>

namespace KODE {

class AutoMakefile
{
  public:
    class Target
    {
      public:
        typedef QValueList<Target> List;

        Target() {}
        Target( const QString &type, const QString &name );
           
        QString type;
      
        QString name;
      
        QString sources;
        QString libadd;
        QString ldflags;
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
