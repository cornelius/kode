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

#include "formgui.h"

#include "xformscommon.h"
#include "list.h"
#include "input.h"
#include "textarea.h"
#include "select1.h"
#include "section.h"
#include "prefs.h"
#include "manager.h"
#include "select.h"

#include <kdebug.h>
#include <klocale.h>
#include <kdialog.h>
#include <kmessagebox.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>

using namespace KXForms;


FormGui::FormGui( const QString &label, Manager *m, QWidget *parent )
  : QWidget( parent ), mManager( m ), mTabWidget( 0 ), mLabelHidden( false )
{
  kDebug() << "FormGui()" << endl;

  mTopLayout = mManager->getTopLayout();
  setLayout( mTopLayout );

  mLabel = new QLabel( label, this );
  QFont f = mLabel->font();
  f.setBold( true );
  mLabel->setFont( f );
  mLabel->hide();
  mManager->addWidget( mTopLayout, mLabel );

  mRefLabel = new QLabel( this );
  f = mRefLabel->font();
  f.setPointSize( f.pointSize() - 2 );
  mRefLabel->setFont( f );
  mManager->addWidget( mTopLayout, mRefLabel );

  //TODO: turn this into a KAction
  mEditButton = new QPushButton( "Toggle Edit Mode", this );
  connect( mEditButton, SIGNAL(clicked()), mManager->editor(), SLOT(toggleEditMode()) );
  mManager->addWidget( mTopLayout, mEditButton );

  setRefLabel( "[undefined reference]" );

  if ( !Prefs::developerMode() ) {
    mRefLabel->hide();
  }
}

void FormGui::setRef( const Reference &ref )
{
  mRef = ref;

  if ( mRef.isEmpty() ) setRefLabel( i18n("[empty reference]") );
  else setRefLabel( mRef );
}

void FormGui::setRefLabel( const Reference &ref )
{
  mRefLabel->setText( i18n("Reference: %1", ref.toString() ) );
}

Reference FormGui::ref() const
{
  return mRef;
}

QString FormGui::label() const
{
  return mLabel->text();
}

void FormGui::setLabelHidden( bool hidden )
{
  mLabelHidden = hidden;
}

void FormGui::parseElement( const QDomElement &element, QLayout *l, const QString &overrideLabel, Layout *overrideGroup )
{
  kDebug() << "FormGui::parseElement()" << endl;

  QMap< QString, Layout > layoutMap;

  bool hasList = false;
  QLayout *layout = l ? l : mTopLayout;

  QDomNode n;
  for ( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    QString tag = e.tagName();
    kDebug() << "  Found element '" << tag << "'" << endl;

    XFormsCommon c = XFormsCommon::parseElement( e );
    if( !overrideLabel.isEmpty() )
      c.setLabel( overrideLabel );

    GuiElement *guiElement = 0;
    GuiElement::Properties *properties = new GuiElement::Properties();
    *properties = *mManager->defaultProperties();
    GuiElement::parseProperties( e, properties );

    if ( tag == "xf:label" ) {
      mLabel->setText( e.text() );
      if ( !mLabelHidden ) mLabel->show();
    } else if ( tag == "list" ) {
      guiElement = new KXForms::List( mManager, c.label(), this, properties );
      guiElement->setRef( e.attribute( "ref" ) );
      hasList = true;
    } else if ( tag == "xf:input" ) {
      Input *input = new Input( mManager, c.label(), this, properties );
      connect( input, SIGNAL( returnPressed() ), SIGNAL( editingFinished() ) );
      guiElement = input;
      guiElement->setRef( e.attribute( "ref" ) );
    } else if ( tag == "xf:textarea" ) {
      guiElement = new TextArea( mManager, c.label(), this, properties );
      guiElement->setRef( e.attribute( "ref" ) );
    } else if ( tag == "xf:select1" ) {
      guiElement = new Select1( mManager, c.label(), this, properties );
      guiElement->setRef( e.attribute( "ref" ) );
    } else if ( tag == "xf:select" ) {
      guiElement = new Select( mManager, c.label(), this, properties );
      guiElement->setRef( e.attribute( "ref" ) );
    } else if ( tag == "kxf:section" ) {
      if( e.attribute( "visible" ) != "false" ) {
        bool externalLabel = (e.attribute( "externalLabel" ) == "true" );
        guiElement = new Section( mManager, c.label(), this, properties, externalLabel );
        guiElement->setRef( e.attribute( "ref" ) );
        parseElement( e, static_cast<Section *>( guiElement )->layout(), e.attribute( "overrideLabel" ));
      } else {
        parseElement( e, layout, e.attribute( "overrideLabel" ), &layoutMap[properties->group] );
      }
    } else if ( tag == "attributes" ) {
      parseElement( e, layout, QString(), &layoutMap[properties->group] );
    } else {
      kWarning() << "  Unsupported element: " << tag << endl;
      delete properties;
    }
    if( guiElement ) {
      if( !c.tip().isEmpty() )
        guiElement->setTip( c.tip() );
      guiElement->parseElement( e );
      parseAttributeElements( guiElement, e );
      if( overrideGroup )
        overrideGroup->addElement( guiElement );
      else
        layoutMap[properties->group].addElement( guiElement );
    }
  }

  QMap< QString, Layout >::iterator it;
  int threshold = 100;
  int space = 0;
  bool grouped = false;

  int totalSpace = 0;
  for( it = layoutMap.begin(); it != layoutMap.end(); ++it ){
    it.value().order();
    totalSpace += it.value().space();
  }

  if( (hasGroups( element ) || totalSpace > threshold) &&
       layoutMap.size() > 1 ) {
    setupGroups( layout, element );
    layout = 0;
    grouped = true;
  }

  for( it = layoutMap.begin(); it != layoutMap.end(); ++it ) {
    it.value().order();
    QList< Layout::Element * > list = it.value().orderedList();

    space += it.value().space();
    int height = it.value().height();
    int width = it.value().width();

    if( ( space > threshold && grouped ) || !layout ) {
      space = it.value().space();
      layout = mManager->getTopLayout();
      QWidget *w = new QWidget( mTabWidget );
      w->setLayout( layout );
      QString title = mGroups[ it.key() ];
      if( title.isEmpty() )
        title = mGroups.values().first();
      if( mTabWidget ) {
        mTabWidget->addTab( w, title );
      }
    } else if( mTabWidget ) {
      int index = mTabWidget->indexOf( layout->parentWidget() );
      QString title = mTabWidget->tabText( index );
      if( !mGroups[ it.key() ].isEmpty() ) {
        if( !title.isEmpty() )
          title += QString(" && ");
        title += mGroups[ it.key() ];
      }
      if( mTabWidget )
        mTabWidget->setTabText( index, title );
    }

    foreach( Layout::Element *e, list ) {
      mManager->addElementRow( layout, e, width, height );
    }

    foreach( GuiElement *guiElem, it.value().elements() ) {
      mGuiElements.append( guiElem );
    }
  }

  kDebug() << "FormGui::parseElement() done" << endl;
}


