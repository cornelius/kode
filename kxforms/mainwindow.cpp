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

#include "mainwindow.h"

#include "formgui.h"
#include "guihandlerdialogs.h"
#include "remotefile.h"
#include "formcreator.h"

#include <kxml_compiler/schema.h>
#include <kxml_compiler/parserxsd.h>

#include <kmessagebox.h>
#include <klocale.h>
#include <kstdaction.h>
#include <kfiledialog.h>
#include <kstatusbar.h>
#include <kdebug.h>

#include <QFile>
#include <QTextStream>
#include <QLabel>

using namespace KXForms;

MainWindow::MainWindow()
    : KMainWindow()
{
  mLabel = new QLabel( i18n("Welcome!"), this );

  QFont font( "Arial", 100 );
  mLabel->setFont( font );
  mLabel->setAlignment( Qt::AlignCenter );

  // tell the KMainWindow that this is indeed the main widget
  setCentralWidget( mLabel );

  // then, setup our actions
  setupActions();

  // Add typical actions and save size/toolbars/statusbar
  setupGUI();

//  statusBar()->message( i18n("Ready.") );

  mSchemaFile = new RemoteFile( this );
  connect( mSchemaFile, SIGNAL( resultGet( bool ) ),
    SLOT( slotGetSchemaResult( bool ) ) );

  mHintsFile = new RemoteFile( this );
  connect( mHintsFile, SIGNAL( resultGet( bool ) ),
    SLOT( slotGetHintsResult( bool ) ) );

  mFormFile = new RemoteFile( this );
  connect( mFormFile, SIGNAL( resultGet( bool ) ),
    SLOT( slotGetFormResult( bool ) ) );

  mDataFile = new RemoteFile( this );
  connect( mDataFile, SIGNAL( resultGet( bool ) ),
    SLOT( slotGetDataResult( bool ) ) );
  connect( mDataFile, SIGNAL( resultGet( bool ) ),
    SLOT( slotPutDataResult( bool ) ) );
}

MainWindow::~MainWindow()
{
}

KXForms::Manager *MainWindow::formsManager()
{
  return &mFormsManager;
}

void MainWindow::setVersion( const QString &version )
{
  mLabel->setText( version );
}

bool MainWindow::load(const KUrl& url)
{
  if ( !url.isValid() ) {
    KMessageBox::sorry( this, i18n("Invalid URL '%1'.",
        url.prettyUrl() ) );    
    return false;
  }

  mDataFile->get( url );

  return true;
}

bool MainWindow::save( const KUrl & url )
{
  if ( !url.isValid() ) {
    KMessageBox::sorry( this, i18n("Invalid URL '%1'.",
        url.prettyUrl() ) );    
    return false;
  }
  
  mDataFile->setUrl( url );

  return save();
}

bool MainWindow::save()
{
  QString xml;
  KResult result = mFormsManager.saveData( xml );
  if ( result ) {
    mDataFile->put( xml );
    return true;
  } else {
    KMessageBox::sorry( this, result.fullMessage(),
      i18n("Error saving data") );
    return false;
  }
}

void MainWindow::setupActions()
{
  KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
  KStdAction::open(this, SLOT(fileOpen()), actionCollection());
  KStdAction::save(this, SLOT(save()), actionCollection());
  KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
  KStdAction::quit(this, SLOT(close()), actionCollection());

  KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

#if 0
  // this doesn't do anything useful.  it's just here to illustrate
  // how to insert a custom menu and menu item
  KAction *custom = new KAction(i18n("Cus&tom Menuitem"), 0,
                                this, SLOT(optionsPreferences()),
                                actionCollection(), "custom_action");
  Q_UNUSED( custom );
#endif
}

void MainWindow::fileNew()
{
  // this slot is called whenever the File->New menu is selected,
  // the New shortcut is pressed (usually CTRL+N) or the New toolbar
  // button is clicked

  // create a new window
  (new MainWindow)->show();
}

