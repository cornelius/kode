/*
    This file is part of KDE.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef KMESSAGEBOX_H
#define KMESSAGEBOX_H

#include <QString>

class QWidget;

class KMessageBox
{
public:
    enum ButtonCode { Ok = 1, Cancel = 2, Yes = 3, No = 4, Continue = 5 };

    static void sorry(QWidget *parent, const QString &text, const QString &caption = QString());

    static void error(QWidget *parent, const QString &text, const QString &caption = QString());

    static int warningContinueCancel(QWidget *parent, const QString &text,
                                     const QString &caption = QString());
};

#endif
