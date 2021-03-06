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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef KXFORMS_LIST_H
#define KXFORMS_LIST_H

#include "guielement.h"

#include <QDomElement>
#include <QModelIndex>

class QTreeView;
class KLineEdit;

namespace KXForms {

class ListModel;
class ListItem;
class ListProxyModel;

class List : public GuiElement
{
    Q_OBJECT
public:
    List(Manager *, const QString &label, QWidget *parent, Properties *p);

    struct ListProperties
    {
        ListProperties() : showHeader(false), showFilter(false) {}
        bool showHeader;
        bool showFilter;
    };

    class ItemClass
    {
    public:
        typedef QList<ItemClass> List;

        ItemClass() {}

        bool isValid() const { return !mRef.isEmpty(); }

        void setRefName(const QString &r) { mRef = r; }
        QString refName() const { return mRef; }

        void addLabelDom(const QDomElement &e) { mLabelDoms.append(e); }
        QList<QDomElement> labelDoms() const { return mLabelDoms; }

        void setList(bool l) { mList = l; }
        bool isList() { return mList; }

    private:
        QString mRef;
        QList<QDomElement> mLabelDoms;
        bool mList;
    };

    void parseElement(const QDomElement &);

    void loadData();
    void saveData();

    void setId(const Reference &id) { mId = id; }
    virtual Reference id() const { return mId; }

    int space() const { return 40; }

    ItemClass itemClass(const QString &ref);

    QStringList itemLabels(const ItemClass &itemClass, const QDomElement &itemElement);

    ListProperties listProperties() { return mListProperties; }

protected:
    void loadElement(QDomElement &e, ListItem *parent, QMap<QString, int> &counts);
    QModelIndex selectedItem();

protected slots:
    void newItem();
    void deleteItem();
    void editItem();
    void moveUp();
    void moveDown();

    void resizeColumns();

private:
    ItemClass::List mItemClasses;

    ListProperties mListProperties;

    QTreeView *mView;

    KLineEdit *mFilterEdit;

    ListModel *mModel;
    ListProxyModel *mProxyModel;

    Reference mId;
};
}

#endif
