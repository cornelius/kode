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

#include "inputtypeaction.h"

#include "editor.h"
#include "editorwidget.h"
#include "../hints.h"
#include "../guielement.h"

#include <kinputdialog.h>
#include <kdebug.h>
#include <klocale.h>

using namespace KXForms;

InputTypeAction::InputTypeAction(Editor *e) : EditorAction(e) {}

InputTypeAction::~InputTypeAction() {}

void InputTypeAction::perform(GuiElement *e)
{
    kDebug();
    editor()->beginEdit();

    QString newType;
    bool ok;
    QStringList list;
    list << "xs:string"
         << "xs:integer"
         << "xs:boolean";

    int currentPosition = 0;
    if (e->properties())
        currentPosition = list.indexOf(e->properties()->type);

    newType = KInputDialog::getItem(i18n("Select the input type"),
                                    i18n("Input type for %1:", e->ref().toString()), list,
                                    currentPosition, true, &ok);

    if (ok) {
        kDebug() << "New Type:" << newType;
        Hint h;
        h.setRef(e->id());
        h.setValue(Hint::InputType, newType);
        emit hintGenerated(h);
    }

    editor()->finishEdit();
}

#include "inputtypeaction.moc"
