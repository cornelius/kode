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

#include "formgui.h"

#include "xformscommon.h"
#include "list.h"
#include "input.h"
#include "textarea.h"
#include "select1.h"

#include <kdebug.h>
#include <klocale.h>
#include <kdialog.h>

#include <qlayout.h>
#include <qlabel.h>
//Added by qt3to4:
#include <QVBoxLayout>

using namespace KXForms;

FormGui::FormGui( Manager *m, QWidget *parent )
  : QWidget( parent ), mManager( m )
{
  kDebug() << "FormGui()" << endl;

  mTopLayout = new QVBoxLayout( this );
  mTopLayout->setSpacing( KDialog::spacingHint() );

  mRefLabel = new QLabel( this );
  mTopLayout->addWidget( mRefLabel );

  setRefLabel( "[undefined reference]" );
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

void FormGui::parseElement( const QDomElement &element )
{
  kDebug() << "FormGui::parseElement()" << endl;

  QDomNode n;
  for ( n = element.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    QString tag = e.tagName();
    kDebug() << "  Found element '" << tag << "'" << endl;
    XFormsCommon c = XFormsCommon::parseElement( e );
    GuiElement *guiElement = 0;
    if ( tag == "list" ) {
      guiElement = new KXForms::List( mManager, c.label(), this );
      guiElement->setRef( ref() );
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

  kDebug() << "FormGui::parseElement() done" << endl;
}

QDomElement FormGui::findContextElement( const QDomDocument &doc )
{
  QDomElement contextElement;

  Reference::Segment::List segments = ref().segments();
  Reference::Segment::List::ConstIterator it;
  for( it = segments.begin(); it != segments.end(); ++it ) {
    Reference::Segment segment = *it;
    if ( contextElement.isNull() ) {
      kDebug() << "ROOT" << endl;
      if ( doc.documentElement().tagName() == segment.name() ) {
        contextElement = doc.documentElement();
        continue;
      } else {
        kError() << "Document element '" << doc.documentElement().tagName() <<
           "' isn't '" << segment.name() << "'" << endl;
        return QDomElement();
      }
    }

    QMap<QString, int> counts;
    QDomNode n;
    for( n = contextElement.firstChild(); !n.isNull(); n = n.nextSibling() ) {
      QDomElement e = n.toElement();
      int count = 1;
      QMap<QString, int>::ConstIterator itCount = counts.find( e.tagName() );
      if ( itCount != counts.end() ) count = itCount.data();
      if ( e.tagName() == segment.name() && count == segment.count() ) {
        contextElement = e;
        break;
      }
      counts.insert( e.tagName(), ++count );
    }
    if ( n.isNull() ) {
      kError() << "FormGui::loadData(): Unable to find element '" <<
        segment.toString() << "'" << endl;
      return QDomElement();
    }
  }
  
  return contextElement;
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
