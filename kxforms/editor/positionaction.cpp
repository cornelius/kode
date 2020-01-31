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

#include "positionaction.h"

#include "editor.h"
#include "../hints.h"
#include "../guielement.h"

#include <kmessagebox.h>
#include <kinputdialog.h>
#include <kdebug.h>
#include <klocale.h>

using namespace KXForms;

PositionAction::PositionAction(Editor *e) : EditorAction(e) {}

PositionAction::~PositionAction() {}

void PositionAction::perform(GuiElement *e)
{
    kDebug();
    editor()->beginEdit();

    KMessageBox::information(e->widget(),
                             i18n("Select the new neighbour of %1", e->ref().toString()),
                             i18n("New Neighbour"));
    GuiElement *chosenElement = editor()->selectWidget(Editor::SelectSameGroupOnly);

    if (!chosenElement) {
        editor()->finishEdit();
        return;
    }
    kDebug() << "Chosen element:" << chosenElement->ref().toString();

    perform(e, chosenElement);
}

void PositionAction::perform(GuiElement *e, GuiElement *target)
{

    QString position;
    QStringList list;
    bool ok;
    list << "above"
         << "rightOf"
         << "below"
         << "leftOf";
    position = KInputDialog::getItem(i18n("Select the relative Position"),
                                     i18n("Relative Position of %1:", e->ref().toString()), list, 0,
                                     false, &ok);

    if (!ok) {
        editor()->finishEdit();
        return;
    }

    Hint h;
    h.setRef(e->id());
    QDomDocument doc;
    QDomElement elem = doc.createElement(position);
    QDomNode child = doc.createTextNode(target->ref().toString());
    elem.appendChild(child);
    h.addElement(Hint::Position, elem);
    emit hintGenerated(h);

    editor()->finishEdit();
}

#include "positionaction.moc"
