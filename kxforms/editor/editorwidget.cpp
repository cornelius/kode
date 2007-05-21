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

#include <QTimer>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>

using namespace KXForms;

EditorWidget::EditorWidget( Editor *e, QWidget *parent )
  : QWidget( parent ), mEditor( e ), mHoveredElement( 0 )
{
  setMouseTracking( true );
  setGeometry( parent->geometry() );
}


void EditorWidget::mouseMoveEvent( QMouseEvent *event )
{
  kDebug() << k_funcinfo << endl;
  GuiElement *newElement = 0;
  QPoint pos = event->pos();
  foreach( GuiElement *e, mGuiElements ) {
    kDebug() << pos << " : " << e->widget()->geometry() << endl;
    if( e->widget()->geometry().contains( pos ) ) {
      kDebug() << k_funcinfo << "Contains!!!!" << endl;
      newElement = e;
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
    drawInterface( &p, mHoveredElement );
  }
}

void EditorWidget::drawInterface( QPainter *p, GuiElement *w )
{
  KActionMenu *menu = new KActionMenu( this );

//   menu->menu()->addTitle( i18n("Edit %1", w->element()->ref().toString() ) );

  QPen pen;
  pen.setColor( QColor(255,255,255,255) );
  pen.setWidth( 3 );
  p->setPen( pen );
  p->drawRect( w->widget()->geometry() );

  QBrush b( QColor(0,0,0,50) );
  p->fillRect( w->widget()->geometry(), b );


  QPoint point( w->widget()->geometry().x()+20, w->widget()->geometry().y()+20 );

  if( w->actionTypes() & Editor::CommonActions ) {
    p->drawText( point, i18n("Change Label") );
    point.setY( point.y() + 20 );
    KAction *titleAction = new KAction( i18n("Change Label"), menu );
    titleAction->setData( "edit_label" );
    QObject::connect( titleAction, SIGNAL(triggered(bool)), w, SLOT( actionTriggered() ) );
    menu->addAction( titleAction );


    KAction *positionAction = new KAction( i18n("Change Position"), menu );
    positionAction->setData( "edit_position" );
    QObject::connect( positionAction, SIGNAL(triggered(bool)), w, SLOT( actionTriggered() ) );
    menu->addAction( positionAction );


    KAction *layoutStyleAction = new KAction( i18n("Change Layout Style"), menu );
    layoutStyleAction->setData( "edit_layoutstyle" );
    QObject::connect( layoutStyleAction, SIGNAL(triggered(bool)), w, SLOT( actionTriggered() ) );
    menu->addAction( layoutStyleAction );
  }

  if( w->actionTypes() & Editor::AppearanceActions ) {
    p->drawText( point, i18n("Change Appearance") );
    point.setY( point.y() + 20 );
    KAction *appearanceAction = new KAction( i18n("Change Appearance"), menu );
    appearanceAction->setData( "edit_appearance" );
    QObject::connect( appearanceAction, SIGNAL(triggered(bool)), w, SLOT( actionTriggered() ) );
    menu->addAction( appearanceAction );
  }

  if( w->actionTypes() & Editor::ListActions ) {
    p->drawText( point, i18n("Change List Properties") );
    point.setY( point.y() + 20 );
    KAction *listAction = new KAction( i18n("Change List Properties"), menu );
    listAction->setData( "edit_list" );
    QObject::connect( listAction, SIGNAL(triggered(bool)), w, SLOT( actionTriggered() ) );
    menu->addAction( listAction );
  }
}

void EditorWidget::actionTriggered()
{
  QAction *action = dynamic_cast<QAction *>( sender() );
  if( !action )
    return;

  mEditor->performAction( action->data().toString(), this );
}

#include "editorwidget.moc"
