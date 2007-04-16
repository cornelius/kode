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
#include "listproxymodel.h"
#include "prefs.h"

#include <kmessagebox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kinputdialog.h>
#include <kdebug.h>
#include <kdialog.h>
#include <klineedit.h>

#include <QPushButton>
#include <QVBoxLayout>
#include <QTreeView>
#include <QTimer>
#include <QHeaderView>

using namespace KXForms;

List::List( Manager *m, const QString &label, QWidget *parent, Properties *p )
  : GuiElement( parent, m, p )
{
  mManager->dispatcher()->registerElement( this );
  kDebug() << "List() " << label << endl;
  mWidget = new QWidget( mParent );
  mLabel = new QLabel( label );

  QBoxLayout *topLayout;
  if ( Prefs::verticalListButtons() ) {
    topLayout = new QHBoxLayout( mWidget );
  } else {
    topLayout = new QVBoxLayout( mWidget );
  }
  topLayout->setMargin( 0 );

  mModel = new ListModel( mWidget );
  mProxyModel = new ListProxyModel( mWidget );
  mProxyModel->setSourceModel( mModel );

  mView = new QTreeView( mWidget );
  mFilterEdit = new KLineEdit;
  mFilterEdit->setClearButtonShown( true );
  mFilterEdit->hide();
  connect( mFilterEdit, SIGNAL(textChanged(QString)), mProxyModel, SLOT(setFilterFixedString(QString)) );


  topLayout->addWidget( mFilterEdit );
  topLayout->addWidget( mView );
  mView->setModel( mProxyModel );
  mView->header()->hide();
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

  QPushButton *button = new QPushButton( i18n("New Item..."), mWidget );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( newItem() ) );

  button = new QPushButton( i18n("Delete Selected Item"), mWidget );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( deleteItem() ) );

  button = new QPushButton( i18n("Edit Item..."), mWidget );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( editItem() ) );

  button = new QPushButton( i18n("Move Item Up"), mWidget );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( moveUp() ) );

  button = new QPushButton( i18n("Move Item Down"), mWidget );
  buttonLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( moveDown() ) );

  if ( !Prefs::developerMode() ) {
    mView->hideColumn( 1 );
  }

  applyProperties();
}

void List::newItem()
{
  QString formRef;
// 
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
      items, 0, false, 0, mParent );
    if ( formRef.isEmpty() ) return;
  } else {
    KMessageBox::sorry( mParent, i18n("No item classes.") );
    return;
  }

  QDomElement newElement = mManager->document().createElement( formRef );
  QDomElement lastListElement = context().lastChildElement( formRef );
  if( !lastListElement.isNull() )
    context().insertAfter( newElement, lastListElement );
  else
    context().appendChild( newElement );

  Reference::Segment segment( formRef, mModel->itemCount( 0, formRef ) + 1 );

  Reference reference = ref();
  if ( segment.name() != "." ) reference.append( segment );

  QStringList il = itemLabels( itemClass( formRef ), newElement );
  mModel->addItem( 0, il, ref() + segment );

  mManager->createGui( reference, this );

}

