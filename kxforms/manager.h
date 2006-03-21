/*
    This file is part of KDE.

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
#ifndef KXFORMS_MANAGER_H
#define KXFORMS_MANAGER_H

#include "form.h"
#include "formgui.h"
#include "kresult.h"
#include "guihandler.h"

#include <qdom.h>

namespace KXForms {

class Manager
{
  public:
    Manager();
    ~Manager();

    bool parseForms( const QString & );

    Form *parseForm( const QDomElement & );

    Form *rootForm();

    Form *form( const QString &ref );

    KResult loadData( const QString & );
    KResult saveData( QString & );

    void loadData( FormGui * );

    void clearForms();

    QWidget *createRootGui( QWidget *parent );
    void createGui( const Reference &ref, QWidget *parent );
    void destroyGui( FormGui * );

    bool hasData() const;

  private:
    GuiHandler *mGuiHandler;
  
    Form::List mForms;

    FormGui::List mGuis;

    QDomDocument mData;
    bool mDataLoaded;
};

}

#endif
