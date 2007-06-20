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
#include "globalsettingsdlg.h"
#include "positionaction.h"
#include "../manager.h"
#include "../formgui.h"

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
#include <QEventLoop>
#include <QWhatsThis>
#include <QVBoxLayout>
#include <QApplication>

using namespace KXForms;

EditorWidget::EditorWidget( Editor *e, FormGui *parent )
  : QWidget( parent ), mEditor( e ), mGui( parent ), mHoveredElement( 0 ), mActiveElement( 0 ),
    mEventLoop( new QEventLoop( this ) ), mInSelection( false ), mInEdit( false ),
    mDraggedElement( 0 )
{
  setMouseTracking( true );
  setGeometry( parent->geometry() );
  init();
}

void EditorWidget::init()
{
  mInterfaceWidget = new QWidget( this );
  QVBoxLayout *vbl = new QVBoxLayout( mInterfaceWidget );
  vbl->setMargin( 0 );

  mShowHintsButton = new QPushButton( mInterfaceWidget );
  mShowHintsButton->setIcon( KIconLoader::global()->loadIcon( "list", K3Icon::NoGroup, 32 ) );
  mShowHintsButton->setToolTip( i18n("Show hints") );
  connect( mShowHintsButton, SIGNAL(clicked()), SLOT(showHints()) );
  vbl->addWidget( mShowHintsButton );


  mSaveHintsButton = new QPushButton( mInterfaceWidget );
  mSaveHintsButton->setIcon( KIconLoader::global()->loadIcon( "document-save", K3Icon::NoGroup, 32 ) );
  mSaveHintsButton->setToolTip( i18n("Save hints") );
  connect( mSaveHintsButton, SIGNAL(clicked()), mEditor, SLOT(saveHints()) );
  vbl->addWidget( mSaveHintsButton );

  mSaveHintsAsButton = new QPushButton( mInterfaceWidget );
  mSaveHintsAsButton->setIcon( KIconLoader::global()->loadIcon( "document-save-as", K3Icon::NoGroup, 32 ) );
  mSaveHintsAsButton->setToolTip( i18n("Save hints as...") );
  connect( mSaveHintsAsButton, SIGNAL(clicked()), mEditor, SLOT(saveHintsAs()) );
  vbl->addWidget( mSaveHintsAsButton );

  mEditDefaultsButton = new QPushButton( mInterfaceWidget );
  mEditDefaultsButton->setIcon( KIconLoader::global()->loadIcon( "edit", K3Icon::NoGroup, 32 ) );
  mEditDefaultsButton->setToolTip( i18n("Edit global settings") );
  connect( mEditDefaultsButton, SIGNAL(clicked()), SLOT(editDefaults()) );
  vbl->addWidget( mEditDefaultsButton );

  mExitButton = new QPushButton( this );
  mExitButton->setIcon( KIconLoader::global()->loadIcon( "dialog-cancel", K3Icon::NoGroup, 32 ) );
  mExitButton->setToolTip( i18n("Quit edit mode") );
  connect( mExitButton, SIGNAL(clicked()), mEditor, SLOT(toggleEditMode()) );
  vbl->addWidget( mExitButton );

  mEditButton = new QPushButton( this );
  mEditButton->setIcon( KIconLoader::global()->loadIcon( "edit", K3Icon::NoGroup, 32 ) );
  mEditButton->hide();
  mEditButton->setToolTip( i18n("Edit element") );
  connect( mEditButton, SIGNAL(clicked()), SLOT(showActionMenu()) );
}

void EditorWidget::setGuiElements( const GuiElement::List &list )
{
  foreach( GuiElement *e, list ) {
    QRect r = e->widget()->geometry();
    if( e->labelWidget() )
      r |= e->labelWidget()->geometry();
    QPoint widgetPos = e->widget()->mapToGlobal(QPoint(0,0)) - mapToGlobal(QPoint(0,0));
    QPoint labelWidgetPos = e->labelWidget() ? e->labelWidget()->mapToGlobal(QPoint(0,0)) - mapToGlobal(QPoint(0,0)) : widgetPos;
    r.moveTop( qMin( widgetPos.y(), labelWidgetPos.y() ) );
    r.moveLeft( qMin( widgetPos.x(), labelWidgetPos.x() ) );

    mElementMap[e] = r;
    mGroupMap[e->properties()->group] |= r;
  }
}

void EditorWidget::setInEdit( bool b )
{
  mInEdit = b;
  if( !b )
    mActiveElement = 0;
}

