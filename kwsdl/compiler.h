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

#ifndef KWSDL_COMPILER_H
#define KWSDL_COMPILER_H

#include <qdom.h>
#include <qobject.h>

#include "parser.h"

namespace KWSDL {

class Compiler : public QObject
{
  Q_OBJECT

  public:
    Compiler();

    void setWSDLUrl( const QString &wsdlUrl );
    void setOutputDirectory( const QString &outputDirectory );
    void setNameSpace( const QString &nameSpace );

  public slots:
    void run();

  private slots:
    void download();
    void parse( const QDomElement& );
    void create();

  private:
    QString mWSDLUrl;
    QString mOutputDirectory;
    QString mNameSpace;    

    Parser mParser;
    QString mWSDLBaseUrl;
};

}

#endif
