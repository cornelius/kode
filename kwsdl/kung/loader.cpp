/*
    This file is part of Kung.

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

#include <qfile.h>

#include <schema/fileprovider.h>

#include "dispatcher.h"

#include "loader.h"

Loader::Loader()
  : QObject( 0, "KWSDL::Loader" )
{
}

void Loader::setWSDLUrl( const QString &wsdlUrl )
{
  mWSDLUrl = wsdlUrl;
  mWSDLBaseUrl = mWSDLUrl.left( mWSDLUrl.findRev( '/' ) );

  mParser.setSchemaBaseUrl( mWSDLBaseUrl );
}

void Loader::run()
{
  download();
}

void Loader::download()
{
  Schema::FileProvider provider;

  QString fileName;
  if ( provider.get( mWSDLUrl, fileName ) ) {
    QFile file( fileName );
    if ( !file.open( IO_ReadOnly ) ) {
      qDebug( "Unable to download wsdl file %s", mWSDLUrl.latin1() );
      provider.cleanUp();
      return;
    }

    QString errorMsg;
    int errorLine, errorCol;
    QDomDocument doc;
    if ( !doc.setContent( &file, true, &errorMsg, &errorLine, &errorCol ) ) {
      qDebug( "%s at (%d,%d)", errorMsg.latin1(), errorLine, errorCol );
      return;
    }

    parse( doc.documentElement() );

    provider.cleanUp();
  }
}

void Loader::parse( const QDomElement &element )
{
  mParser.parse( element );
  execute();
}

void Loader::execute()
{
  mDispatcher = new Dispatcher;
  mDispatcher->setWSDL( mParser.wsdl() );

  mDispatcher->run();
}

#include "loader.moc"
