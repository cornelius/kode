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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "manager.h"
#include "guielement.h"

#include <kdebug.h>
#include <klocale.h>

#include <QDomDocument>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QGridLayout>

using namespace KXForms;

Manager::Manager()
  : mGuiHandler( 0 ), mDefaultProperties( new GuiElement::Properties() ), mDataLoaded( false ), 
    mDispatcher( new Dispatcher() ), mEditor( new Editor() )
{
}

Manager::~Manager()
{
  clearForms();

  delete mGuiHandler;
  delete mDispatcher;
  delete mEditor;
}

void Manager::setGuiHandler( GuiHandler *handler )
{
  mGuiHandler = handler;
}

bool Manager::parseForms( const QString &xml )
{
  kDebug() << "Manager::parseForms()" << endl;

  clearForms();

  QDomDocument doc;
  doc.setContent( xml );

  QDomElement docElement = doc.documentElement();

  if ( docElement.tagName() != "kxforms" ) {
    kError() << "Top element is '" << docElement.tagName() <<
      "'. Expected 'kxforms'." << endl;
    return false;
  }

  QDomNode n;
  for( n = docElement.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    QDomElement e = n.toElement();
    if ( e.tagName() =="form" ) {
      Form *form = parseForm( e );
      if ( !form ) return false;
      mForms.append( form );
    } else if( e.tagName() == "defaults" ) {
      kDebug() << "Found a defaults-element" << endl;
      GuiElement::parseProperties( docElement, mDefaultProperties );
    } else {
      kError() << "Expected 'form' or 'defaults' element. Got '" << e.tagName() << "'" << endl;
      return false;
    }
  }

  if ( hasData() ) loadData();

  return true;
}

Form *Manager::parseForm( const QDomElement &element )
{
  QString ref = element.attribute( "ref" );
  kDebug() << "Manager::parseForm() ref '" << ref << "'" << endl;
  if ( ref.isEmpty() ) return 0;

  Form *form = new Form( this );
  form->setElement( element );
  form->setRef( ref );

  return form;
}

Form *Manager::rootForm()
{
  if ( mForms.isEmpty() ) {
    kError() << "No forms." << endl;
    return 0;
  }

  return mForms.first();
}

Form *Manager::form( const QString &ref )
{
  Form::List::ConstIterator it;
  for( it = mForms.begin(); it != mForms.end(); ++it ) {
    if ( (*it)->ref() == ref ) return *it;
  }
  return 0;
}

KResult Manager::loadData( const QString &xml )
{
  kDebug() << "Manager::loadData()" << endl;

  if ( mForms.isEmpty() ) {
    kDebug() << "No Forms" << endl;
  }

  QXmlInputSource source;
  source.setData( xml );
  QXmlSimpleReader reader;
  reader.setFeature( "http://xml.org/sax/features/namespaces", true );
  reader.setFeature( "http://xml.org/sax/features/namespace-prefixes", false );

  QString errorMsg;
  int errorLine;
  int errorCol;
  if ( !mData.setContent( &source, &reader, &errorMsg, &errorLine, &errorCol ) ) {
    QString msg = i18n("%1 (line %2, column %3)", errorMsg, errorLine, errorCol );
    return KResultError( KResult::ParseError, msg );
  }

  QString schemaLocationAttribute = mData.documentElement().attributeNS(
    "http://www.w3.org/2001/XMLSchema-instance", "schemaLocation" );
  QStringList schemaLocation =
    schemaLocationAttribute.simplified().split( " " );

  if ( schemaLocation.count() > 1 ) {
    mSchemaUri = schemaLocation[ 1 ];
  } else {
    mSchemaUri.clear();
  }

#if 1
  if ( !mData.setContent( xml, &errorMsg, &errorLine, &errorCol ) ) {
    QString msg = i18n("%1 (line %2, column %3)", errorMsg, errorLine, errorCol );
    return KResultError( KResult::ParseError, msg );
  }
#endif

  loadData();

  mDataLoaded = true;

  return KResultOk();
}

void Manager::loadData()
{
  FormGui::List::ConstIterator it;
  for( it = mGuis.begin(); it != mGuis.end(); ++it ) {
    (*it)->loadData( mData );
  }
}

KResult Manager::saveData( QString &xml )
{
  kDebug() << "Manager::saveData()" << endl;
  
  if ( !mDataLoaded ) return KResultError( i18n("No data loaded.") );

  FormGui::List::ConstIterator it;
  for( it = mGuis.begin(); it != mGuis.end(); ++it ) {
    (*it)->saveData();
  }
  
  xml = mData.toString( 2 );
  xml.append( "\n" );
  
  return KResultOk();
}

void Manager::clearForms()
{
  Form::List::ConstIterator it;
  for( it = mForms.begin(); it != mForms.end(); ++it ) {
    delete *it;
  }
  mForms.clear();
}

QWidget *Manager::createRootGui( QWidget *parent )
{
  if ( !mGuiHandler ) {
    kError() << "Manager::createRootGui(): No GuiHandler" << endl;
    return 0;
  }

  return mGuiHandler->createRootGui( parent );
}

void Manager::createGui( const Reference &ref, GuiElement *parent )
{
  if ( !mGuiHandler ) {
    kError() << "Manager::createGui(): No GuiHandler" << endl;
    return;
  }

  mGuiHandler->createGui( ref, parent->widget() );
}

void Manager::registerGui( FormGui *gui )
{
  mGuis.append( gui );
}

void Manager::unregisterGui( FormGui *gui )
{
  mGuis.removeAll( gui );
}

bool Manager::hasData() const
{
  return mDataLoaded;
}

QDomDocument Manager::document() const
{
  return mData;
}

void Manager::loadData( FormGui *gui )
{
  gui->loadData( mData );
}

QDomElement Manager::applyReference( const Reference &ref )
{
  return ref.apply( mData );
}

QString Manager::schemaUri() const
{
  return mSchemaUri;
}

QLayout *Manager::getTopLayout() const
{
  return mGuiHandler->getTopLayout();
}

void Manager::addWidget( QLayout *l, QWidget *w ) const
{
  mGuiHandler->addWidget( l, w );
}

void Manager::addElementRow( QLayout *l, Layout::Element *e, int totalWidth, int totalHeight ) const
{
  QGridLayout *gl = dynamic_cast<QGridLayout *>( l );
  int row = gl ? gl->rowCount() : 0;

  addElementRowElement( l, e, totalWidth, totalHeight, 0, row );
}

void Manager::addElementRowElement( QLayout *l, Layout::Element *e, int totalWidth, int totalHeight, int xPosition, int yPosition ) const
{
  int width = e->rightElement() ? 1 : totalWidth - xPosition;
  int height = e->belowElement() ? 1 : totalHeight - e->element()->attributeElements().size();

//   kDebug() << e->element()->ref().toString() << ": " << xPosition << " " << yPosition << " " << width << " " << totalWidth << " " << totalHeight << endl;
  mGuiHandler->addElement( l, e->element()->labelWidget(), e->element()->widget(), xPosition,yPosition, width, height, e->element()->properties() );

  foreach( GuiElement *a, e->element()->attributeElements() ) {
    ++yPosition;
    mGuiHandler->addElement( l, a->labelWidget(), a->widget(), xPosition, yPosition, width, 1, a->properties(), true );
  }

  if( e->rightElement() )
    addElementRowElement( l, e->rightElement(), totalWidth, totalHeight, xPosition+1, yPosition );
  if( e->belowElement() )
    addElementRowElement( l, e->belowElement(), totalWidth, totalHeight, xPosition, yPosition+1 );
}