void EditorWidget::mouseMoveEvent( QMouseEvent *event )
{
//   kDebug() << k_funcinfo << endl;
  GuiElement *newElement = 0;
  QPoint pos = event->pos();
  QRect r;


  if( mInDrag && !mDraggedElement && ( mDragPoint - pos ).manhattanLength() > QApplication::startDragDistance() ) {
    foreach( QRect dragR, mElementMap.values() ) {
      if( dragR.contains( mDragPoint ) ) {
        if( !mDraggedElement || 
            mElementMap[ mDraggedElement ].width() > dragR.width() ||
            mElementMap[ mDraggedElement ].height() > dragR.height() ) {
          mDraggedElement = mElementMap.key( dragR );
        }
      }
    }
    QPixmap cursorPixmap = QPixmap::grabWidget( mGui, mElementMap[ mDraggedElement ] ).scaled( 300, 300, Qt::KeepAspectRatio );
    QApplication::setOverrideCursor( QCursor( cursorPixmap ) );
    mActiveElement = mDraggedElement;
    mInEdit = true;
    mInSelection = true;
    mSelectionMode = Editor::SelectSameGroupOnly;
  }

  foreach( r, mElementMap.values() ) {
    if( r.contains( pos ) ) {
      if( !newElement || 
          mElementMap[ newElement ].width() > r.width() ||
          mElementMap[ newElement ].height() > r.height() ) {
        newElement = mElementMap.key( r );
      }
    }
  }
  if( mHoveredElement != newElement ) {
    mHoveredElement = newElement;
    update();
  }
}

void EditorWidget::mousePressEvent( QMouseEvent *event )
{
//   kDebug() << k_funcinfo << endl;
  mDragPoint = event->pos();
  mInDrag = true;
}

void EditorWidget::mouseReleaseEvent( QMouseEvent *event )
{
  Q_UNUSED( event )
//   kDebug() << k_funcinfo << endl;

  if( mInDrag ) {
    mInDrag = false;
    QApplication::restoreOverrideCursor();
    if( mDraggedElement && mHoveredElement && 
        mDraggedElement != mHoveredElement &&
        mHoveredElement->properties()->group == mActiveElement->properties()->group) {
      PositionAction *a = dynamic_cast<PositionAction *>(mEditor->action( "edit_position" ));
      if( a )
        a->perform( mDraggedElement, mHoveredElement );
    }
  }
  mDraggedElement = 0;

  if( !mInSelection )
    return;

  if( mEventLoop->isRunning() ) {
    mEventLoop->exit();
  }
  mInSelection = false;
}

void EditorWidget::paintEvent( QPaintEvent *event )
{
  Q_UNUSED( event )
//   kDebug() << k_funcinfo << endl;
  QPainter p( this );

  drawGroups( &p );
  drawWidgetFrames( &p );

  if( mInSelection ) {
    if( mHoveredElement != mActiveElement ) {
      if( mHoveredElement && mActiveElement->properties()->group != mHoveredElement->properties()->group )
        printMessage( &p, mElementMap[mHoveredElement], i18n("Groups do not match: %1 != %2", mActiveElement->properties()->group,
           mHoveredElement->properties()->group));
      else
        targetElement( &p, mElementMap[mHoveredElement], mHoveredElement );
    }
  }

  if( !mInEdit ) {
    if( mHoveredElement ) {
      highlightElement( &p, mElementMap[mHoveredElement], mHoveredElement );
      drawInterface( &p, mElementMap[mHoveredElement], mHoveredElement );
    } else {
      mEditButton->hide();
    }
  } else {
    if( mActiveElement ) {
        highlightElement( &p, mElementMap[mActiveElement], mActiveElement );
    }
  }

  drawGlobalInterface( &p );
}

void EditorWidget::drawGlobalInterface( QPainter *p )
{
  p->save();

  QBrush b( QColor(0,0,0,25) );
  p->fillRect( rect(), b );

  mInterfaceWidget->move( width() - mInterfaceWidget->width()-10, 10 );
  QRect r( width() - mInterfaceWidget->width() - 15, 5,  mInterfaceWidget->width()+10,  mInterfaceWidget->height()+10 );

  QPen pen;
  pen.setColor( QColor(255,255,255,255) );
  pen.setWidth( 2 );
  b.setColor( QColor(0,0,0,100) );
  p->setPen( pen );
  p->setBrush( b );
  p->drawRoundRect( r );

  p->restore();
}

