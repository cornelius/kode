/*
    This file is part of KDE.

    Copyright (c) 2005 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/

#include "guihandler.h"

#include "manager.h"

#include <kdebug.h>
#include <kdialogbase.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <qlayout.h>
#include <qlabel.h>

using namespace KXForms;

FormDialog::FormDialog( QWidget *parent, const QString &title, Manager *m )
  : KDialogBase( parent, 0, true, title,
    KDialogBase::Ok, KDialogBase::Ok, false ), mFormGui( 0 ), mManager( m )
{
  QFrame *topFrame = makeMainWidget();
  mTopLayout = new QVBoxLayout( topFrame );
}

void FormDialog::setGui( FormGui *gui )
{
  mTopLayout->addWidget( gui );
  mFormGui = gui;
}

void FormDialog::slotOk()
{
  kdDebug() << "FormDialog::slotOk()" << endl;

  mFormGui->saveData();
  
  mManager->destroyGui( mFormGui );
  
  accept();
}


GuiHandler::GuiHandler( Manager *m )
  : mManager( m )
{
}

FormGui *GuiHandler::createRootGui( QWidget *parent )
{
  kdDebug() << "GuiHandler::createRootGui()" << endl;

  Form *f = mManager->rootForm();

  if ( !f ) {
    KMessageBox::sorry( parent, i18n("Root form not found.") );
    return 0;
  }

  FormGui *gui = createGui( f, parent );

  gui->setRef( "/" + f->ref() );
  gui->parseElement( f->element() );

  if ( mManager->hasData() ) {
    kdDebug() << "Manager::createGui() Load data on creation" << endl;
    mManager->loadData( gui );
  }

  return gui;
}

FormGui *GuiHandler::createGui( const Reference &ref, QWidget *parent )
{
  kdDebug() << "GuiHandler::createGui() ref: '" << ref.toString() << "'" << endl;

  if ( ref.isEmpty() ) {
    KMessageBox::sorry( parent, i18n("No reference.") );
    return 0;
  }

  QString r = ref.segments().last().name();

  Form *f = mManager->form( r );

  if ( !f ) {
    KMessageBox::sorry( parent, i18n("Form '%1' not found.").arg( ref.toString() ) );
    return 0;
  }

  FormDialog dlg( parent, i18n("Edit %1").arg( ref.toString() ), mManager );

  FormGui *gui = createGui( f, dlg.mainWidget() );
  gui->setRef( ref );
  gui->parseElement( f->element() );

  if ( !gui ) {
    KMessageBox::sorry( parent, i18n("Unable to create GUI for '%1'.")
      .arg( ref.toString() ) );
    return 0;
  }

  dlg.setGui( gui );

  if ( mManager->hasData() ) {
    kdDebug() << "Manager::createGui() Load data on creation" << endl;
    mManager->loadData( gui );
  }

  dlg.exec();
  
  return 0;
}

FormGui *GuiHandler::createGui( Form *form, QWidget *parent )
{
  kdDebug() << "Manager::createGui() form: '" << form->ref() << "'" << endl;

  if ( !form ) {
    kdError() << "KXForms::Manager::createGui(): form is null." << endl;
    return 0;
  }

  FormGui *gui = new FormGui( mManager, parent );

  return gui;
}

#include "guihandler.moc"
