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
#include <QComboBox>

#include <klocale.h>
#include <klineedit.h>

using namespace KXForms;

GlobalSettingsDialog::GlobalSettingsDialog( Manager *manager, QWidget *parent )
: KDialog( parent ), mManager( manager )
{
  setCaption( i18n("Global Settings") );

  setButtons( KDialog::Ok | KDialog::Cancel );
  setDefaultButton( KDialog::Ok );
  showButtonSeparator( true );

  QWidget *page = new QWidget(this);
  setMainWidget(page);

  QGridLayout *topLayout = new QGridLayout( page );

  QLabel *appearanceLabel = new QLabel( i18n("Appearance"), page );
  topLayout->addWidget( appearanceLabel, 0, 0 );

  QStringList appearanceValues;
  appearanceValues << "minimal" << "compact" << "full";
  mAppearanceBox = new QComboBox( page );
  mAppearanceBox->addItems( appearanceValues );
  topLayout->addWidget( mAppearanceBox, 0, 1 );


  QLabel *typeLabel = new QLabel( i18n("Type for input elements"), page );
  topLayout->addWidget( typeLabel, 1, 0 );

  mTypeLineEdit = new KLineEdit( page );
  topLayout->addWidget( mTypeLineEdit, 1, 1 );


  QLabel *constraintLabel = new QLabel( i18n("Constraint for input elements"), page );
  topLayout->addWidget( constraintLabel, 2, 0 );

  mConstraintLineEdit = new KLineEdit( page );
  topLayout->addWidget( mConstraintLineEdit, 2, 1 );

  load();
}

void GlobalSettingsDialog::load()
{
  GuiElement::Properties p( *mManager->defaultProperties() );

  mAppearanceBox->setCurrentIndex( p.appearance );
  mTypeLineEdit->setText( p.type );
  mConstraintLineEdit->setText( p.constraint );
}

void GlobalSettingsDialog::save()
{

}
