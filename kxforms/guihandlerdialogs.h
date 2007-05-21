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
#ifndef GUIHANDLERDIALOGS_H
#define GUIHANDLERDIALOGS_H

#include "guihandler.h"
#include "formgui.h"

#include <kdialog.h>

class QWidget;
class QBoxLayout;

namespace KXForms {

class Manager;
class Form;

class FormDialog : public KDialog
{
    Q_OBJECT
  public:
    FormDialog( QWidget *parent, const QString &title, Manager * );

    void setGui( FormGui * );

  signals:
    void aboutToClose();

  protected slots:
    void slotOk();

  private:
    QBoxLayout *mTopLayout;
    
    FormGui *mFormGui;
    
    Manager *mManager;
};

class GuiHandlerDialogs : public QObject, public GuiHandler
{
  Q_OBJECT
  public:
    GuiHandlerDialogs( Manager * );

    QWidget *createRootGui( QWidget *parent );
    void createGui( const Reference &ref, QWidget *parent );

    FormGui *currentGui() { return static_cast<FormGui *>( mWidgetStack.last() ); }

  protected slots:
    void slotDialogClosed();

  protected:
    FormGui *createGui( Form *form, QWidget *parent );

  private:
    FormGui *mRootGui;
    QList< QWidget *> mWidgetStack;
};

}

#endif
