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

#include "layoutstyleaction.h"

#include "editor.h"
#include "editorwidget.h"
#include "../hints.h"
#include "../guielement.h"

#include <kmessagebox.h>
#include <kinputdialog.h>
#include <kdebug.h>
#include <klocale.h>

using namespace KXForms;

LayoutStyleAction::LayoutStyleAction( Editor *e)
: EditorAction( e )
{
}

LayoutStyleAction::~LayoutStyleAction()
{
}

void LayoutStyleAction::perform( GuiElement *e )
{
  kDebug() << k_funcinfo << endl;
  editor()->beginEdit();

  QString style;
  QStringList list;
  bool ok;
  int current = e->properties()->layoutStyle;
  list << "horizontal" << "vertical";
  style = KInputDialog::getItem( i18n("Select the layout style"), i18n("Layout style of %1:", e->ref().toString()),
      list, current, false, &ok );

  if( !ok ) {
    editor()->finishEdit();
    return;
  }

  Hint h;
  h.setRef( e->id() );
  h.setValue( Hint::LayoutStyle, style );
  emit hintGenerated( h );

  editor()->finishEdit();
}

#include "layoutstyleaction.moc"
