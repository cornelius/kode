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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "manager.h"

#include <kxmlguiwindow.h>
#include <kurl.h>

class QLabel;
class RemoteFile;
namespace KXForms {
  class Hints;
}

using namespace KXForms;

/**
 * This class serves as the main window for MainWindow.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Cornelius Schumacher <schumacher@kde.org>
 * @version 0.1
 */
class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
  public:
    MainWindow();
    virtual ~MainWindow();

    bool load( const KUrl & );
    bool save( const KUrl & );

    void loadSchema( const KUrl & );
    void loadHints( const KUrl & );
    void loadForm( const KUrl & );

    void setVersion( const QString & );

    KXForms::Manager *formsManager();

  protected:
    void parseSchema();
    void parseForm( const QString &data );

  protected slots:
    void slotGetSchemaResult( bool ok );
    void slotGetHintsResult( bool ok );
    void slotGetFormResult( bool ok );
    void slotGetDataResult( bool ok );
    void slotPutDataResult( bool ok );

    bool save();

    void refresh( const Hints &h );

  private slots:
    void fileNew();
    void fileOpen();
    void fileSaveAs();
    void optionsPreferences();
    void exportKxformsDocument();

    void changeStatusbar(const QString& text);
    void changeCaption(const QString& text);

  private:
    void setupAccel();
    void setupActions();

  private:
    QLabel *mLabel;

    KXForms::Manager mFormsManager;

    RemoteFile *mFormFile;
    RemoteFile *mDataFile;
    RemoteFile *mSchemaFile;
    RemoteFile *mHintsFile;
    QString mKxformsDocument;
};

#endif
