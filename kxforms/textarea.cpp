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

#include "textarea.h"

#include "manager.h"

#include <kdebug.h>

#include <QLabel>
#include <QTextEdit>

using namespace KXForms;

TextArea::TextArea( Manager *m, const QString &label, QWidget *parent, Properties *p )
  : GuiElement( parent, m, p )
{
  mManager->dispatcher()->registerElement( this );
  mLabel = new QLabel( label, mParent );
  mEdit = new QTextEdit( mParent );
  mWidget = mEdit;

  mEdit->setMinimumHeight( 40 );

  applyProperties();

  connect( mEdit, SIGNAL(textChanged()), SLOT(emitValueChanged()) );
}

void TextArea::loadData()
{
  QDomElement element = ref().applyElement( context() );

  QString txt;
  QTextStream ts( &txt, QIODevice::WriteOnly );

  QDomNode n;
  for( n = element.firstChild(); ! n.isNull(); n = n.nextSibling() ) {
    n.save( ts, 0 );
  }

  mEdit->setPlainText( txt );
}

void TextArea::saveData()
{
  kDebug() << "TextArea::saveData()" << endl;

  QDomElement e = ref().applyElement( context() );

  if ( e.isNull() ) {
    e = createElement( ref() );
  }

  QString tag = e.tagName();

  while( !e.firstChild().isNull() ) {
    e.removeChild( e.firstChild() );
  }

  QString xml = '<' + tag + '>' + mEdit->toPlainText() + "</" + tag + '>';

  kDebug() << "XML: " << xml << endl;

  QDomDocument doc;
  doc.setContent( xml );

  kDebug() << "DOC: " << doc.toString( 2 ) << "END OF DOC" << endl;

  QDomElement docElement = doc.documentElement();
  QDomNode n;
  for( n = docElement.firstChild(); !n.isNull(); n = n.nextSibling() ) {
    kDebug() << "TAG: " << n.toElement().tagName() << endl;
    e.appendChild( n.cloneNode() );
  }
}

void TextArea::applyProperties()
{
  if( mEdit ) {
    mEdit->setReadOnly( mProperties->readonly );
  }
}

void TextArea::emitValueChanged()
{
  emit valueChanged( ref().toString(), mEdit->toPlainText() );
}


bool TextArea::isValid() const
{
  if( mProperties->constraint.isEmpty() )
    return true;

  QRegExp regExp( mProperties->constraint );  

  if( (mEdit->toPlainText().indexOf( regExp ) >= 0 ) ) {
    return true;
  } else {
    return false;
  }
}
#include "textarea.moc"
