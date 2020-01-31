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

#include "changelabelaction.h"

#include "editor.h"
#include "editorwidget.h"
#include "../hints.h"
#include "../guielement.h"

#include <kinputdialog.h>
#include <kdebug.h>
#include <klocale.h>

#include <QLabel>

using namespace KXForms;

ChangeLabelAction::ChangeLabelAction(Editor *e) : EditorAction(e) {}

ChangeLabelAction::~ChangeLabelAction() {}

void ChangeLabelAction::perform(GuiElement *e)
{
    kDebug();
    editor()->beginEdit();

    QString newLabel;
    bool ok;

    QString currentLabel;
    QLabel *labelWidget = dynamic_cast<QLabel *>(e->labelWidget());
    if (labelWidget)
        currentLabel = labelWidget->text();

    newLabel = KInputDialog::getText(i18n("Enter the new label"),
                                     i18n("Label for %1:", e->ref().toString()), currentLabel, &ok);

    if (ok) {
        kDebug() << "New Label:" << newLabel;
        Hint h;
        h.setRef(e->id());
        h.setValue(Hint::Label, newLabel);
        emit hintGenerated(h);
    }

    editor()->finishEdit();
}

#include "changelabelaction.moc"