void List::deleteItem()
{
  QModelIndex index = mProxyModel->mapToSource( selectedItem() );

  if ( !index.isValid() ) return;

  ListItem *item = mModel->item( index );

  int result = KMessageBox::warningContinueCancel( mWidget,
    i18n("Delete item '%1'?", item->data( 0 ).toString() ) );
  if ( result == KMessageBox::Continue ) {
    QDomElement element = mManager->applyReference( item->ref() );
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
  QModelIndex index = mProxyModel->mapToSource( selectedItem() );
  if ( index.isValid() ) {
    mManager->createGui( mModel->item( index )->ref(), this );
  }
}

void List::moveUp()
{
  QModelIndex index = mProxyModel->mapToSource( selectedItem() );
  if ( !index.isValid() || index.row() == 0 ) return;

  QString ref = mModel->item( index )->ref().lastSegment().name();
  QDomNode oldNode = context().firstChildElement( ref );
  int cnt = 0;
  while( cnt++ < index.row() && !oldNode.isNull() )
    oldNode = oldNode.nextSiblingElement( ref );
  if( !oldNode.isNull() ) {
    QDomNode newNode = oldNode.cloneNode();
    context().insertBefore( newNode, oldNode.previousSiblingElement( ref ) );
    context().removeChild( oldNode );
  }

  QModelIndex newIndex = mModel->moveItem( index, index.row() - 1 );
  mView->setCurrentIndex( mProxyModel->mapFromSource( newIndex ) );
}

void List::moveDown()
{
  QModelIndex index = mProxyModel->mapToSource( selectedItem() );
  if ( !index.isValid() || index.row() == mModel->rowCount() - 1 ) return;

  QString ref = mModel->item( index )->ref().lastSegment().name();
  QDomNode oldNode = context().firstChildElement( ref );
  int cnt = 0;
  while( cnt++ < index.row() && !oldNode.isNull() )
    oldNode = oldNode.nextSiblingElement( ref );
  if( !oldNode.isNull() ) {
    QDomNode newNode = oldNode.cloneNode();
    context().insertAfter( newNode, oldNode.nextSiblingElement( ref ) );
    context().removeChild( oldNode );
  }

  QModelIndex newIndex = mModel->moveItem( index, index.row() + 1 );
  mView->setCurrentIndex( mProxyModel->mapFromSource( newIndex ) );
}

void List::parseElement( const QDomElement &element )
{
  QDomNode n;
  if( element.attribute( "showHeader" ) == "true" )
    mView->header()->show();
  if( element.attribute( "showSearch" ) == "true" )
    mFilterEdit->show();
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "itemclass" ) {
      ItemClass c;

      c.setRefName( Reference( e.attribute( "ref" ) ).lastSegment().name() );
      c.setList( e.attribute( "list" ) == "true" );
      QDomNode n2;
      for( n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
        QDomElement e2 = n2.toElement();
        if ( e2.tagName() == "itemlabel" ) {
          c.addLabelDom( e2 );
        }
      }

      mItemClasses.append( c );
    } else if( e.tagName() == "headers" ) {
      QStringList headers;
      QDomNode n2;
      for( n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
        QDomElement e = n2.toElement();
        if( e.isNull() )
          continue;
        if( e.tagName() == "header" ) {
          headers.append( e.text() );
        }
      }
      mModel->setHeaders( headers );
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

    loadElement( e, 0, counts );
//    kDebug() << "E: " << e.tagName() << endl;
  }

  QTimer::singleShot( 0, this, SLOT( resizeColumns() ) );
}

void List::loadElement( QDomElement &e, ListItem *parent, QMap<QString, int> &counts )
{
  QMap<QString, int> myCounts;
  ItemClass ic = itemClass( e.tagName() );
  if ( ic.isValid() ) {
    int count = 1;
    QMap<QString, int>::Iterator it = counts.find( ic.refName() );
    if ( it != counts.end() ) count = it.value();
    Reference::Segment s( ic.refName(), count );
    Reference r;
    if( parent )
      r = parent->ref() + s;
    else
      r = ref() + s;
    QStringList il = itemLabels( ic, e );
//   kDebug() << "item label: " << il << endl;
    ListItem *item = mModel->addItem( parent, il, r );

    if( ic.isList() ) {
      QDomNode n2;
      for( n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
        QDomElement e2 = n2.toElement();
        loadElement( e2, item, myCounts );
      }
    }
    counts.insert( ic.refName(), ++count );
  }
}

QStringList List::itemLabels( const ItemClass &itemClass,
  const QDomElement &itemElement )
{
  QStringList itemLabels;
  QDomNode labelDom;
  QDomNode n;
  foreach( QDomNode labelDom, itemClass.labelDoms() ) {
  QString label;
  for( n = labelDom.firstChild(); !n.isNull();
       n = n.nextSibling() ) {
    if ( n.isText() ) {
      label.append( n.toText().data() );
    } else if ( n.isElement() ) {
      QDomElement e2 = n.toElement();
      if ( e2.tagName() != "itemLabelArg" ) {
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
        label.append( txt );
      }
    }
  }
  itemLabels.append( label );
  }
  return itemLabels;
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
