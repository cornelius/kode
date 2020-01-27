/* This file is part of the KDE project
   Copyright (C) 1999 Waldo Bastian <bastian@kde.org>

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
#ifndef KCMDLINEARGS_H
#define KCMDLINEARGS_H

#include "kurl.h"

#include <QString>

class KAboutData;

struct KCmdLineOptions
{
    const char *name;
    const char *description;
    const char *def; // Default
};

#define KCmdLineLastOption                                                                         \
    {                                                                                              \
        0, 0, 0                                                                                    \
    }

class KCmdLineArgs
{
    friend class KApplication;

public:
    static void addCmdLineOptions(const KCmdLineOptions *options);

    bool isSet(const QString &) const;
    QString getOption(const QString &);

    int count() const;

    KUrl url(int index) const;

    static void init(int _argc, char **_argv, const KAboutData *about);

    static KCmdLineArgs *parsedArgs();

private:
    static KCmdLineArgs *mParsedArgs;

    static int *qt_argc();
    static char ***qt_argv();

    static int argc; // The original argc
    static char **argv; // The original argv
};

#endif
