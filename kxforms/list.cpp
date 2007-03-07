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

#include "list.h"

#include "form.h"
#include "manager.h"
#include "formgui.h"
#include "listmodel.h"
#include "prefs.h"

#include <kmessagebox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kinputdialog.h>
#include <kdebug.h>
#include <kdialog.h>

#include <QPushButton>
#include <QVBoxLayout>
#include <QTreeView>
#include <QTimer>

using namespace KXForms;

List::List( Manager *m, const QString &label, QWidget *parent )
  : GuiElement( parent ), mManager( m )
{
  kDebug() << "List() " << label << endl;

  QBoxLayout *topLayout;
  if ( Prefs::verticalListButtons() ) {
    topLayout = new QHBoxLayout( this );
  } else {
    topLayout = new QVBoxLayout( this );
  }

  mModel = new ListModel( this );
  mModel->setLabel( label );
  mView = new QTreeView( this );
  topLayout->addWidget( mView );
  mView->setModel( mModel );
  connect( mView, SIGNAL( doubleClicked( const QModelIndex & ) ),
    SLOT( editItem() ) );

  QBoxLayout *buttonLayout;
  if ( Prefs::verticalListButtons() ) {
    buttonLayout = new QVBoxLayout();
  } else {
    buttonLayout = new QHBoxLayout();
  }
  topLayout->addLayout( buttonLayout );

  if ( Prefs::verticalListButtons() ) {
    buttonLayout->addStretch( 1 );
  }

  QPushButton *button = new QPushButton( i18n("New Item..."), this );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( newItem() ) );

  button = new QPushButton( i18n("Delete Selected Item"), this );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( deleteItem() ) );

  button = new QPushButton( i18n("Edit Item..."), this );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( editItem() ) );

  button = new QPushButton( i18n("Move Item Up"), this );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( moveUp() ) );

  button = new QPushButton( i18n("Move Item Down"), this );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( moveDown() ) );

  if ( !Prefs::developerMode() ) {
    mView->hideColumn( 1 );
  }
}

void List::newItem()
{
  QString formRef;

  if ( mItemClasses.count() == 1 ) {
    formRef = mItemClasses.first().refName();
  } else if ( mItemClasses.count() > 1 ) {
    QStringList items;
    ItemClass::List::ConstIterator it;
    for( it = mItemClasses.begin(); it != mItemClasses.end(); ++it ) {
      items.append( (*it).refName() );
    }
    formRef = KInputDialog::getItem( i18n("Select item class"),
      i18n("Select which type of item you want to create."),
      items, 0, false, 0, this );
    if ( formRef.isEmpty() ) return;
  } else {
    KMessageBox::sorry( this, i18n("No item classes.") );
    return;
  }

  QDomElement newElement = mManager->document().createElement( formRef );
  context().appendChild( newElement );

  Reference::Segment segment( formRef, mModel->itemCount( formRef ) + 1 );

  Reference reference = ref();
  if ( segment.name() != "." ) reference.append( segment );

  mManager->createGui( reference, this );

  QString il = itemLabel( itemClass( formRef ), newElement );
  mModel->addItem( il, ref() + segment );
}

void List::deleteItem()
{
  QModelIndex index = selectedItem();

  if ( !index.isValid() ) return;

  ListModel::Item *item = mModel->item( index );

  int result = KMessageBox::warningContinueCancel( this,
    i18n("Delete item '%1'?", item->label ) );
  if ( result == KMessageBox::Continue ) {
    QDomElement element = mManager->applyReference( item->ref );
    element.parentNode().removeChild( element );
    mModel->removeRows( index.row(), 1 );
    mModel->recalculateSegmentCounts();
  }
}

QModelIndex List::selectedItem()
{
  QModelIndexList selected = mView->selectionModel()->selectedIndexes();
  if ( selected.isEmpty() ) return QModelIndex();
  else return selected.first();
}

void List::editItem()
{
  QModelIndex index = selectedItem();
  if ( index.isValid() ) {
    mManager->createGui( mModel->item( index )->ref, this );
  }
}

void List::moveUp()
{
  QModelIndex index = selectedItem();
  if ( !index.isValid() || index.row() == 0 ) return;
  
  
}

void List::moveDown()
{
  QModelIndex index = selectedItem();
  if ( !index.isValid() || index.row() == mModel->rowCount() - 1 ) return;


}

void List::parseElement( const QDomElement &element )
{
  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "itemclass" ) {
      ItemClass c;

      c.setRefName( Reference( e.attribute( "ref" ) ).lastSegment().name() );

      QDomNode n2;
      for( n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
        QDomElement e2 = n2.toElement();
        if ( e2.tagName() == "itemlabel" ) {
          c.setLabelDom( e2 );
        }
      }

      mItemClasses.append( c );
    }
  }
}

void List::loadData()
{
  kDebug() << "List::loadData() ref: " << ref().toString() << endl;

  mModel->clear();

  QMap<QString, int> counts;
  QDomNode n;
  for( n = context().firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
//    kDebug() << "E: " << e.tagName() << endl;
    ItemClass ic = itemClass( e.tagName() );
    if ( ic.isValid() ) {
      int count = 1;
      QMap<QString, int>::Iterator it = counts.find( ic.refName() );
      if ( it != counts.end() ) count = it.value();
      Reference::Segment s( ic.refName(), count );
      Reference r = ref() + s;

      QString il = itemLabel( ic, e );
//      kDebug() << "item label: " << il << endl;
      mModel->addItem( il, r );
      counts.insert( ic.refName(), ++count );
    }
  }

  QTimer::singleShot( 0, this, SLOT( resizeColumns() ) );
}

QString List::itemLabel( const ItemClass &itemClass,
  const QDomElement &itemElement )
{
  QString itemLabel;
  QDomNode n;
  for( n = itemClass.labelDom().firstChild(); !n.isNull();
       n = n.nextSibling() ) {
    if ( n.isText() ) {
      itemLabel.append( n.toText().data() );
    } else if ( n.isElement() ) {
      QDomElement e2 = n.toElement();
      if ( e2.tagName() != "arg" ) {
        kWarning() << "Illegal tag in itemlabel element: " << e2.tagName()
          << endl;
      } else {
        Reference ref( e2.attribute( "ref" ) );
        QString txt = ref.applyString( itemElement );
        if ( e2.hasAttribute( "truncate" ) ) {
          QString truncate = e2.attribute( "truncate" );
          bool ok;
          int newLen = truncate.toInt( &ok );
          if ( !ok ) {
            kError() << "Illegal truncate value: '" << truncate << "'"
              << endl;
          } else {
            if ( int( txt.length() ) > newLen ) {
              txt.truncate( newLen );
              txt += "...";
            }
          }
        }
        itemLabel.append( txt );
      }
    }
  }
  return itemLabel;
}

void List::resizeColumns()
{
  mView->resizeColumnToContents( 0 );
}

void List::saveData()
{
  kDebug() << "List::saveData()" << endl;
}

List::ItemClass List::itemClass( const QString &ref )
{
  ItemClass::List::ConstIterator it;
  for( it = mItemClasses.begin(); it != mItemClasses.end(); ++it ) {
    if ( (*it).refName() == ref ) return *it;
  }
  return ItemClass();
}

#include "list.moc"
