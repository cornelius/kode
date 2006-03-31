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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "guihandlerflat.h"

#include "manager.h"

#include <kdebug.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QStackedWidget>
#include <QPushButton>
#include <QDebug>

using namespace KXForms;

BreadCrumbLabel::BreadCrumbLabel( QWidget *parent )
  : KActiveLabel( parent )
{
}
    
void BreadCrumbLabel::openLink( const QUrl &link )
{
  emit crumbClicked( link.toString().toInt() );
}

BreadCrumbNavigator::BreadCrumbNavigator( QWidget *parent )
  : QFrame( parent )
{
  QBoxLayout *topLayout = new QHBoxLayout( this );

  QPalette p = palette();
  p.setColor( QPalette::Base, "#ffffa0" );
  setPalette( p );

#if 0
  setFrameStyle( Plain | Panel );
  setLineWidth( 1 );
#endif

  mLabel = new BreadCrumbLabel( this );
  topLayout->addWidget( mLabel );
  mLabel->setPalette( p );
  connect( mLabel, SIGNAL( crumbClicked( int ) ),
    SLOT( slotCrumbClicked( int ) ) );
}

void BreadCrumbNavigator::push( FormGui *gui )
{
  mHistory.push( gui );

  updateLabel();
}

FormGui *BreadCrumbNavigator::pop()
{
  FormGui *result;

  if ( mHistory.isEmpty() ) result = 0;
  else result = mHistory.pop();

  updateLabel();

  result->saveData();
  
  return result;
}

FormGui *BreadCrumbNavigator::last() const
{
  if ( mHistory.isEmpty() ) return 0;
  else return mHistory.last();
}

int BreadCrumbNavigator::count() const
{
  return mHistory.count();
}

void BreadCrumbNavigator::updateLabel()
{
  QString text;
  for( int i = 0; i < mHistory.size(); ++i ) {
    if ( !text.isEmpty() ) {
      text.append( " / " );
    }
    QString label = mHistory[ i ]->label();
    if ( label.isEmpty() ) {
      label = "...";
    }
    text.append( "<a href=\"" + QString::number( i ) + "\">" + label + "</a>" );
  }
  mLabel->setText( text );
}

void BreadCrumbNavigator::slotCrumbClicked( int index )
{
  if ( mHistory.size() <= index ) return;

  FormGui *gui = last();
  gui->saveData();

  while( mHistory.size() > index + 1 ) {
    gui = mHistory.pop();
  }

  updateLabel();
  
  emit guiSelected( last() );
}


GuiHandlerFlat::GuiHandlerFlat( Manager *m )
  : GuiHandler( m )
{
}

QWidget *GuiHandlerFlat::createRootGui( QWidget *parent )
{
  kDebug() << "GuiHandlerFlat::createRootGui()" << endl;

  mMainWidget = new QWidget( parent );
  
  QBoxLayout *topLayout = new QVBoxLayout( mMainWidget );

  mBreadCrumbNavigator = new BreadCrumbNavigator( mMainWidget );
  topLayout->addWidget( mBreadCrumbNavigator );
  connect( mBreadCrumbNavigator, SIGNAL( guiSelected( FormGui * ) ),
    SLOT( showGui( FormGui * ) ) );

  mStackWidget = new QStackedWidget( mMainWidget );
  topLayout->addWidget( mStackWidget, 1 );

  Form *f = manager()->rootForm();

  if ( !f ) {
    KMessageBox::sorry( parent, i18n("Root form not found.") );
    return 0;
  }

  FormGui *gui = createGui( f, mStackWidget );

  gui->setRef( "/" + f->ref() );
  gui->parseElement( f->element() );

  if ( manager()->hasData() ) {
    kDebug() << "Manager::createGui() Load data on creation" << endl;
    manager()->loadData( gui );
  }

  mStackWidget->addWidget( gui );

  QBoxLayout *buttonLayout = new QHBoxLayout( topLayout );
  
  buttonLayout->addStretch( 1 );
  
  mBackButton = new QPushButton( i18n("Back"), mMainWidget );
  buttonLayout->addWidget( mBackButton );
  connect( mBackButton, SIGNAL( clicked() ), SLOT( goBack() ) );
  mBackButton->setEnabled( false );

  return mMainWidget;
}

void GuiHandlerFlat::createGui( const Reference &ref, QWidget *parent )
{
  kDebug() << "GuiHandlerFlat::createGui() ref: '" << ref.toString() << "'" << endl;

  if ( ref.isEmpty() ) {
    KMessageBox::sorry( parent, i18n("No reference.") );
    return;
  }

  QString r = ref.segments().last().name();

  Form *f = manager()->form( r );

  if ( !f ) {
    KMessageBox::sorry( parent, i18n("Form '%1' not found.").arg( ref.toString() ) );
    return;
  }

  FormGui *gui = createGui( f, mMainWidget );
  if ( !gui ) {
    KMessageBox::sorry( parent, i18n("Unable to create GUI for '%1'.")
      .arg( ref.toString() ) );
    return;
  }

  gui->setRef( ref );
  gui->parseElement( f->element() );

  mStackWidget->addWidget( gui );
  mBackButton->setEnabled( true );

  if ( manager()->hasData() ) {
    kDebug() << "Manager::createGui() Load data on creation" << endl;
    manager()->loadData( gui );
  }

  mStackWidget->setCurrentWidget( gui );
}

FormGui *GuiHandlerFlat::createGui( Form *form, QWidget *parent )
{
  kDebug() << "Manager::createGui() form: '" << form->ref() << "'" << endl;

  if ( !form ) {
    kError() << "KXForms::Manager::createGui(): form is null." << endl;
    return 0;
  }

  FormGui *gui = new FormGui( form->label(), manager(), parent );

  if ( gui ) {
    manager()->registerGui( gui );
    mBreadCrumbNavigator->push( gui );
    connect( gui, SIGNAL( editingFinished() ), SLOT( goBack() ) );
  }

  return gui;
}

void GuiHandlerFlat::goBack()
{
  mBreadCrumbNavigator->pop();

  FormGui *current = mBreadCrumbNavigator->last();
  showGui( current );
}

void GuiHandlerFlat::showGui( FormGui *gui )
{
  if ( gui ) {
    mStackWidget->setCurrentWidget( gui );
    manager()->loadData( gui );
  }
  
  if ( mBreadCrumbNavigator->count() == 1 ) {
    mBackButton->setEnabled( false );
  }
}

#include "guihandlerflat.moc"
