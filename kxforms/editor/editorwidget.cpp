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

#include <QTimer>

using namespace KXForms;

EditorWidget::EditorWidget( GuiElement *guiElem, Editor *e, QWidget *parent )
  : QLabel( parent ), mEditor( e ), mElement( guiElem )
{
  mMenuTimer = new QTimer( this );
  connect( mMenuTimer, SIGNAL(timeout()), SLOT(showEditMenu()) );
}

void EditorWidget::setBuddyWidget( QWidget *w )
{
  mBuddyWidget = w;
}

void EditorWidget::takeSnapshot()
{
  QPixmap p = QPixmap::grabWidget( mBuddyWidget );
  setPixmap( KPixmapEffect::fade( p, 0.2, Qt::black ) );
}

void EditorWidget::enterEvent ( QEvent * event )
{
  Q_UNUSED( event );
  if( !mEditor->inEdit() ) {
    mMenuTimer->start( 1000 );
  }
  else {
    
  }
}

void EditorWidget::leaveEvent ( QEvent * event )
{
  Q_UNUSED( event );
  mMenuTimer->stop();
}

void EditorWidget::showEditMenu()
{
  KActionMenu *menu = mEditor->actionMenu( this );

  if( !menu )
    return;

  connect( menu->menu(), SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );
  menu->menu()->popup( mapToGlobal( pos() ) );
}

void EditorWidget::actionTriggered()
{
  QAction *action = dynamic_cast<QAction *>( sender() );
  if( !action )
    return;

  mEditor->performAction( action->data().toString(), this );
}

#include "editorwidget.moc"
