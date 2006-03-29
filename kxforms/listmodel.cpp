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

#include "listmodel.h"

#include <klocale.h>
#include <kdebug.h>

using namespace KXForms;

ListModel::ListModel( QObject *parent ) : QAbstractTableModel( parent )
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
  return 2;
}

QVariant ListModel::data( const QModelIndex & index, int role ) const
{
  if ( role == Qt::DisplayRole ) {
    Item *item = mItems.at( index.row() );
    if ( index.column() == 0 ) return item->label;
    else if ( index.column() == 1 ) return item->ref.toString();
  }
  return QVariant();
}

QVariant ListModel::headerData ( int section, Qt::Orientation orientation,
  int role ) const
{
  if ( orientation == Qt::Horizontal ) {
    if ( role == Qt::DisplayRole ) {
      if ( section == 0 ) {
        if ( mLabel.isEmpty() ) return i18n("Label");
        else return mLabel;
      } else if ( section == 1 ) {
        return i18n("Reference");
      }
    }
  }
  return QVariant();
}

void ListModel::addItem( const QString &label, const Reference &ref )
{
  beginInsertRows( QModelIndex(), mItems.size(), mItems.size() );

  Item *item = new Item;
  item->label = label;
  item->ref = ref;
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
    if ( it != counts.end() ) count = it.data();
    
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
  removeRows( 0, rowCount() );
}

