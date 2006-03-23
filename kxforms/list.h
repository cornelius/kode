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
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef KXFORMS_LIST_H
#define KXFORMS_LIST_H

#include "guielement.h"

#include <QDomElement>
#include <QList>
#include <QAbstractTableModel>

class QTableView;
class QTreeView;

namespace KXForms {

class Manager;

class ListModel : public QAbstractTableModel
{
  public:
    ListModel( QObject *parent );
    ~ListModel();

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex & index,
      int role = Qt::DisplayRole ) const;
    QVariant headerData ( int section, Qt::Orientation orientation,
      int role = Qt::DisplayRole ) const;
    bool removeRows( int row, int count,
      const QModelIndex &parent = QModelIndex() );

    void recalculateSegmentCounts();

    struct Item
    {
      QString label;
      Reference ref;
    };
    
    void addItem( const QString &label, const Reference &ref );
    Item *item( const QModelIndex &index );

    int itemCount( const QString &itemClass );

    void setLabel( const QString & );
    QString label() const;

  private:
    QList<Item *> mItems;
    QString mLabel;
};

class List : public GuiElement
{
    Q_OBJECT
  public:
    List( Manager *, const QString &label, QWidget *parent );

    class ItemClass
    {
      public:
        typedef QList<ItemClass> List;

        ItemClass() {}

        bool isValid() const { return !mRef.isEmpty(); }

        void setRefName( const QString &r ) { mRef = r; }
        QString refName() const { return mRef; }
        
        void setLabelDom( const QDomElement &e ) { mLabelDom = e; }
        QDomElement labelDom() const { return mLabelDom; } 

      private:
        QString mRef;
        QDomElement mLabelDom;
    };

    void parseElement( const QDomElement & );

    void loadData();
    void saveData();

    ItemClass itemClass( const QString &ref );

    QString itemLabel( const ItemClass &itemClass,
      const QDomElement &itemElement );

  protected:
    QModelIndex selectedItem();

  protected slots:
    void newItem();
    void deleteItem();
    void editItem();
    void moveUp();
    void moveDown();

    void resizeColumns();

  private:
    Manager *mManager;

    ItemClass::List mItemClasses;

    QTreeView *mView;

    ListModel *mModel;
};

}

#endif
