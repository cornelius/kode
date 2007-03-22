/*
    This file is part of KXForms.

    Copyright (c) 2005,2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "guihandlerdialogs.h"

#include "manager.h"

#include <kdebug.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>

using namespace KXForms;

FormDialog::FormDialog( QWidget *parent, const QString &title, Manager *m )
  : KDialog( parent ),
    mFormGui( 0 ), mManager( m )
{
  QFrame *topFrame = new QFrame( this );
  setMainWidget( topFrame );
  mTopLayout = new QVBoxLayout( topFrame );

  setCaption( title );
  setButtons( Ok );
  connect( this, SIGNAL( okClicked() ), SLOT( slotOk() ) );
}

void FormDialog::setGui( FormGui *gui )
{
  mTopLayout->addWidget( gui );
  mFormGui = gui;
}

void FormDialog::slotOk()
{
  kDebug() << "FormDialog::slotOk()" << endl;

  mFormGui->saveData();

  mManager->unregisterGui( mFormGui );

  accept();
}


GuiHandlerDialogs::GuiHandlerDialogs( Manager *m )
  : GuiHandler( m )
{
}

QWidget *GuiHandlerDialogs::createRootGui( QWidget *parent )
{
  kDebug() << "GuiHandlerDialogs::createRootGui()" << endl;

  Form *f = manager()->rootForm();

  if ( !f ) {
    KMessageBox::sorry( parent, i18n("Root form not found.") );
    return 0;
  }

  FormGui *gui = createGui( f, parent );

  gui->setRef( '/' + f->ref() );
  gui->parseElement( f->element() );

  if ( manager()->hasData() ) {
    kDebug() << "Manager::createGui() Load data on creation" << endl;
    manager()->loadData( gui );
  }

  return gui;
}

void GuiHandlerDialogs::createGui( const Reference &ref, QWidget *parent )
{
  kDebug() << "GuiHandlerDialogs::createGui() ref: '" << ref.toString() << "'" << endl;

  if ( ref.isEmpty() ) {
    KMessageBox::sorry( parent, i18n("No reference.") );
    return;
  }

  QString r = ref.segments().last().name();

  Form *f = manager()->form( r );

  if ( !f ) {
    KMessageBox::sorry( parent, i18n("Form '%1' not found.", ref.toString() ) );
    return;
  }

  FormDialog dlg( parent, i18n("Edit %1", ref.toString() ), manager() );

  FormGui *gui = createGui( f, dlg.mainWidget() );
  if ( !gui ) {
    KMessageBox::sorry( parent, i18n("Unable to create GUI for '%1'.",
        ref.toString() ) );
    return;
  }

  gui->setRef( ref );
  gui->parseElement( f->element() );

  dlg.setGui( gui );

  if ( manager()->hasData() ) {
    kDebug() << "Manager::createGui() Load data on creation" << endl;
    manager()->loadData( gui );
  }

  dlg.exec();
}

FormGui *GuiHandlerDialogs::createGui( Form *form, QWidget *parent )
{
  if ( !form ) {
    kError() << "KXForms::Manager::createGui(): form is null." << endl;
    return 0;
  }

  kDebug() << "Manager::createGui() form: '" << form->ref() << "'" << endl;

  FormGui *gui = new FormGui( form->label(), manager(), parent );
  if ( gui ) manager()->registerGui( gui );

  return gui;
}

QLayout *GuiHandlerDialogs::getTopLayout()
{
  if( layoutStyle() == GuiHandler::Grid ) {
    QGridLayout *l = new QGridLayout();
    return l;
  }
  else {
    QVBoxLayout *l = new QVBoxLayout();
    l->addStretch( 1 );
    return l;
  }
}

void GuiHandlerDialogs::addWidget( QLayout *l, QWidget *w )
{
  if( layoutStyle() == GuiHandler::Grid ) {
    QGridLayout *gl = dynamic_cast< QGridLayout *>( l );
    if( !gl )
      return;

    gl->addWidget( w, gl->rowCount(), 0, 1, 2 );
  }
  else {
    QVBoxLayout *vbl = dynamic_cast< QVBoxLayout *>( l );
    if( !vbl )
      return;

    vbl->insertWidget( l->count() - 1, w );
  }
}

void GuiHandlerDialogs::addElement( QLayout *l, QWidget *label, QWidget *widget, GuiElement::Properties *prop )
{
  Q_UNUSED( prop );
  if( layoutStyle() == GuiHandler::Grid ) {
    QGridLayout *gl = dynamic_cast< QGridLayout *>( l );
    if( !gl )
      return;

    int row = gl->rowCount();
    if( label ) {
      gl->addWidget( label, row, 0, prop->alignment() );
      gl->addWidget( widget, row, 1, prop->alignment() );
    }
    else {
      gl->addWidget( widget, row, 0, 1, 2, prop->alignment() );
    }
  }
  else {
    QVBoxLayout *vbl = dynamic_cast< QVBoxLayout *>( l );
    if( !vbl )
      return;

    QBoxLayout *newLayout;
    newLayout = new QVBoxLayout();

    newLayout->addWidget( label, prop->alignment() );
    newLayout->addWidget( widget, prop->alignment() );

    vbl->insertLayout( l->count() - 1, newLayout );
  }
}

#include "guihandlerdialogs.moc"
