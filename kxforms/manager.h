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
#ifndef KXFORMS_MANAGER_H
#define KXFORMS_MANAGER_H

#include "form.h"
#include "formgui.h"
#include "kresult.h"
#include "guihandler.h"
#include "dispatcher.h"
#include "editor/editor.h"

#include <QDomElement>

namespace KXForms {

struct GuiElement::Properties;

class Manager
{
  public:
    Manager();
    ~Manager();

    void setGuiHandler( GuiHandler * );

    bool parseForms( const QString & );

    Form *parseForm( const QDomElement & );

    Form *rootForm();

    Form *form( const QString &ref );

    KResult loadData( const QString & );
    KResult saveData( QString & );

    void loadData( FormGui * );

    void clearForms();

    QWidget *createRootGui( QWidget *parent );
    void createGui( const Reference &ref, GuiElement *parent );

    void registerGui( FormGui * );
    void unregisterGui( FormGui * );

    bool hasData() const;
    QDomDocument document() const;

    QDomElement applyReference( const Reference & );

    QString schemaUri() const;

    QLayout *getTopLayout() const;
    void addWidget( QLayout *, QWidget * ) const;
    void addElementRow( QLayout *, Layout::Element *e, int width, int height ) const;
    void addElementRowElement( QLayout *l, Layout::Element *e, int totalWidth, int totalHeight, int xPosition, int yPosition ) const;

    Dispatcher *dispatcher() const { return mDispatcher; }

    Editor *editor() const { return mEditor; }

    GuiElement::Properties *defaultProperties() { return mDefaultProperties; }

    FormGui *currentGui() { return mGuiHandler->currentGui(); }
  protected:
    void loadData();

  private:
    GuiHandler *mGuiHandler;

    GuiElement::Properties *mDefaultProperties;

    Form::List mForms;

    FormGui::List mGuis;

    QDomDocument mData;
    bool mDataLoaded;

    QString mSchemaUri;

    Dispatcher *mDispatcher;

    Editor *mEditor;
};

}

#endif
