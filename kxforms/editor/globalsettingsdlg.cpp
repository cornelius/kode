/*
    This file is part of KXForms.

    Copyright (c) 2007 Andre Duffeck <aduffeck@suse.de>

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

#include "globalsettingsdlg.h"
#include "../manager.h"
#include "../guielement.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QTreeWidget>
#include <QPushButton>
#include <QSpinBox>

#include <kdebug.h>
#include <klocale.h>
#include <klineedit.h>
#include <ktabwidget.h>

using namespace KXForms;

class GroupDialog :  public KDialog
{
  public:
    GroupDialog( const QString &caption, const QString &id, const QString &title, QWidget *parent );

    QString id() { return mId->text(); }
    QString title() { return mTitle->text(); }

  private:
    KLineEdit *mId;
    KLineEdit *mTitle;
};

GroupDialog::GroupDialog( const QString &caption, const QString &id, const QString &title, QWidget *parent )
: KDialog( parent )
{
  setCaption( caption );
  setButtons( KDialog::Ok | KDialog::Cancel );
  setDefaultButton( KDialog::Ok );
  showButtonSeparator( true );

  QWidget *page = new QWidget(this);
  setMainWidget(page);

  QGridLayout *topLayout = new QGridLayout( page );

  QLabel *idLabel = new QLabel( i18n("Id"), this );
  topLayout->addWidget( idLabel, 0, 0 );
  mId = new KLineEdit( this );
  topLayout->addWidget( mId, 0, 1 );
  QLabel *titleLabel = new QLabel( i18n("Title"), this );
  topLayout->addWidget( titleLabel, 1, 0 );
  mTitle = new KLineEdit( this );
  topLayout->addWidget( mTitle, 1, 1 );

  mId->setText( id );
  mTitle->setText( title );
  mId->setFocus();
  mId->setSelection( 0, id.length() );
}




GlobalSettingsDialog::GlobalSettingsDialog( Manager *manager, QWidget *parent )
: KDialog( parent ), mManager( manager )
{
  setCaption( i18n("Global Settings") );

  setButtons( KDialog::Ok | KDialog::Cancel );
  setDefaultButton( KDialog::Ok );
  showButtonSeparator( true );

  KTabWidget *tabWidget = new KTabWidget(this);
  setMainWidget(tabWidget);

  QWidget *page = new QWidget( tabWidget );
  tabWidget->addTab( page, i18n("Global options") );

  QGridLayout *topLayout = new QGridLayout( page );

  QLabel *appearanceLabel = new QLabel( i18n("Appearance"), page );
  topLayout->addWidget( appearanceLabel, 0, 0 );

  QStringList appearanceValues;
  appearanceValues << "minimal" << "compact" << "full";
  mAppearanceBox = new QComboBox( page );
  mAppearanceBox->addItems( appearanceValues );
  topLayout->addWidget( mAppearanceBox, 0, 1 );


  QLabel *styleLabel = new QLabel( i18n("Layout Style"), page );
  topLayout->addWidget( styleLabel, 1, 0 );

  QStringList styleValues;
  styleValues << "horizontal" << "vertical";
  mStyleBox = new QComboBox( page );
  mStyleBox->addItems( styleValues );
  topLayout->addWidget( mStyleBox, 1, 1 );


  QLabel *readOnlyLabel = new QLabel( i18n("Read only"), page );
  topLayout->addWidget( readOnlyLabel, 2, 0 );

  mReadOnlyCheckBox = new QCheckBox( page );
  topLayout->addWidget( mReadOnlyCheckBox, 2, 1 );


  QLabel *thresholdLabel = new QLabel( i18n("Size Threshold"), page );
  topLayout->addWidget( thresholdLabel, 3, 0, Qt::AlignTop  );

  mSizeThresholdSpin = new QSpinBox( this );
  mSizeThresholdSpin->setRange( 0, 500 );
  topLayout->addWidget( mSizeThresholdSpin, 3, 1, Qt::AlignTop  );




  QWidget *formPage = new QWidget( tabWidget );
  tabWidget->addTab( formPage, i18n("%1 options", mManager->currentGui()->ref().lastSegment().toString()) );

  QGridLayout *topFormLayout = new QGridLayout( formPage );

  QLabel *groupLabel = new QLabel( i18n("Groups"), formPage );
  topFormLayout->addWidget( groupLabel, 1, 0, 2, 1, Qt::AlignTop );

  mGroupWidget = new QTreeWidget( formPage );
  connect( mGroupWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), SLOT(slotEditGroup()));
  QStringList headerStrings;
  headerStrings << i18n("Id") << i18n("Title");
  QTreeWidgetItem *header = new QTreeWidgetItem( headerStrings );
  mGroupWidget->setHeaderItem( header );
  topFormLayout->addWidget( mGroupWidget, 1, 1 );

  QHBoxLayout *buttonLayout = new QHBoxLayout( formPage );
  mAddGroupButton = new QPushButton( i18n("Add"), this );
  connect( mAddGroupButton, SIGNAL(clicked()), SLOT(slotAddGroup()));
  buttonLayout->addWidget( mAddGroupButton );
  mEditGroupButton = new QPushButton( i18n("Edit"), this );
  connect( mEditGroupButton, SIGNAL(clicked()), SLOT(slotEditGroup()));
  buttonLayout->addWidget( mEditGroupButton );
  mDeleteGroupButton = new QPushButton( i18n("Remove"), this );
  connect( mDeleteGroupButton, SIGNAL(clicked()), SLOT(slotDeleteGroup()));
  buttonLayout->addWidget( mDeleteGroupButton );
  topFormLayout->addLayout( buttonLayout, 2, 1 );


  load();
}

void GlobalSettingsDialog::load()
{
  GuiElement::Properties p( *mManager->defaultProperties() );
  mGroups = mManager->currentGui()->groups();

  mAppearanceBox->setCurrentIndex( p.appearance );
  mReadOnlyCheckBox->setCheckState( p.readonly ? Qt::Checked : Qt::Unchecked );
  mStyleBox->setCurrentIndex( p.layoutStyle );
  mSizeThresholdSpin->setValue( mManager->currentGui()->sizeThreshold() );

  mGroupWidget->clear();
  QList<QTreeWidgetItem *> items;
  foreach( QString id, mGroups.keys() ) {
    QStringList data;
    data << id << mGroups[ id ];
    items.append( new QTreeWidgetItem( data ) );
  }
  mGroupWidget->insertTopLevelItems(0, items);
}

void GlobalSettingsDialog::accept()
{
  kDebug() ;
  mHints = Hints();
  Hint globalHint;
  globalHint.setRef( "*" );
  globalHint.setValue( Hint::Appearance, mAppearanceBox->currentText() );
  globalHint.setValue( Hint::LayoutStyle, mStyleBox->currentText() );
  globalHint.setValue( Hint::ReadOnly, mReadOnlyCheckBox->checkState() == Qt::Checked ? "true" : "false" );
  globalHint.setValue( Hint::FormSizeThreshold, QString::number( mSizeThresholdSpin->value() ) );
  mHints.insertHint( globalHint );

  Hint formHint;
  QDomDocument doc;
  formHint.setRef( Reference( mManager->currentGui()->ref().lastSegment().toString() ) );
  for( int i = 0; i < mGroupWidget->topLevelItemCount(); ++i ) {
    QTreeWidgetItem *item = mGroupWidget->topLevelItem( i );
    kDebug() << item->text( 0 ) << item->text( 1 );
    QDomElement e = doc.createElement( "group" );
    QDomText t = doc.createTextNode( item->text( 1 ) );
    e.setAttribute( "id", item->text( 0 ) );
    e.appendChild( t );
    formHint.addElement( Hint::Groups, e );
  }
  mHints.insertHint( formHint );


  KDialog::accept();
}

void GlobalSettingsDialog::slotAddGroup()
{
  GroupDialog *dlg = new GroupDialog( i18n("Add Group"), QString(), QString(), this );
  if( dlg->exec() == QDialog::Accepted ) {
    QStringList data;
    data << dlg->id() << dlg->title();
    mGroupWidget->insertTopLevelItem( mGroupWidget->topLevelItemCount(), new QTreeWidgetItem( data ));
  }
  dlg->deleteLater();
}

void GlobalSettingsDialog::slotEditGroup()
{
  QTreeWidgetItem *item = mGroupWidget->currentItem();
  if( !item )
    return;

  GroupDialog *dlg = new GroupDialog( i18n("Edit Group"), item->text(0), item->text(1), this );
  if( dlg->exec() == QDialog::Accepted ) {
    QStringList data;
    data << dlg->id() << dlg->title();
    mGroupWidget->insertTopLevelItem( mGroupWidget->indexOfTopLevelItem( item ), new QTreeWidgetItem( data ));
    mGroupWidget->takeTopLevelItem( mGroupWidget->indexOfTopLevelItem( item ) );
  }
  dlg->deleteLater();
}

void GlobalSettingsDialog::slotDeleteGroup()
{
  if( mGroupWidget->currentItem() )
    mGroupWidget->takeTopLevelItem( mGroupWidget->indexOfTopLevelItem( mGroupWidget->currentItem() ) );
}

#include "globalsettingsdlg.moc"
