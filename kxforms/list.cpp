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

#include "list.h"

#include "form.h"
#include "manager.h"
#include "formgui.h"

#include <kmessagebox.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kinputdialog.h>
#include <kdebug.h>
#include <kdialog.h>

#include <qlayout.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QVBoxLayout>

using namespace KXForms;

ListItem::ListItem( K3ListView *parent, const Reference &ref,
  const QString &label )
  : K3ListViewItem( parent ), mRef( ref )
{
  setText( 0, label );
  setText( 1, mRef.toString() );
}


List::List( Manager *m, const QString &label, QWidget *parent )
  : GuiElement( parent ), mManager( m )
{
  kDebug() << "List() " << label << endl;

  QBoxLayout *topLayout = new QVBoxLayout( this );
  topLayout->setSpacing( KDialog::spacingHint() );

  mListView = new K3ListView( this );
  mListView->addColumn( label );
  mListView->addColumn( i18n("Reference") );
  topLayout->addWidget( mListView );
  connect( mListView, SIGNAL( doubleClicked( Q3ListViewItem *, const QPoint &,
    int ) ), SLOT( editItem() ) );

  QPushButton *button = new QPushButton( i18n("New Item..."), this );
  topLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( newItem() ) );

  button = new QPushButton( i18n("Delete Selected Item"), this );
  topLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( deleteItem() ) );

  button = new QPushButton( i18n("Edit Item..."), this );
  topLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( editItem() ) );

  button = new QPushButton( i18n("Move Item Up"), this );
  topLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( moveUp() ) );

  button = new QPushButton( i18n("Move Item Down"), this );
  topLayout->addWidget( button );
  connect( button, SIGNAL( clicked() ), SLOT( moveDown() ) );
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

  Reference::Segment segment( formRef, mListView->childCount() + 1 );

  mManager->createGui( ref() + segment, this );
}

void List::deleteItem()
{
}

void List::editItem()
{
  ListItem *item = static_cast<ListItem *>( mListView->selectedItem() );
  if ( !item ) return;
  
  mManager->createGui( item->ref(), this );
}

void List::moveUp()
{
}

void List::moveDown()
{
}

void List::parseElement( const QDomElement &element )
{
  QDomNode n;
  for( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() == "itemclass" ) {
      ItemClass c;

      c.setRefName( e.attribute( "ref" ) );

      QDomNode n2;
      for( n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
        QDomElement e2 = n2.toElement();
        if ( e2.tagName() == "itemlabel" ) {
          // TODO: Implement item label support
        }
      }

      mItemClasses.append( c );
    }
  }
}

void List::loadData()
{
  kDebug() << "List::loadData() ref: " << ref().toString() << endl;

  QMap<QString, int> counts;
  QDomNode n;
  for( n = context().firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
//    kDebug() << "E: " << e.tagName() << endl;
    ItemClass ic = itemClass( e.tagName() );
    if ( ic.isValid() ) {
      int count = 1;
      QMap<QString, int>::Iterator it = counts.find( ic.refName() );
      if ( it != counts.end() ) count = it.data();
      Reference::Segment s( ic.refName(), count );
      Reference r = ref() + s;
      QString itemLabel = ic.refName() + QString::number( count );
//      kDebug() << "item label: " << itemLabel << endl;
      new ListItem( mListView, r, itemLabel );
      counts.insert( ic.refName(), ++count );
    }
  }
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
