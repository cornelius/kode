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
#ifndef KXFORMS_FORMGUI_H
#define KXFORMS_FORMGUI_H

#include "guielement.h"
#include "layout.h"

#include <QWidget>
#include <QDomElement>
#include <QMap>

class QLayout;
class QLabel;
class QTabWidget;
class QPushButton;

namespace KXForms {

class Manager;


class FormGui : public QWidget
{
    Q_OBJECT
  public:
    typedef QList<FormGui *> List;


    FormGui( const QString &label, Manager *, QWidget *parent );

    void parseElement( const QDomElement &, QLayout *l = 0, const QString &overrideLabel = QString(), Layout *overrideGroup = 0 );

    void setRef( const Reference &ref );
    Reference ref() const;

    QString label() const;

    void loadData( const QDomDocument & );
    void saveData();

    void setLabelHidden( bool );

    GuiElement::List elements();

    QMap< QString, QString > groups() { return mGroups; }
  signals:
    void editingFinished();

  protected:
    void setRefLabel( const Reference &ref );

    void parseAttributeElements( GuiElement *parent, QDomElement &e );

    QDomElement findContextElement( const QDomDocument &doc );

    bool hasGroups( const QDomElement & );
    void setupGroups( QLayout *, const QDomElement &element );
  private:
    Manager *mManager;

    Reference mRef;

    GuiElement::List mGuiElements;

    QLayout *mTopLayout;

    QLabel *mLabel;
    QLabel *mRefLabel;

    QPushButton *mEditButton;

    QTabWidget *mTabWidget;
    QMap< QString, QString > mGroups;

    bool mLabelHidden;
};

}

#endif
