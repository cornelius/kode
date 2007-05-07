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

#include "appearanceaction.h"

#include "editor.h"
#include "editorwidget.h"
#include "../hints.h"
#include "../guielement.h"

#include <kinputdialog.h>
#include <kdebug.h>
#include <klocale.h>

using namespace KXForms;

AppearanceAction::AppearanceAction( Editor *e)
: EditorAction( e )
{
}

AppearanceAction::~AppearanceAction()
{
}

void AppearanceAction::perform( EditorWidget *w )
{
  kDebug() << k_funcinfo << endl;
  editor()->beginEdit();

  QString newStyle;
  bool ok;
  QStringList list;
  list << "minimal" << "compact" << "full";
  newStyle = KInputDialog::getItem( i18n("Select the appearance style"), i18n("Appearance style for %1:", w->element()->ref().toString()),
      list, 0, false, &ok );

  if( ok ) {
    kDebug() << k_funcinfo << "New Style: " << newStyle << endl;
    Hint h;
    h.setRef( w->element()->ref() );
    h.setValue( Hint::Appearance, newStyle );
    emit hintGenerated( h );
  }

  editor()->finishEdit();
}

#include "appearanceaction.moc"
