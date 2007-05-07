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

#include "editor.h"
#include "editorwidget.h"
#include "editoraction.h"
#include "changelabelaction.h"

#include "../guielement.h"

#include <kactionmenu.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmenu.h>

using namespace KXForms;

Editor::Editor()
: mEditMode( false ), mInEdit( false )
{
  setupActions();
}

Editor::~Editor()
{
  foreach( EditorAction *a, mActions.values() ) {
    delete a;
  }
}

void Editor::setupActions()
{
  EditorAction *a;

  a = new ChangeLabelAction( this );
  connect( a, SIGNAL(hintGenerated( const Hint & )), 
      SLOT(applyHint( const Hint & )) );
  mActions[ "edit_label" ] = a;
}

void Editor::registerElement( GuiElement *element )
{
  kDebug() << k_funcinfo << "Registered element " << element->ref().toString() << endl;
  mElements.append( element );
}

void Editor::setEditMode( bool enabled )
{
  kDebug() << k_funcinfo << "Setting editmode to " << enabled << endl;
  mEditMode = enabled;
  foreach( GuiElement *e, mElements ) {
    e->setEditMode( enabled );
  }
}

void Editor::toggleEditMode()
{
  setEditMode( !mEditMode );
}

KActionMenu *Editor::actionMenu( EditorWidget *w )
{
  KActionMenu *menu = new KActionMenu( this );

  menu->menu()->addTitle( i18n("Edit %1", w->element()->ref().toString() ) );

  KAction *titleAction = new KAction( i18n("Change Label"), menu );
  titleAction->setData( "edit_label" );
  QObject::connect( titleAction, SIGNAL(triggered(bool)), w, SLOT( actionTriggered() ) );
  menu->addAction( titleAction );

  return menu;
}

void Editor::performAction( const QString &actionId, EditorWidget *w )
{
  kDebug() << k_funcinfo << "Performing action " << actionId << endl;

  EditorAction *a = mActions[ actionId ];
  if( !a )
    return;

  a->perform( w );
}

void Editor::applyHint( const Hint &h )
{
  kDebug() << k_funcinfo << endl;
  mHints.dump();

  if( mHints.hint( h.ref() ).isValid() )
    mHints.hint( h.ref() ).merge( h );
  else
    mHints.insertHint( h );

  mHints.dump();
}

#include "editor.moc"
