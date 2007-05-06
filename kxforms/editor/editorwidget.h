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
#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QLabel>

class QTimer;

namespace KXForms {

class Editor;
class GuiElement;

class EditorWidget : public QLabel
{
  Q_OBJECT
  public:
    EditorWidget( GuiElement *guiElem, Editor *e, QWidget *parent = 0 );

    void setBuddyWidget( QWidget *w );
    void takeSnapshot();

    GuiElement *element() const { return mElement; }

    virtual void enterEvent ( QEvent * event );
    virtual void leaveEvent ( QEvent * event );

  public Q_SLOTS:
    void actionTriggered();

  private Q_SLOTS:
    void showEditMenu();

  private:
    Editor *mEditor;
    QWidget *mBuddyWidget;
    GuiElement *mElement;

    QTimer *mMenuTimer;
};

}
#endif
