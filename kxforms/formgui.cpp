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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "formgui.h"

#include "xformscommon.h"
#include "list.h"
#include "input.h"
#include "textarea.h"
#include "select1.h"
#include "prefs.h"

#include <kdebug.h>
#include <klocale.h>
#include <kdialog.h>

#include <QLabel>
#include <QVBoxLayout>

using namespace KXForms;

FormGui::FormGui( const QString &label, Manager *m, QWidget *parent )
  : QWidget( parent ), mManager( m )
{
  kDebug() << "FormGui()" << endl;

  mTopLayout = new QVBoxLayout( this );


  mLabel = new QLabel( label, this );
  QFont f = mLabel->font();
  f.setBold( true );
  mLabel->setFont( f );
  mTopLayout->addWidget( mLabel );
  mLabel->hide();

  mRefLabel = new QLabel( this );
  f = mRefLabel->font();
  f.setPointSize( f.pointSize() - 2 );
  mRefLabel->setFont( f );
  mTopLayout->addWidget( mRefLabel );

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
  mRefLabel->setText( i18n("Reference: %1").arg( ref.toString() ) );
}

Reference FormGui::ref() const
{
  return mRef;
}

QString FormGui::label() const
{
  return mLabel->text();
}

void FormGui::parseElement( const QDomElement &element )
{
  kDebug() << "FormGui::parseElement()" << endl;

  bool hasList = false;

  QDomNode n;
  for ( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    QString tag = e.tagName();
    kDebug() << "  Found element '" << tag << "'" << endl;
    XFormsCommon c = XFormsCommon::parseElement( e );
    GuiElement *guiElement = 0;
    if ( tag == "xf:label" ) {
      mLabel->setText( e.text() );
      mLabel->show();
    } else if ( tag == "list" ) {
      guiElement = new KXForms::List( mManager, c.label(), this );
      guiElement->setRef( ref() );
      hasList = true;
    } else if ( tag == "xf:input" ) {
      guiElement = new Input( mManager, c.label(), this );
      guiElement->setRef( e.attribute( "ref" ) );
    } else if ( tag == "xf:textarea" ) {
      guiElement = new TextArea( mManager, c.label(), this );
      guiElement->setRef( e.attribute( "ref" ) );
    } else if ( tag == "xf:select1" ) {
      guiElement = new Select1( mManager, c.label(), this );
      guiElement->setRef( e.attribute( "ref" ) );
    } else {
      kWarning() << "  Unsupported element" << endl;
    }
    
    if ( guiElement ) {
      guiElement->parseElement( e );
      mTopLayout->addWidget( guiElement );
      mGuiElements.append( guiElement );
    }
  }

  if ( !hasList ) mTopLayout->addStretch( 0 );

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
  for( itGui = mGuiElements.begin(); itGui != mGuiElements.end(); ++itGui ) {
    (*itGui)->saveData();
  }
}
