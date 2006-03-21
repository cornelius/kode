#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "manager.h"

#include <kapplication.h>
#include <kmainwindow.h>
#include <kurl.h>
//Added by qt3to4:
#include <QDragEnterEvent>
#include <QLabel>
#include <QDropEvent>

class KPrinter;
class QLabel;
class KUpdate;

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


  private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
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

    KUrl mDataUrl;
};

#endif
