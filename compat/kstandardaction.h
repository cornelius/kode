/* This file is part of the KDE libraries
   Copyright (C) 1999,2000 Kurt Granroth <granroth@kde.org>
   Copyright (C) 2001,2002 Ellis Whitehead <ellis@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef KSTANDARDACTION_H
#define KSTANDARDACTION_H

class QObject;
class KActionCollection;
class KAction;

namespace KStandardAction
{
  void openNew(const QObject *recvr, const char *slot,
    KActionCollection* parent, const char *name = 0 );
  void open(const QObject *recvr, const char *slot,
    KActionCollection* parent, const char *name = 0 );
  void save(const QObject *recvr, const char *slot,
    KActionCollection* parent, const char *name = 0 );
  void saveAs(const QObject *recvr, const char *slot,
    KActionCollection* parent, const char *name = 0 );
  void quit(const QObject *recvr, const char *slot,
    KActionCollection* parent, const char *name = 0 );
  void preferences(const QObject *recvr, const char *slot,
    KActionCollection* parent, const char *name = 0 );
}

#endif
