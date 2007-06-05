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
#include "../manager.h"

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

using namespace KXForms;

EditorWidget::EditorWidget( Editor *e, QWidget *parent )
  : QWidget( parent ), mEditor( e ), mHoveredElement( 0 ), mActiveElement( 0 ),
    mEventLoop( new QEventLoop( this ) ), mSelectionMode( false ), mInEdit( false )
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
  foreach( QRect r, mElementMap.values() ) {
    if( r.contains( pos ) ) {
      if( !newElement || 
          mElementMap[ newElement ].width() > r.width() ||
          mElementMap[ newElement ].height() > r.height() ) {
        newElement = mElementMap.key( r );
      }
    }
  }
  if( mHoveredElement != newElement )
    update();

  mHoveredElement = newElement;
}

void EditorWidget::mouseReleaseEvent( QMouseEvent *event )
{
  Q_UNUSED( event )
//   kDebug() << k_funcinfo << endl;
  if( !mSelectionMode )
    return;

  if( mEventLoop->isRunning() ) {
    mEventLoop->exit();
  }
  mSelectionMode = false;
}

void EditorWidget::paintEvent( QPaintEvent *event )
{
  Q_UNUSED( event )
//   kDebug() << k_funcinfo << endl;
  QPainter p( this );

  if( mSelectionMode ) {
    if( mHoveredElement != mActiveElement )
      targetElement( &p, mElementMap[mHoveredElement], mHoveredElement );
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

void EditorWidget::targetElement( QPainter *p, const QRect &r, GuiElement *w )
{
  Q_UNUSED( w )

  p->save();

  QPen pen;
  pen.setColor( QColor(255,0,0,255) );
  pen.setWidth( 3 );
  p->setPen( pen );
  p->drawRect( r );

  p->restore();
}

void EditorWidget::highlightElement( QPainter *p, const QRect &r, GuiElement *w )
{
  p->save();

  QPoint point( r.x()+20, r.y() );

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
  p->drawText( point + QPoint(0,QFontMetrics( fnt ).height() ), w->ref().toString() );

  p->restore();
}

void EditorWidget::drawInterface( QPainter *p, const QRect &r, GuiElement *w )
{
  Q_UNUSED( p );

  QPoint point( r.x()+20, r.y() );
  QFont fnt;
  fnt.setPointSize( 14 );
  fnt.setBold( true );
  point.setX( point.x() + 10 + QFontMetrics( fnt ).width( w->ref().toString() ));

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
  Hint h;

  if( dlg->exec() == QDialog::Accepted ) {
    h = dlg->hint();
  }
  dlg->deleteLater();

  if( h.isValid() )
    mEditor->applyHint( h );
}

GuiElement *EditorWidget::selectElement()
{
  mSelectionMode = true;
  mEventLoop->exec();
  return mHoveredElement;
}

#include "editorwidget.moc"
