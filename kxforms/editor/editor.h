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
#ifndef EDITOR_H
#define EDITOR_H

#include "../hints.h"

#include <QObject>
#include <QList>
#include <QMap>

#include <kurl.h>

class KActionMenu;
class QEventLoop;

namespace KXForms {

class EditorAction;
class EditorWidget;
class Manager;
class GuiElement;

class Editor : public QObject
{
    Q_OBJECT
public:
    enum ActionType {
        CommonActions = 0x1,
        ListActions = 0x2,
        AppearanceActions = 0x4,
        InputActions = 0x8
    };
    Q_DECLARE_FLAGS(ActionTypes, ActionType)
    enum SelectionMode { SelectAllElements, SelectSameGroupOnly };

    Editor(Manager *);
    ~Editor();

    void registerElement(GuiElement *);
    bool editMode() const { return mEditMode; }

    void beginEdit();
    void finishEdit();

    bool inEdit() const { return mInEdit; }

    KActionMenu *actionMenu(GuiElement *e);

    void performAction(const QString &actionId, GuiElement *w);
    EditorAction *action(const QString &actionId) { return mActions[actionId]; }

    void addHints(const Hints &h) { mHints.merge(h); }
    Hints hints() const { return mHints; }

    void setCurrentHintsUrl(const KUrl &url) { mHintsUrl = url; }

    GuiElement *selectWidget(SelectionMode sm);

    Manager *manager() { return mManager; }

Q_SIGNALS:
    void hintsChanged(const Hints &h);

public Q_SLOTS:
    void setEditMode(bool);
    void toggleEditMode();

    void applyHint(const Hint &h);
    void applyHints(const Hints &h);

    void saveHints();
    void saveHintsAs();

private Q_SLOTS:
    void actionTriggered();

private:
    void setupActions();

private:
    QList<GuiElement *> mElements;
    QMap<QString, EditorAction *> mActions;
    QEventLoop *mEventLoop;
    bool mEditMode;
    bool mInEdit;

    EditorWidget *mEditorWidget;

    Hints mHints;

    KUrl mHintsUrl;

    Manager *mManager;
};
}

Q_DECLARE_OPERATORS_FOR_FLAGS(KXForms::Editor::ActionTypes)
#endif
