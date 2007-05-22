/*
    This file is part of KXForms.

    Copyright (c) 2007 Andre Duffeck <aduffeck@suse.de>

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

#include "editorwidget.h"
#include "editor.h"

#include <kdebug.h>
#include <kpixmapeffect.h>
#include <kactionmenu.h>
#include <kmenu.h>
#include <klocale.h>
#include <kiconloader.h>

#include <QTimer>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>

using namespace KXForms;

EditorWidget::EditorWidget( Editor *e, QWidget *parent )
  : QWidget( parent ), mEditor( e ), mHoveredElement( 0 )
{
  setMouseTracking( true );
  setGeometry( parent->geometry() );


  mEditButton = new QPushButton( this );
  mEditButton->setIcon( KIconLoader::global()->loadIcon( "edit", K3Icon::NoGroup, 32 ) );
  mEditButton->hide();
  connect( mEditButton, SIGNAL(clicked()), SLOT(showActionMenu()) );
}

void EditorWidget::setGuiElements( const GuiElement::List &list )
{
//   mGuiElements = list;
  foreach( GuiElement *e, list ) {
    QRect r = e->widget()->geometry() | e->labelWidget()->geometry();
    QPoint widgetPos = e->widget()->mapToGlobal(QPoint(0,0)) - mapToGlobal(QPoint(0,0));
    QPoint labelWidgetPos = e->labelWidget()->mapToGlobal(QPoint(0,0)) - mapToGlobal(QPoint(0,0));
    r.moveTop( qMin( widgetPos.y(), labelWidgetPos.y() ) );
    r.moveLeft( qMin( widgetPos.x(), labelWidgetPos.x() ) );

    mElementMap[e] = r;
  }
}

void EditorWidget::mouseMoveEvent( QMouseEvent *event )
{
  kDebug() << k_funcinfo << endl;
  GuiElement *newElement = 0;
  QPoint pos = event->pos();
  foreach( QRect r, mElementMap.values() ) {
    if( r.contains( pos ) ) {
      newElement = mElementMap.key( r );
      break;
    }
  }
  if( mHoveredElement != newElement )
    update();

  mHoveredElement = newElement;
}

void EditorWidget::paintEvent( QPaintEvent *event )
{
  kDebug() << k_funcinfo << endl;
  QPainter p( this );
  QBrush b( QColor(0,0,0,25) );
  p.fillRect( event->rect(), b );

//   QLabel::paintEvent( event );
  if( mHoveredElement ) {
    kDebug() << k_funcinfo << endl;
    drawInterface( &p, mElementMap[mHoveredElement], mHoveredElement );
  } else {
    mEditButton->hide();
  }
}

void EditorWidget::drawInterface( QPainter *p, const QRect &r, GuiElement *w )
{
  //   menu->menu()->addTitle( i18n("Edit %1", w->element()->ref().toString() ) );

  QPen pen;
  pen.setColor( QColor(255,255,255,255) );
  pen.setWidth( 3 );
  p->setPen( pen );
  p->drawRect( r );

  QBrush b( QColor(0,0,0,100) );
  p->fillRect( r, b );


  QPoint point( r.x()+20, r.y() );

  p->save();
  QFont fnt;
  fnt.setPointSize( 14 );
  fnt.setBold( true );
  p->setFont( fnt );
  p->drawText( point + QPoint(0,QFontMetrics( fnt ).height() ), w->ref().toString() );
  point.setX( point.x() + 10 + QFontMetrics( fnt ).width( w->ref().toString() ));
  p->restore();

  mEditButton->move( point );
  point.setY( point.y() + 20 );
  mEditButton->show();
  

}

void EditorWidget::showActionMenu()
{
  KActionMenu *menu = mEditor->actionMenu( mHoveredElement );
  menu->menu()->popup( mapToGlobal( mEditButton->pos() ) );
}


#include "editorwidget.moc"
