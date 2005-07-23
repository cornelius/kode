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

#include <kfiledialog.h>
#include <kio/netaccess.h>
#include <kmdcodec.h>
#include <kmessagebox.h>
#include <kmimemagic.h>
#include <klocale.h>
#include <kparts/part.h>
#include <kparts/componentfactory.h>
#include <ktempfile.h>

#include <qfile.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwidget.h>

#include <schema/simpletype.h>

#include "binaryinputfield.h"

BinaryInputField::BinaryInputField( const QString &name, const QString &typeName, const Schema::SimpleType *type )
  : SimpleInputField( name, type ),
    mValue( 0 ), mTypeName( typeName )
{
}

void BinaryInputField::setXMLData( const QDomElement &element )
{
  if ( mName != element.tagName() ) {
    qDebug( "BinaryInputField: Wrong dom element passed: expected %s, got %s", mName.latin1(), element.tagName().latin1() );
    return;
  }

  setData( element.text() );
}

void BinaryInputField::xmlData( QDomDocument &document, QDomElement &parent )
{
  QDomElement element = document.createElement( mName );
  element.setAttribute( "xsi:type", "xsd:" + mTypeName );
  QDomText text = document.createTextNode( data() );
  element.appendChild( text );

  parent.appendChild( element );
}

void BinaryInputField::setData( const QString &data )
{
  KCodecs::base64Decode( data.utf8(), mValue );
}

QString BinaryInputField::data() const
{
  QByteArray data = KCodecs::base64Encode( mValue, false );
  return QString::fromUtf8( data.data(), data.size() );
}

QWidget *BinaryInputField::createWidget( QWidget *parent )
{
  mInputWidget = new BinaryWidget( parent );

  if ( !mValue.isEmpty() )
    mInputWidget->setData( mValue );

  return mInputWidget;
}

void BinaryInputField::valueChanged( const QByteArray &value )
{
  mValue = value;

  emit modified();
}


BinaryWidget::BinaryWidget( QWidget *parent )
  : QWidget( parent, "BinaryWidget" ),
    mMainWidget( 0 )
{
  mLayout = new QGridLayout( this, 3, 2, 11, 6 );

  mLoadButton = new QPushButton( i18n( "Load..." ), this );
  mSaveButton = new QPushButton( i18n( "Save..." ), this );
  mSaveButton->setEnabled( false );

  mLayout->addWidget( mLoadButton, 0, 1 );
  mLayout->addWidget( mSaveButton, 1, 1 );

  connect( mLoadButton, SIGNAL( clicked() ), SLOT( load() ) );
  connect( mSaveButton, SIGNAL( clicked() ), SLOT( save() ) );
}

void BinaryWidget::setData( const QByteArray &data )
{
  KMimeMagic magic;
  QString mimetype;

  delete mMainWidget;

  KMimeMagicResult *result = magic.findBufferType( data );
  if ( result->isValid() )
    mimetype = result->mimeType();

  if ( !mimetype.isEmpty() ) {
    KParts::ReadOnlyPart *part = KParts::ComponentFactory::createPartInstanceFromQuery<KParts::ReadOnlyPart>( mimetype, QString::null, this, 0, this, 0 );
    if ( part ) {
      KTempFile file;
      file.file()->writeBlock( data );
      file.close();
      part->openURL( KURL( file.name() ) );
      mMainWidget = part->widget();
    } else {
      mMainWidget = new QLabel( i18n( "No part found for visualization of mimetype %1" ).arg( mimetype ), this );
    }

    mData = data;
    mSaveButton->setEnabled( true );
  } else {
    mMainWidget = new QLabel( i18n( "Got data of unknown mimetype" ), this );
  }

  mLayout->addMultiCellWidget( mMainWidget, 0, 2, 0, 0 );
  mMainWidget->show();
}

void BinaryWidget::load()
{
  KURL url = KFileDialog::getOpenURL( QString(), QString(), this );
  if ( url.isEmpty() )
    return;

  QString tempFile;
  if ( KIO::NetAccess::download( url, tempFile, this ) ) {
    QFile file( tempFile );
    if ( !file.open( IO_ReadOnly ) ) {
      KMessageBox::error( this, i18n( "Unable to open file %1" ).arg( url.url() ) );
      return;
    }

    QByteArray data = file.readAll();
    setData( data );

    file.close();
    KIO::NetAccess::removeTempFile( tempFile );

    emit valueChanged( data );
  } else
    KMessageBox::error( this, KIO::NetAccess::lastErrorString() );
}

void BinaryWidget::save()
{
  KURL url = KFileDialog::getSaveURL( QString(), QString(), this );
  if ( url.isEmpty() )
    return;

  KTempFile tempFile;
  tempFile.file()->writeBlock( mData );
  tempFile.close();

  if ( !KIO::NetAccess::upload( tempFile.name(), url, this ) )
    KMessageBox::error( this, KIO::NetAccess::lastErrorString() );
}

#include "binaryinputfield.moc"