void FormGui::parseAttributeElements( GuiElement *parent, QDomElement &e )
{
  QDomNode n;
  for( n = e.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if( e.tagName() != "attributes" )
      continue;

    QDomNode n2;
    for( n2 = e.firstChild(); !n2.isNull(); n2 = n2.nextSibling() ) {
      QDomElement e2 = n2.toElement();
      QString tag = e2.tagName();

      XFormsCommon c = XFormsCommon::parseElement( e2 );
      GuiElement::Properties *properties = new GuiElement::Properties;
      GuiElement::parseProperties( e2, properties );
      GuiElement *guiElement = 0;

      kDebug() << "Got an attribute element: " << tag << " label: " << c.label() << endl;

      if ( tag == "list" ) {
        guiElement = new KXForms::List( mManager, c.label(), this, properties );
        guiElement->setRef( ref() );
      } else if ( tag == "xf:input" ) {
        Input *input = new Input( mManager, c.label(), this, properties );
        connect( input, SIGNAL( returnPressed() ), SIGNAL( editingFinished() ) );
        guiElement = input;
        guiElement->setRef( e2.attribute( "ref" ) );
      }else if ( tag == "xf:select1" ) {
        guiElement = new Select1( mManager, c.label(), this, properties );
        guiElement->setRef( e2.attribute( "ref" ) );
      } else {
        kWarning() << "  Unsupported element: " << tag << endl;
        delete properties;
      }

      if( guiElement ) {
        parent->addAttributeElement( guiElement );
      }
    }
  }
}

QDomElement FormGui::findContextElement( const QDomDocument &doc )
{
  return ref().apply( doc );
}

void FormGui::loadData( const QDomDocument &doc )
{
  kDebug() << "FormGui::loadData() " << ref().toString() << endl;

  QDomElement contextElement = findContextElement( doc );

  GuiElement::List::ConstIterator itGui;
  for( itGui = mGuiElements.begin(); itGui != mGuiElements.end(); ++itGui ) {
    (*itGui)->loadData( contextElement );
  }
}

void FormGui::saveData()
{
  GuiElement::List::ConstIterator itGui;
  QString invalidElements;

  bool valid = true;
  for( itGui = mGuiElements.begin(); itGui != mGuiElements.end(); ++itGui ) {
    if( !(*itGui)->isValid() ) {
      valid = false;
      invalidElements += QString( " - %1 (constraint: %2)\n" ).arg( (*itGui)->ref().path(), (*itGui)->properties()->constraint  );
    }
  }
  if( valid ) {
    for( itGui = mGuiElements.begin(); itGui != mGuiElements.end(); ++itGui ) {
      (*itGui)->save();
    }
  } else {
    kDebug() << k_funcinfo << "Not all elements were valid" << endl;
    KMessageBox::error( this, i18n( "There were elements not matching their constraint:\n" ) + invalidElements );
  }
}

bool FormGui::hasGroups( const QDomElement &e )
{
  return !e.firstChildElement( "groups" ).isNull();
}

void FormGui::setupGroups( QLayout *l, const QDomElement &element )
{
  mTabWidget = new QTabWidget( this );
  l->addWidget( mTabWidget );
  QDomElement e = element.firstChildElement( "groups" ).firstChild().toElement();
  while( !e.isNull() ) {
    if( e.tagName() == "group" ) {
      mGroups[ e.attribute( "id" ) ] = e.text();
    }
    e = e.nextSibling().toElement();
  }
}

GuiElement::List FormGui::elements()
{
  if( !mTabWidget )
    return mGuiElements;
  else {
    GuiElement::List list;
    int currentIndex = mTabWidget->currentIndex();
    foreach( GuiElement *e, mGuiElements ) {
      QWidget *it = e->widget();
      while( it->parentWidget() ) {
        if( mTabWidget->indexOf( it ) >= 0 &&
             mTabWidget->indexOf( it ) == currentIndex ) {
          list.append( e );
          break;
        }
        it = it->parentWidget();
      }
    }
    return list;
  }
}

#include "formgui.moc"
