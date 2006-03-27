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

#include <kdebug.h>
#include <klocale.h>

#include <QDomDocument>

using namespace KXForms;

Manager::Manager()
  : mDataLoaded( false )
{
  mGuiHandler = new GuiHandler( this );
}

Manager::~Manager()
{
  clearForms();

  delete mGuiHandler;
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
    } else {
      kError() << "Expected 'form' element. Got '" << e.tagName() << "'" << endl;
      return false;
    }
  }

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
    kError() << "No Forms" << endl;
    return KResultError( i18n("No Forms loaded.") );
  }

  QString errorMsg;
  int errorLine;
  int errorCol;
  if ( !mData.setContent( xml, &errorMsg, &errorLine, &errorCol ) ) {
    QString msg = i18n("%1 (line %2, column %3)").arg( errorMsg,
      QString::number( errorLine ), QString::number( errorCol ) );
    return KResultError( KResult::ParseError, msg );
  }

  FormGui::List::ConstIterator it;
  for( it = mGuis.begin(); it != mGuis.end(); ++it ) {
    (*it)->loadData( mData );
  }

  mDataLoaded = true;

  return KResultOk();
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
  FormGui *gui = mGuiHandler->createRootGui( parent );
  mGuis.append( gui );
  return gui;
}

void Manager::createGui( const Reference &ref, QWidget *parent )
{
  FormGui *gui = mGuiHandler->createGui( ref, parent );
  if ( gui ) mGuis.append( gui );
}

void Manager::destroyGui( FormGui *gui )
{
  mGuis.remove( gui );
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