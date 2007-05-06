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

namespace KXForms {

class EditorWidget : public QLabel
{
  Q_OBJECT
  public:
    EditorWidget( QWidget *parent = 0 ) : QLabel( parent ) {};

    void setBuddyWidget( QWidget *w );
    void takeSnapshot();

  private:
    QWidget *mBuddyWidget;
};

}
#endif
