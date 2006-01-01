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

#include "converter.h"

using namespace KWSDL;

void Converter::createKDETransport()
{
  KODE::Class transport( "Transport" );
  transport.addBaseClass( mQObject );
  transport.addHeaderInclude( "qobject.h" );
  transport.addHeaderInclude( "kio/job.h" );

  transport.addInclude( "kdebug.h" );

  KODE::MemberVariable url( "url", "QString" );
  transport.addMemberVariable( url );

  KODE::MemberVariable slotDataVar( "data", "QByteArray" );
  transport.addMemberVariable( slotDataVar );

  // ctor
  KODE::Function ctor( "Transport" );
  ctor.addArgument( "const QString &url" );
  ctor.setBody( url.name() + " = url;" );

  transport.addFunction( ctor );

  // query
  KODE::Function query( "query", "void" );
  query.addArgument( "const QString &xml" );
  query.addArgument( "const QString &header" );

  KODE::Code queryCode;
  queryCode += slotDataVar.name() + ".truncate( 0 );";
  queryCode.newLine();
  queryCode += "QByteArray postData;";
  queryCode += "QDataStream stream( postData, IO_WriteOnly );";
  queryCode += "stream.writeRawBytes( xml.utf8(), xml.utf8().length() );";
  queryCode.newLine();
  queryCode += "KIO::TransferJob* job = KIO::http_post( KURL( " + url.name() + " ), postData, false );";
  queryCode += "if ( !job ) {";
  queryCode.indent();
  queryCode += "kdWarning() << \"Unable to create KIO job for \" << " + url.name() + " << endl;";
  queryCode += "return;";
  queryCode.unindent();
  queryCode += "}";
  queryCode.newLine();
  queryCode += "job->addMetaData( \"UserAgent\", \"KWSDL\" );";
  queryCode += "job->addMetaData( \"content-type\", \"Content-Type: text/xml; charset=utf-8\" );";
  queryCode += "if ( !header.isEmpty() ) {";
  queryCode.indent();
  queryCode += "job->addMetaData( \"customHTTPHeader\", header );";
  queryCode.unindent();
  queryCode += "}";
  queryCode.newLine();
  queryCode += "connect( job, SIGNAL( data( KIO::Job*, const QByteArray& ) ), this, SLOT( slotData( KIO::Job*, const QByteArray& ) ) );";
  queryCode += "connect( job, SIGNAL( result( KIO::Job* ) ), this, SLOT( slotResult( KIO::Job* ) ) );";

  query.setBody( queryCode );

  transport.addFunction( query );

  // signal
  KODE::Function result( "result", "void", KODE::Function::Signal );
  result.addArgument( "const QString &xml" );

  KODE::Function error( "error", "void", KODE::Function::Signal );
  error.addArgument( "const QString &msg" );

  transport.addFunction( result );
  transport.addFunction( error );

  // data slot
  KODE::Function slotData( "slotData", "void", KODE::Function::Private | KODE::Function::Slot );

  slotData.addArgument( "KIO::Job*" );
  slotData.addArgument( "const QByteArray &data" );

  KODE::Code slotDataCode;
  slotDataCode += "unsigned int oldSize = " + slotDataVar.name() + ".size();";
  slotDataCode += slotDataVar.name() + ".resize( oldSize + data.size() );";
  slotDataCode += "memcpy( " + slotDataVar.name() + ".data() + oldSize, data.data(), data.size() );";

  slotData.setBody( slotDataCode );

  transport.addFunction( slotData );

  // result slot
  KODE::Function slotResult( "slotResult", "void", KODE::Function::Private | KODE::Function::Slot );
  slotResult.addArgument( "KIO::Job* job" );

  KODE::Code slotResultCode;
  slotResultCode += "if ( job->error() != 0 ) {";
  slotResultCode.indent();
  slotResultCode += "emit error( job->errorText() );";
  slotResultCode += "return;";
  slotResultCode.unindent();
  slotResultCode += "}";
  slotResultCode.newLine();

  slotResultCode += "emit result( QString::fromUtf8( " + slotDataVar.name() + ".data(), " + slotDataVar.name() + ".size() ) );";

  slotResult.setBody( slotResultCode );

  transport.addFunction( slotResult );

  mClasses.append( transport );
}

void Converter::createQtTransport()
{
}
