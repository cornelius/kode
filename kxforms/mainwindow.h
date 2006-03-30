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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "manager.h"

#include <kapplication.h>
#include <kmainwindow.h>
#include <kurl.h>

class KPrinter;
class QLabel;
class KUpdate;
class RemoteFile;

/**
 * This class serves as the main window for MainWindow.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Cornelius Schumacher <schumacher@kde.org>
 * @version 0.1
 */
class MainWindow : public KMainWindow
{
    Q_OBJECT
  public:
    MainWindow();
    virtual ~MainWindow();

    bool load( const KUrl & );
    bool save( const KUrl & );

    void loadForm( const KUrl & );

    void setVersion( const QString & );

    KXForms::Manager *formsManager();

  protected:
    /**
     * This function is called when it is time for the app to save its
     * properties for session management purposes.
     */
    void saveProperties(KConfig *);

    /**
     * This function is called when this app is restored.  The KConfig
     * object points to the session management config file that was saved
     * with @ref saveProperties
     */
    void readProperties(KConfig *);

  protected slots:
    void slotGetFormResult( bool ok );
    void slotGetDataResult( bool ok );
    void slotPutDataResult( bool ok );

    bool save();

  private slots:
    void fileNew();
    void fileOpen();
    void fileSaveAs();
    void optionsConfigureToolbars();
    void optionsPreferences();
    void newToolbarConfig();

    void changeStatusbar(const QString& text);
    void changeCaption(const QString& text);

  private:
    void setupAccel();
    void setupActions();

  private:
    KPrinter   *m_printer;

    QLabel *mLabel;

    KXForms::Manager mFormsManager;

    RemoteFile *mFormFile;
    RemoteFile *mDataFile;
};

#endif