void MainWindow::fileOpen()
{
  KUrl url = KFileDialog::getOpenUrl( KUrl(), QString::null, this,
    i18n("Open XML File") );

  if ( !url.isEmpty() ) {
    load( url );
  }
}

void MainWindow::fileSaveAs()
{
  KUrl file_url = KFileDialog::getSaveUrl();
  if ( !file_url.isEmpty() && file_url.isValid() ) {
    save( file_url );
  }
}

void MainWindow::optionsPreferences()
{
}

void MainWindow::changeStatusbar(const QString& text)
{
  // display the text on the statusbar
  statusBar()->showMessage(text);
}

void MainWindow::changeCaption(const QString& text)
{
  // display the text on the caption
  setCaption(text);
}

void MainWindow::loadSchema( const KUrl &url )
{
  kDebug() << "MainWindow::loadSchema() " << url.prettyUrl() << endl;

  if ( !url.isValid() ) {
    KMessageBox::sorry( this, i18n("Invalid URL '%1'.",
        url.prettyUrl() ) );    
    return;
  }

  mSchemaFile->get( url );
}

void MainWindow::slotGetSchemaResult( bool ok )
{
  if ( !ok ) {
    return;
  }

  if ( !mHintsFile->isValid() || !mHintsFile->isLoading() ) {
    parseSchema();
  }
}

void MainWindow::parseSchema()
{
  ParserXsd parser;
  Schema::Document schemaDocument = parser.parse( mSchemaFile->data() );

  KXForms::FormCreator creator;

  Hints schemaHints;
  schemaHints.extractHints( schemaDocument );
  
//  schemaHints.dump();
  
  creator.setHints( schemaHints );

  if ( mHintsFile->isLoaded() ) {
    KXForms::Hints hints;
    hints.parseString( mHintsFile->data() );
//    hints.dump();
    creator.mergeHints( hints );
  }
  
  creator.hints().dump();
  QString form = creator.create( schemaDocument );
 
  parseForm( form );
}

void MainWindow::loadHints( const KUrl &url )
{
  kDebug() << "MainWindow::loadHints() " << url.prettyUrl() << endl;

  if ( !url.isValid() ) {
    KMessageBox::sorry( this, i18n("Invalid URL '%1'.",
        url.prettyUrl() ) );    
    return;
  }

  mHintsFile->get( url );
}

void MainWindow::slotGetHintsResult( bool ok )
{
  if ( !ok ) {
    return;
  }

  if ( mSchemaFile->isLoaded() ) {
    parseSchema();
  }
}

void MainWindow::loadForm( const KUrl &url )
{
  if ( !url.isValid() ) {
    KMessageBox::sorry( this, i18n("Invalid URL '%1'.",
        url.prettyUrl() ) );    
    return;
  }

  mFormFile->get( url );
}

void MainWindow::slotGetFormResult( bool ok )
{
  if ( !ok ) {
    return;
  }

  parseForm( mFormFile->data() );
}

void MainWindow::parseForm( const QString &data )
{
  if ( !mFormsManager.parseForms( data ) ) {
    KMessageBox::sorry( this, i18n("Unable to parse kxforms file '%1'.",
        mFormFile->url().prettyUrl() ) );
  } else {
    delete mLabel;
    mLabel = 0;
    QWidget *gui = mFormsManager.createRootGui( this );
    setCentralWidget( gui );
    gui->show();
  }
}

void MainWindow::slotGetDataResult( bool ok )
{
  if ( !ok ) {
    return;
  }

  KResult result = mFormsManager.loadData( mDataFile->data() );
  if ( result ) {
    setCaption( mDataFile->url().prettyUrl() );
  } else {    
    KMessageBox::sorry( this, result.fullMessage(),
      i18n("Error loading data") );
  }

  if ( !mSchemaFile->isValid() && !mFormsManager.schemaUri().isEmpty() ) {
    loadSchema( mFormsManager.schemaUri() );
  }
}

void MainWindow::slotPutDataResult( bool ok )
{
  if ( !ok ) {
    return;
  }
}

#include "mainwindow.moc"
