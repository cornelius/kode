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
#ifndef GUIHANDLERFLAT_H
#define GUIHANDLERFLAT_H

#include "guihandler.h"
#include "formgui.h"

#include <k3activelabel.h>

#include <QObject>
#include <QStack>
#include <QUrl>

class QWidget;
class QBoxLayout;
class QStackedWidget;
class QPushButton;

namespace KXForms {

class Manager;
class Form;
class FormGui;

class BreadCrumbLabel : public K3ActiveLabel
{
    Q_OBJECT
  public:
    BreadCrumbLabel( QWidget *parent );
    
  public slots:
    void openLink( const QUrl &link );

  signals:
    void crumbClicked( int index );
};

class BreadCrumbNavigator : public QFrame
{
    Q_OBJECT
  public:
    BreadCrumbNavigator( QWidget *parent );

    void push( FormGui * );
    FormGui *pop();

    FormGui *last() const;
    int count() const;

  signals:
    void guiSelected( FormGui * );

  protected:
    void updateLabel();

  protected slots:
    void slotCrumbClicked( int index );

  private:
    QStack<FormGui *> mHistory;

    BreadCrumbLabel *mLabel; 
};

class GuiHandlerFlat : public QObject, public GuiHandler
{
    Q_OBJECT
  public:
    GuiHandlerFlat( Manager * );

    QWidget *createRootGui( QWidget *parent );
    void createGui( const Reference &ref, QWidget *parent );

    QLayout *getTopLayout();
    void addWidget( QLayout *, QWidget * );
    void addElement( QLayout *, QWidget *label, QWidget *widget );

  protected:
    FormGui *createGui( Form *form, QWidget *parent );

  protected slots:
    void goBack();

    void showGui( FormGui *gui );

  private:
    QWidget *mMainWidget;
    
    BreadCrumbNavigator *mBreadCrumbNavigator;
    QStackedWidget *mStackWidget;
    QPushButton *mBackButton;
};

}

#endif
