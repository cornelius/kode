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

#include <kdebug.h>
#include <klocale.h>
#include <kdialog.h>
#include <kmessagebox.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QTabWidget>

using namespace KXForms;

FormGui::FormGui( const QString &label, Manager *m, QWidget *parent )
  : QWidget( parent ), mManager( m ), mLabelHidden( false )
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

void FormGui::parseElement( const QDomElement &element, QLayout *l, const QString overrideLabel )
{
  kDebug() << "FormGui::parseElement()" << endl;

  bool hasList = false;
  QLayout *layout = l ? l : mTopLayout;

  if( hasPages( element ) )
    setupPages( element, layout );

  QDomNode n;
  for ( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    QString tag = e.tagName();
    kDebug() << "  Found element '" << tag << "'" << endl;

    XFormsCommon c = XFormsCommon::parseElement( e );
    if( !overrideLabel.isEmpty() )
      c.setLabel( overrideLabel );

    GuiElement *guiElement = 0;
    GuiElement::Properties *properties = new GuiElement::Properties;
    GuiElement::parseProperties( e, properties );
    if( hasPages( element ) ) {
      if( !properties->page.isEmpty() && mTabs[ properties->page ] )
        layout = mTabs[ properties->page ]->layout();
      else
        layout = mTabWidget->widget( 0 )->layout();
    }

    if ( tag == "xf:label" ) {
      mLabel->setText( e.text() );
      if ( !mLabelHidden ) mLabel->show();
    } else if ( tag == "list" ) {
      guiElement = new KXForms::List( mManager, c.label(), this, properties );
      guiElement->setRef( ref() );
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
    } else if ( tag == "kxf:section" ) {
      if( e.attribute( "visible" ) != "false" ) {
        guiElement = new Section( mManager, c.label(), this, properties );
        guiElement->setRef( e.attribute( "ref" ) );
        parseElement( e, static_cast<Section *>( guiElement )->layout(), e.attribute( "overrideLabel" ) );
      } else {
        parseElement( e, layout, e.attribute( "overrideLabel" ) );
      }
    } else {
      kWarning() << "  Unsupported element: " << tag << endl;
      delete properties;
    }
    
    if ( guiElement ) {
      guiElement->parseElement( e );
      mManager->addElement( layout, guiElement );
      mGuiElements.append( guiElement );
    }
  }

  kDebug() << "FormGui::parseElement() done" << endl;
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
      (*itGui)->saveData();
    }
  } else {
    kDebug() << k_funcinfo << "Not all elements were valid" << endl;
    KMessageBox::error( this, i18n( "There were elements not matching their constraint:\n" ) + invalidElements );
  }
}

bool FormGui::hasPages( const QDomElement &e )
{
  return !e.firstChildElement( "pages" ).isNull();
}

void FormGui::setupPages( const QDomElement &element, QLayout *l )
{
  mTabWidget = new QTabWidget( this );
  l->addWidget( mTabWidget );
  QDomElement e = element.firstChildElement( "pages" ).firstChild().toElement();
  while( !e.isNull() ) {
    if( e.tagName() == "page" ) {
      QWidget *w = new QWidget( mTabWidget );
      QLayout *l = mManager->getTopLayout();
      w->setLayout( l );
      mTabWidget->addTab( w, e.text() );
      mTabs[ e.attribute( "id" ) ] = w;
    }
    e = e.nextSibling().toElement();
  }
}

#include "formgui.moc"
