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
#ifndef KXFORMS_GUIELEMENT_H
#define KXFORMS_GUIELEMENT_H

#include "reference.h"
#include "editor/editor.h"

#include <QList>
#include <QDomElement>
#include <QLabel>
#include <QMap>
#include <QStackedWidget>

namespace KXForms {

class Manager;

class GuiElement : public QObject
{
    Q_OBJECT
public:
    enum Appearance { Undefined = -1, Minimal, Compact, Full };
    enum LayoutStyle { Horizontal, Vertical };
    struct Properties
    {
        Properties() : readonly(false), appearance(Undefined), layoutStyle(Horizontal) {}
        QString type;
        QString constraint;
        bool readonly;
        QMap<QString, QString> relevance;
        QString group;
        QList<QDomElement> positions;
        Appearance appearance;
        LayoutStyle layoutStyle;
    };

    typedef QList<GuiElement *> List;

    GuiElement(QWidget *parent, Manager *m, Properties *p);
    virtual ~GuiElement();

    virtual void parseElement(const QDomElement &) {}
    static void parseProperties(const QDomElement &, Properties *);

    void setRef(const Reference &);
    Reference ref() const;

    virtual Reference id() const { return mRef; }

    QDomElement context() const;

    QDomElement createElement(const Reference &);

    void loadData(const QDomElement &context);
    void save();

    void setActionTypes(Editor::ActionTypes t) { mActionTypes = t; }
    Editor::ActionTypes actionTypes() const { return mActionTypes; }

    virtual QWidget *widget() const { return mWidget; }
    virtual QWidget *labelWidget() const { return mLabel; }

    virtual void loadData() = 0;
    virtual void saveData() = 0;
    virtual bool isValid() const { return true; }

    virtual int space() const { return 15; }

    Properties *properties() const { return mProperties; }

    void setTip(const QString &tip);

    void addAttributeElement(GuiElement *e) { mAttributeElements.append(e); }
    GuiElement::List attributeElements() { return mAttributeElements; }

signals:
    void valueChanged(const QString &, const QString &);

public slots:
    void slotValueChanged(const QString &, const QString &);
    void setRelevant(bool);

protected:
    virtual void applyProperties();
    virtual void setWidget(QWidget *);

protected:
    QWidget *mParent;
    QLabel *mLabel;
    QWidget *mWidget;
    Manager *mManager;
    Properties *mProperties;
    GuiElement::List mAttributeElements;

private:
    Reference mRef;
    QDomElement mContext;
    Editor::ActionTypes mActionTypes;
};
}

#endif
