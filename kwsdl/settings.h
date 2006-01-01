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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <qmap.h>
#include <qstring.h>

class Settings
{
  public:
    typedef QMap<QString, QString> NSMapping;

    ~Settings();

    static Settings* self();

    bool load( const QString &fileName );

    void setWsdlUrl( const QString &wsdlUrl );
    QString wsdlUrl() const;
    QString wsdlBaseUrl() const;

    void setOutputFileName( const QString &outputFileName );
    QString outputFileName() const;

    void setOutputDirectory( const QString &outputDirectory );
    QString outputDirectory() const;

    void setNamespaceMapping( const NSMapping &namespaceMapping );
    NSMapping namespaceMapping() const;

  private:
    Settings();

    static Settings *mSelf;

    QString mWsdlUrl;
    QString mOutputFileName;
    QString mOutputDirectory;
    NSMapping mNamespaceMapping;
};

#endif