void EditorWidget::drawGroups( QPainter *p )
{
  p->save();

  QList<QRect> alreadyPaintedRects;

  QBrush b( QColor(200,200,50,50) );
  QPen pen;
  pen.setColor( QColor(50,50,0,255) );
  pen.setWidth( 3 );
  p->setPen( pen );
  p->setBrush( b );

  QFont fnt;
  fnt.setPointSize( 14 );
  fnt.setBold( true );
  p->setFont( fnt );

  foreach( QString group, mGroupMap.keys() ) {
    p->save();
    QBrush b2 = b;
    foreach( QRect otherRect, alreadyPaintedRects ) {
      if( mGroupMap[group].intersects( otherRect ) ) {
        b2 = QBrush( QColor(0,200,0,50) );
        QPen pen2;
        pen2.setColor( QColor(50,150,50,255) );
        pen2.setWidth( 3 );
        p->setPen( pen2 );
        break;
      }
    }
    p->fillRect( mGroupMap[group], b2 );
    p->drawText( mGroupMap[group].center(), i18n("Group: %1", group ) );
    alreadyPaintedRects.append( mGroupMap[group] );
    p->restore();
  }
  p->restore();
}

void EditorWidget::drawWidgetFrames( QPainter *p )
{
  p->save();

  QPen pen;
  pen.setColor( QColor(50,50,50,150) );
  pen.setWidth( 1 );
  p->setPen( pen );

  foreach( QRect r, mElementMap.values() ) {
    p->drawRect( r );
  }

  p->restore();
}

void EditorWidget::targetElement( QPainter *p, const QRect &r, GuiElement *w )
{
  p->save();

  QPen pen;
  pen.setColor( QColor(255,0,0,255) );
  pen.setWidth( 3 );
  p->setPen( pen );
  p->drawRect( r );

  QBrush b( QColor(0,0,0,50) );
  p->fillRect( r, b );

  if( w ) {
    QPoint point( r.x()+20, r.y() );
    QFont fnt;
    fnt.setPointSize( 14 );
    fnt.setBold( true );
    p->setFont( fnt );
    p->drawText( point + QPoint(0,QFontMetrics( fnt ).height() ), w->id().toString() );
  }

  p->restore();
}

void EditorWidget::highlightElement( QPainter *p, const QRect &r, GuiElement *w )
{
  p->save();

  QPoint point( r.x()+30+mEditButton->sizeHint().width(), r.y() );

  QPen pen;
  pen.setColor( QColor(255,255,255,255) );
  pen.setWidth( 3 );
  p->setPen( pen );
  p->drawRect( r );

  QBrush b( QColor(0,0,0,100) );
  p->fillRect( r, b );

  QFont fnt;
  fnt.setPointSize( 14 );
  fnt.setBold( true );
  p->setFont( fnt );
  p->drawText( point + QPoint(0,QFontMetrics( fnt ).height() ), w->id().toString() );

  p->restore();
}

void EditorWidget::printMessage( QPainter *p, const QRect &r, const QString &msg )
{
  p->save();

  QPen pen;
  pen.setColor( QColor(0,0,0,180) );
  pen.setWidth( 3 );
  p->setPen( pen );
  p->drawRect( r );

  QBrush b( QColor(0,0,0,50) );
  p->fillRect( r, b );

  QPoint point( r.x()+20, r.y() );
  QFont fnt;
  fnt.setPointSize( 14 );
  fnt.setBold( true );
  p->setFont( fnt );
  p->drawText( point + QPoint(0,QFontMetrics( fnt ).height() ), msg );

  p->restore();
}

void EditorWidget::drawInterface( QPainter *p, const QRect &r, GuiElement *w )
{
  Q_UNUSED( p );
  Q_UNUSED( w );

  QPoint point( r.x()+20, r.y() );
  mEditButton->move( point );
  mEditButton->show();
}

void EditorWidget::showActionMenu()
{
  KActionMenu *menu = mEditor->actionMenu( mHoveredElement );
  menu->menu()->popup( mapToGlobal( mEditButton->pos() ) );
  mActiveElement = mHoveredElement;
}

void EditorWidget::showHints()
{
  QString text = mEditor->hints().toRichText();
  QWhatsThis::showText( mapToGlobal( mShowHintsButton->pos() ), text );
}

void EditorWidget::editDefaults()
{
  GlobalSettingsDialog *dlg = new GlobalSettingsDialog( mEditor->manager(), this );
  Hints hints;

  if( dlg->exec() == QDialog::Accepted ) {
    hints = dlg->hints();
  }
  dlg->deleteLater();

  mEditor->applyHints( hints );
}

GuiElement *EditorWidget::selectElement( Editor::SelectionMode sm )
{
  mInSelection = true;
  mSelectionMode = sm;
  mEventLoop->exec();
  if( !mHoveredElement | !mActiveElement || mHoveredElement->properties()->group !=
      mActiveElement->properties()->group )
    mHoveredElement = 0;
  return mHoveredElement;
}

#include "editorwidget.moc"
