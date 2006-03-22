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

#include "mainwindow.h"

#include "formgui.h"

#include <kprinter.h>
#include <kdeversion.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kaccel.h>
#include <kio/netaccess.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <kurlrequesterdlg.h>
#include <kedittoolbar.h>
#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kdebug.h>
#include <kmessagebox.h>
#include <kfiledialog.h>

#include <QFile>
#include <QTextStream>
#include <QLabel>

MainWindow::MainWindow()
    : KMainWindow( 0, "MainWindow" ),
      m_printer(0)
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

  statusBar()->message( i18n("Ready.") );
}

MainWindow::~MainWindow()
{
  delete m_printer;
}

void MainWindow::setVersion( const QString &version )
{
  mLabel->setText( version );
}

bool MainWindow::load(const KUrl& url)
{
  if ( !url.isValid() ) {
    KMessageBox::sorry( this, i18n("Invalid URL '%1'.")
      .arg( url.prettyURL() ) );    
    return false;
  }

  if ( !url.isLocalFile() ) {
    KMessageBox::sorry( this, i18n("Non-local URL '%1' not supported.")
      .arg( url.prettyURL() ) );
  } else {
    QString filename = url.path();
    QFile f( filename );
    if ( !f.open( QIODevice::ReadOnly ) ) {
      KMessageBox::sorry( this, i18n("Unable to open file '%1'.")
        .arg( filename ) );
    } else {
      QTextStream ts( &f );
      KResult result = mFormsManager.loadData( ts.read() );
      if ( result ) {
        setCaption( url.prettyURL() );
        mDataUrl = url;
        return true;
      } else {
        KMessageBox::sorry( this, result.fullMessage(),
          i18n("Error loading data") );
      }
    }
  }

  return false;
}

bool MainWindow::save( const KUrl & url )
{
  if ( !url.isValid() ) {
    KMessageBox::sorry( this, i18n("Invalid URL '%1'.")
      .arg( url.prettyURL() ) );    
    return false;
  }

  if ( !url.isLocalFile() ) {
    KMessageBox::sorry( this, i18n("Non-local URL '%1' not supported.")
      .arg( url.prettyURL() ) );
  } else {
    QString filename = url.path();
    QFile f( filename );
    if ( !f.open( QIODevice::WriteOnly ) ) {
      KMessageBox::sorry( this, i18n("Unable to open file '%1'.")
        .arg( filename ) );
    } else {
      QString xml;
      KResult result = mFormsManager.saveData( xml );
      if ( !result ) {
        KMessageBox::sorry( this, result.fullMessage(),
          i18n("Error saving data") );
      } else {
        QTextStream ts( &f );
        ts << xml;
        return true;
      }
    }
  }
  
  return false;
}

void MainWindow::setupActions()
{
  KStdAction::openNew(this, SLOT(fileNew()), actionCollection());
  KStdAction::open(this, SLOT(fileOpen()), actionCollection());
  KStdAction::save(this, SLOT(fileSave()), actionCollection());
  KStdAction::saveAs(this, SLOT(fileSaveAs()), actionCollection());
  KStdAction::quit(kapp, SLOT(quit()), actionCollection());

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

void MainWindow::saveProperties(KConfig *config)
{
  // the 'config' object points to the session managed
  // config file.  anything you write here will be available
  // later when this app is restored

  Q_UNUSED( config );
}

void MainWindow::readProperties(KConfig *config)
{
  // the 'config' object points to the session managed
  // config file.  this function is automatically called whenever
  // the app is being restored.  read in here whatever you wrote
  // in 'saveProperties'

  Q_UNUSED( config );
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
  KUrl url = KFileDialog::getOpenURL( QString::null, QString::null, this,
    i18n("Open XML File") );

  if ( !url.isEmpty() ) {
    load( url );
  }
}

void MainWindow::fileSave()
{
  save( mDataUrl );
}

void MainWindow::fileSaveAs()
{
  KUrl file_url = KFileDialog::getSaveURL();
  if ( !file_url.isEmpty() && file_url.isValid() ) {
    save( file_url );
  }
}

void MainWindow::optionsConfigureToolbars()
{
  // use the standard toolbar editor
  saveMainWindowSettings( KGlobal::config(), autoSaveGroup() );
  KEditToolbar dlg(actionCollection());
  connect(&dlg, SIGNAL(newToolbarConfig()), this, SLOT(newToolbarConfig()));
  dlg.exec();
}

void MainWindow::newToolbarConfig()
{
  // this slot is called when user clicks "Ok" or "Apply" in the toolbar editor.
  // recreate our GUI, and re-apply the settings (e.g. "text under icons", etc.)
  createGUI();
  applyMainWindowSettings( KGlobal::config(), autoSaveGroup() );
}

void MainWindow::optionsPreferences()
{
}

void MainWindow::changeStatusbar(const QString& text)
{
  // display the text on the statusbar
  statusBar()->message(text);
}

void MainWindow::changeCaption(const QString& text)
{
  // display the text on the caption
  setCaption(text);
}

void MainWindow::loadForm( const KUrl &url )
{
  if ( !url.isValid() ) {
    KMessageBox::sorry( this, i18n("Invalid URL '%1'.")
      .arg( url.prettyURL() ) );    
    return;
  }

  if ( !url.isLocalFile() ) {
    KMessageBox::sorry( this,
      i18n("Non-local URL '%1' not supported.").arg( url.prettyURL() ) );
  } else {
    QString filename = url.path();
    QFile f( filename );
    if ( !f.open( QIODevice::ReadOnly ) ) {
      KMessageBox::sorry( this, i18n("Unable to open file '%1'.")
        .arg( filename ) );
    } else {
      QTextStream ts( &f );
      if ( !mFormsManager.parseForms( ts.read() ) ) {
        KMessageBox::sorry( this, i18n("Unable to parse kxforms file '%1'.")
          .arg( url.prettyURL() ) );
      } else {
        delete mLabel;
        mLabel = 0;
        QWidget *gui = mFormsManager.createRootGui( this );
        setCentralWidget( gui );
        gui->show();
      }
    }
  }
}

#include "mainwindow.moc"
