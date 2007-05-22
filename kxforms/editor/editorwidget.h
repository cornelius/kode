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

#include <QWidget>
#include "../guielement.h"

class QTimer;
class QPushButton;

namespace KXForms {

class Editor;

class EditorWidget : public QWidget
{
  Q_OBJECT
  public:

    EditorWidget( Editor *e, QWidget *parent = 0 );

    void setGuiElements( const GuiElement::List &list ) { mGuiElements = list; }

    GuiElement *hoveredElement() { return mHoveredElement; }
  public Q_SLOTS:
    void showActionMenu();

  protected:
    void mouseMoveEvent( QMouseEvent *event );
    void paintEvent( QPaintEvent *event );
    void drawInterface( QPainter *p, GuiElement *e );

  private:
    Editor *mEditor;
    GuiElement::List mGuiElements;

    QPushButton *mEditButton;

    GuiElement *mHoveredElement;
};

}
#endif
