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

#include "listmodel.h"

#include <klocale.h>
#include <kdebug.h>

using namespace KXForms;

ListItem::ListItem(const QList<QVariant> &data, ListItem *parent)
{
  mParentItem = parent;
  mItemData = data;
}

ListItem::~ListItem()
{
  qDeleteAll(mChildItems);
}

void ListItem::appendChild(ListItem *item)
{
  mChildItems.append(item);
}

ListItem *ListItem::child(int row)
{
  return mChildItems.value(row);
}

void ListItem::removeChild(int row)
{
  delete mChildItems.value(row);
  mChildItems.removeAt( row );
}

int ListItem::childCount() const
{
  return mChildItems.count();
}

int ListItem::row() const
{
  if (mParentItem)
    return mParentItem->mChildItems.indexOf(const_cast<ListItem*>(this));
  return 0;
}

int ListItem::columnCount() const
{
  return mItemData.count();
}

QVariant ListItem::data(int column) const
{
  return mItemData.value(column);
}

ListItem *ListItem::parent()
{
  return mParentItem;
}

void ListItem::moveChild( int from, int to )
{
  mChildItems.move( from, to );
}

void ListItem::setItemData( const QList<QVariant> &data )
{
  mItemData = data;
}

QList<QVariant> ListItem::itemData()
{
  return mItemData;
}





ListModel::ListModel( QObject *parent ) : QAbstractListModel( parent )
{
  QList<QVariant> rootData;
  rootData << "Title";
  rootItem = new ListItem(rootData);
}

ListModel::~ListModel()
{
  delete rootItem;
}

bool ListModel::hasChildren( const QModelIndex &parent ) const
{
  if( parent.isValid() ) {
    ListItem *parentItem =static_cast<ListItem*>(parent.internalPointer());
    if( parentItem )
      return parentItem->childCount() > 0;
    else
      return false;
  } else
    return rootItem->childCount() > 0;
}

int ListModel::rowCount( const QModelIndex &parent ) const
{
  ListItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<ListItem*>(parent.internalPointer());

   return parentItem->childCount();
}

int ListModel::columnCount( const QModelIndex &parent ) const
{
  if (parent.isValid())
    return static_cast<ListItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QModelIndex ListModel::index(int row, int column, const QModelIndex &parent) const
{
  ListItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<ListItem*>(parent.internalPointer());

  ListItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex ListModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  ListItem *childItem = static_cast<ListItem*>(index.internalPointer());
  if( !childItem )
    return QModelIndex();

  ListItem *parentItem = childItem->parent();
  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

QVariant ListModel::data( const QModelIndex & index, int role ) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  ListItem *item = static_cast<ListItem*>(index.internalPointer());

  return item->data(index.column());
}

QVariant ListModel::headerData ( int section, Qt::Orientation orientation,
  int role ) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return rootItem->data(section);

  return QVariant();
}

Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

ListItem *ListModel::addItem( ListItem *parent, const QStringList &labels, const Reference &ref )
{
  kDebug() << k_funcinfo << labels;
  ListItem *parentItem = parent ? parent : rootItem;
  beginInsertRows( QModelIndex(), parentItem->childCount(), parentItem->childCount() );

  QList<QVariant> itemData;
  foreach( QString label, labels )
    itemData << label;
  ListItem *item = new ListItem( itemData, parentItem );
  item->setReference( ref );
  parentItem->appendChild( item );

  endInsertRows();

  return item;
}

int ListModel::itemCount( ListItem *parent, const QString &itemClass )
{
  int count = 0;
  ListItem *item = parent ? parent : rootItem;

  for( int i = 0; i < item->childCount(); ++i ) {
    ListItem *child = item->child( i );
    Reference r = child->ref();
    if( !r.isEmpty() && r.lastSegment().name() == itemClass ) count++;
  }
  
  return count;
}

bool ListModel::removeRows( int row, int count, const QModelIndex &parent )
{
  beginRemoveRows( parent, row, row + count - 1 );
  
  for( int i = 0; i < count; ++i ) {
    ListItem *parentItem = static_cast<ListItem*>(parent.internalPointer());
    if( !parentItem )
      parentItem = rootItem;

    parentItem->removeChild( row );
  }
  
  endRemoveRows();
  
  return true;
}

void ListModel::recalculateSegmentCounts()
{
  recalculateSegmentCounts( rootItem );
}

void ListModel::recalculateSegmentCounts( ListItem *parent )
{
  int startRow = 0;
  int endRow = 0;

  QMap<QString, int> counts;

  for( int i = 0; i < parent->childCount(); ++i ) {
    ListItem *item = parent->child( i );
    int count = 1;
    Reference::Segment segment = item->ref().segments().last();
    QMap<QString, int>::ConstIterator it = counts.find( segment.name() );
    if ( it != counts.end() ) count = it.value();

    if ( count != segment.count() ) {
      item->ref().lastSegment().setCount( count );
    } else {
      startRow++;
    }

    endRow++;

    counts.insert( segment.name(), ++count );

    if( item->childCount() > 0 )
      recalculateSegmentCounts( item );
  }

  emit dataChanged( createIndex( startRow, 1 ), createIndex( endRow, 1 ) );
}

ListItem *ListModel::item( const QModelIndex &index )
{
  if( index.isValid() )
    return static_cast<ListItem*>(index.internalPointer());
  else return 0;
}

void ListModel::setLabel( const QString &label )
{
  mLabel = label;
}

QString ListModel::label() const
{
  return mLabel;
}

void ListModel::setHeaders( const QStringList &headers )
{
  QList<QVariant> data;
  foreach( QString s, headers )
    data << s;
  rootItem->setItemData( data );
}

void ListModel::clear()
{
  if( rowCount() > 0 )
    removeRows( 0, rowCount() );
}

QModelIndex ListModel::moveItem( QModelIndex index, int to )
{
  ListItem *item = static_cast<ListItem*>(index.internalPointer());
  ListItem *parent = item->parent();

  if( !parent )
    return index;

  parent->moveChild( index.row(), to );
  emit dataChanged( index, createIndex( to, 1 ) );
  return createIndex( to, 1 );
}
