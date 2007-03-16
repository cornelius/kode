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

ListModel::ListModel( QObject *parent ) : QAbstractListModel( parent )
{
}

ListModel::~ListModel()
{
  qDeleteAll( mItems );
}

int ListModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return mItems.size();
}

int ListModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  if( mVisibleElements.size() > 0 )
    return mVisibleElements.size();
  else
    return 1;
}

QVariant ListModel::data( const QModelIndex & index, int role ) const
{
  Item *item = mItems.at( index.row() );
  if( mVisibleElements.size() == 0 ) {
    if ( role == Qt::DisplayRole ) {
      if ( index.column() == 0 ) return item->label;
      else if ( index.column() == 1 ) return item->ref.toString();
    }
    return QVariant();
  } else {
    if ( role == Qt::DisplayRole ) {
      Reference key = mVisibleElements[index.column()].ref;
      QDomElement e = key.applyElement( item->element );
      if( !e.isNull() )
        return e.text();
      else return QVariant();
    }
    return QVariant();
  }
}

QVariant ListModel::headerData ( int section, Qt::Orientation orientation,
  int role ) const
{
  if ( orientation == Qt::Horizontal ) {
    if( mVisibleElements.size() == 0 ) {
    if ( role == Qt::DisplayRole ) {
      if ( section == 0 ) {
        if ( mLabel.isEmpty() ) return i18n("Label");
        else return mLabel;
      } else if ( section == 1 ) {
        return i18n("Reference");
      }
    }
    } else {
      if ( role == Qt::DisplayRole ) {
          return mVisibleElements[section].label;
      }
    }
  }
  return QVariant();
}

void ListModel::addItem( const QString &label, const Reference &ref, QDomElement element )
{
  beginInsertRows( QModelIndex(), mItems.size(), mItems.size() );

  Item *item = new Item;
  item->label = label;
  item->ref = ref;
  item->element = element;
  mItems.append( item );
  
  endInsertRows();
}

int ListModel::itemCount( const QString &itemClass )
{
  int count = 0;

  foreach( Item *item, mItems ) {
    if ( item->ref.lastSegment().name() == itemClass ) count++;
  }
  
  return count;
}

bool ListModel::removeRows( int row, int count, const QModelIndex &parent )
{
  beginRemoveRows( parent, row, row + count - 1 );
  
  for( int i = 0; i < count; ++i ) {
    Item *item = mItems.at( row );
    mItems.removeAt( row );
    delete item;
  }
  
  endRemoveRows();
  
  return true;
}

void ListModel::recalculateSegmentCounts()
{
  int startRow = 0;
  int endRow = 0;

  QMap<QString, int> counts;

  foreach( Item *item, mItems ) {
    int count = 1;
    Reference::Segment segment = item->ref.segments().last();
    QMap<QString, int>::ConstIterator it = counts.find( segment.name() );
    if ( it != counts.end() ) count = it.value();
    
    if ( count != segment.count() ) {
      item->ref.lastSegment().setCount( count );
    } else {
      startRow++;
    }

    endRow++;
    
    counts.insert( segment.name(), ++count );
  }

  emit dataChanged( createIndex( startRow, 1 ), createIndex( endRow, 1 ) );
}

ListModel::Item *ListModel::item( const QModelIndex &index )
{
  return mItems.at( index.row() );
}

void ListModel::setLabel( const QString &label )
{
  mLabel = label;
}

QString ListModel::label() const
{
  return mLabel;
}

void ListModel::clear()
{
  if( rowCount() > 0 )
    removeRows( 0, rowCount() );
}

QModelIndex ListModel::moveItem( int from, int to )
{
  mItems.move( from, to );
  emit dataChanged( createIndex( from, 1 ), createIndex( to, 1 ) );
  return createIndex( to, 1 );
}
