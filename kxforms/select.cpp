/*
    This file is part of KXForms.

    Copyright (c) 2007 Andre Duffeck <aduffeck@suse.de>

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

#include "select.h"
#include "manager.h"
#include "editor/editorwidget.h"

#include <kdebug.h>

#include <QLabel>
#include <QCheckBox>
#include <QListWidget>

using namespace KXForms;

Select::Select(Manager *m, const QString &label, QWidget *parent, Properties *p)
    : GuiElement(parent, m, p)
{
    mManager->dispatcher()->registerElement(this);
    setActionTypes(Editor::CommonActions | Editor::AppearanceActions);

    mLabel = new QLabel(label, parent);
    QWidget *w;
    if (mProperties->appearance == Full) {
        w = new QWidget(parent);
        w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        w->setLayout(new QVBoxLayout(parent));
    } else {
        mListWidget = new QListWidget(parent);
        mListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        mListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        w = mListWidget;
    }

    setWidget(w);
    applyProperties();
}

void Select::parseElement(const QDomElement &formElement)
{
    QDomNode n;
    for (n = formElement.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement e = n.toElement();
        if (e.tagName() == "xf:item") {
            QString label;
            QString value;
            QDomNode n2;
            for (n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling()) {
                QDomElement e2 = n2.toElement();
                if (e2.tagName() == "xf:label") {
                    label = e2.text();
                } else if (e2.tagName() == "xf:value") {
                    value = e2.text();
                }
            }
            if (!label.isEmpty() && !value.isEmpty()) {
                mValues.append(value);
                if (mProperties->appearance == Full) {
                    QCheckBox *chk = new QCheckBox(label, mWidget);
                    mWidget->layout()->addWidget(chk);
                    mCheckBoxes.append(chk);
                } else {
                    mListWidget->addItem(label);
                }
            }
        }
    }
}

void Select::loadData()
{
    kDebug() << ref().toString() << "context:" << context().tagName();

    Reference::Segment s = ref().segments().last();

    QDomNode parent = ref().applyElement(context());
    QDomNode n;
    for (n = parent.firstChild(); !n.isNull(); n = n.nextSibling()) {
        QDomElement e = n.toElement();
        QString s = e.tagName();
        int count = 0;
        QStringList::ConstIterator it;
        for (it = mValues.constBegin(); it != mValues.constEnd(); ++it, ++count) {
            if (*it == s)
                break;
        }
        if (it != mValues.constEnd()) {
            if (mProperties->appearance == Full) {
                QCheckBox *chk = mCheckBoxes[count];
                if (chk)
                    chk->setChecked(true);
            } else {
                mListWidget->item(count)->setSelected(true);
            }
        } else {
            kWarning() << "Select::loadData() unknown value:" << s;
        }
    }
}

void Select::saveData()
{
    kDebug() << "Select::saveData()";
    kDebug() << ref().toString();
    kDebug() << "Context:" << context().nodeName();
    Reference::Segment s = ref().segments().last();

    QStringList values;
    if (mProperties->appearance == Full) {
        for (int i = 0; i < mCheckBoxes.size(); ++i) {
            if (mCheckBoxes[i]->isChecked()) {
                values.append(mValues[i]);
            }
        }
    } else {
        for (int i = 0; i < mListWidget->count(); ++i) {
            if (mListWidget->item(i)->isSelected()) {
                values.append(mValues[i]);
            }
        }
    }

    QDomElement e = ref().applyElement(context());
    if (e.isNull()) {
        e = createElement(ref());
    }
    while (!e.firstChild().isNull())
        e.removeChild(e.firstChild());
    foreach (QString s, values) {
        QDomNode n = mManager->document().createElement(s);
        e.appendChild(n);
    }
}
