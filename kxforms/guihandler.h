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
#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include "formgui.h"

#include <kdialogbase.h>

class QWidget;
class Q3BoxLayout;

namespace KXForms {

class Manager;
class Form;

class FormDialog : public KDialogBase
{
    Q_OBJECT
  public:
    FormDialog( QWidget *parent, const QString &title, Manager * );

    void setGui( FormGui * );

  protected slots:
    void slotOk();

  private:
    Q3BoxLayout *mTopLayout;
    
    FormGui *mFormGui;
    
    Manager *mManager;
};

class GuiHandler
{
  public:
    GuiHandler( Manager * );

    FormGui *createRootGui( QWidget *parent );
    FormGui *createGui( const Reference &ref, QWidget *parent );

  protected:
    FormGui *createGui( Form *form, QWidget *parent );

  private:
    Manager *mManager;    

    FormGui::List mGuis;
};

}

#endif
