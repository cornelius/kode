// -*- c++ -*-
/* This file is part of the KDE libraries
    Copyright (C) 1997, 1998 Richard Moore <rich@kde.org>
                  1998 Stephan Kulow <coolo@kde.org>
                  1998 Daniel Grana <grana@ie.iwi.unibe.ch>
                  2000,2001 Carsten Pfeiffer <pfeiffer@kde.org>
                  2001 Frerich Raabe <raabe@kde.org>

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
#ifndef KFILEDIALOG_H
#define KFILEDIALOG_H

#include <kurl.h>

#include <QString>

class QWidget;

class KFileDialog
{
  public:
    static KUrl getOpenUrl(const KUrl& = KUrl(),
			   const QString& filter= QString(),
			   QWidget *parent= 0,
			   const QString& caption = QString());
    static KUrl getSaveUrl(const KUrl& startDir= KUrl(),
			   const QString& filter= QString(),
			   QWidget *parent= 0,
			   const QString& caption = QString());
};

#endif
