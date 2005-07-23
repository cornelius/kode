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

#include <qlayout.h>
#include <qlistbox.h>
#include <qpushbutton.h>

#include <klocale.h>

#include <schema/simpletype.h>

#include "inputdialog.h"
#include "inputfieldfactory.h"
#include "outputdialog.h"

#include "listinputfield.h"

ListInputField::ListInputField( const QString &name, const QString &typeName, const Schema::SimpleType *type )
  : SimpleInputField( name, type ), mTypeName( typeName )
{
}

void ListInputField::setXMLData( const QDomElement &element )
{
  InputField::List::Iterator it;
  for ( it = mFields.begin(); it != mFields.end(); ++it ) {
    delete *it;
  }
  mFields.clear();

  QDomNode node = element.firstChild();
  while ( !node.isNull() ) {
    QDomElement child = node.toElement();
    if ( !child.isNull() ) {
      InputField *field = InputFieldFactory::self()->createField( name(), mTypeName );
      if ( field ) {
        field->setXMLData( child );
        appendChild( field );
      } else
        qDebug( "ListInputField: Unable to create field of type %s", mTypeName.latin1() );
    }

    node = node.nextSibling();
  }
}

void ListInputField::xmlData( QDomDocument &document, QDomElement &parent )
{
  InputField::List::Iterator it;
  for ( it = mFields.begin(); it != mFields.end(); ++it ) {
    (*it)->xmlData( document, parent );
  }
}

void ListInputField::setData( const QString& )
{
}

QString ListInputField::data() const
{
  return QString();
}

QWidget *ListInputField::createWidget( QWidget *parent )
{
  mInputWidget = new ListWidget( this, name(), mTypeName, parent );

  return mInputWidget;
}


ListWidget::ListWidget( InputField *parentField, const QString &name, const QString &type, QWidget *parent )
  : QWidget( parent ),
    mParentField( parentField ), mName( name ), mType( type )
{
  QGridLayout *layout = new QGridLayout( this, 4, 2, 11, 6 );

  mView = new QListBox( this );
  layout->addMultiCellWidget( mView, 0, 3, 0, 0 );

  mAddButton = new QPushButton( i18n( "Add" ), this );
  layout->addWidget( mAddButton, 0, 1 );

  mEditButton = new QPushButton( i18n( "Edit..." ), this );
  layout->addWidget( mEditButton, 1, 1 );

  mRemoveButton = new QPushButton( i18n( "Remove" ), this );
  layout->addWidget( mRemoveButton, 2, 1 );

  connect( mAddButton, SIGNAL( clicked() ), SLOT( add() ) );
  connect( mEditButton, SIGNAL( clicked() ), SLOT( edit() ) );
  connect( mRemoveButton, SIGNAL( clicked() ), SLOT( remove() ) );

  update();
}

void ListWidget::update()
{
  int pos = mView->currentItem();
  mView->clear();

  const InputField::List fields = mParentField->childFields();
  InputField::List::ConstIterator it;
  for ( it = fields.begin(); it != fields.end(); ++it )
    mView->insertItem( (*it)->name() );

  mView->setCurrentItem( pos );

  updateButtons();
}

void ListWidget::add()
{
  InputField *field = InputFieldFactory::self()->createField( mName, mType );
  if ( !field ) {
    qDebug( "ListInputField: Unable to create field of type %s", mType.latin1() );
    return;
  }

  InputDialog dlg( field->createWidget( this ), this );
  if ( dlg.exec() ) {
    mParentField->appendChild( field );

    update();
  } else
    delete field;
}

void ListWidget::edit()
{
  int pos = mView->currentItem();

  if ( pos == -1 )
    return;

  InputField *field = mParentField->childFields()[ pos ];
  if ( !field )
    return;

  OutputDialog dlg( field->createWidget( this ), this );
  dlg.exec();

  update();
}

void ListWidget::remove()
{
  int pos = mView->currentItem();

  if ( pos == -1 )
    return;

  InputField *field = mParentField->childFields()[ pos ];
  if ( !field )
    return;

  mParentField->removeChild( field );
  delete field;

  update();
}

void ListWidget::updateButtons()
{
  bool enabled = (mParentField->childFields().count() > 0);

  mEditButton->setEnabled( enabled );
  mRemoveButton->setEnabled( enabled );
}

#include "listinputfield.moc"
