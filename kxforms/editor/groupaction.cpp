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

#include "groupaction.h"

#include "editor.h"
#include "editorwidget.h"
#include "../hints.h"
#include "../guielement.h"

#include <kinputdialog.h>
#include <kdebug.h>
#include <klocale.h>

#include <QLabel>

using namespace KXForms;

GroupAction::GroupAction( Editor *e)
: EditorAction( e )
{
}

GroupAction::~GroupAction()
{
}

void GroupAction::perform( GuiElement *e )
{
  kDebug() << k_funcinfo << endl;
  editor()->beginEdit();

  bool ok;

  QStringList groups;
  int currentGroup = groups.indexOf( e->properties()->group );
  QString newGroup;

  newGroup = KInputDialog::getItem( i18n("Select new group"), i18n("Group for %1:", e->ref().toString()),
      groups, currentGroup, true, &ok );

  if( ok ) {
    kDebug() << k_funcinfo << "New Group: " << newGroup << endl;
    Hint h;
    h.setRef( e->ref() );
    h.setValue( Hint::GroupReference, newGroup );
    emit hintGenerated( h );
  }

  editor()->finishEdit();
}

#include "groupaction.moc"
