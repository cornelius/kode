/*
    This file is part of Kung.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#include <kaboutdata.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kdebug.h>
#include <klocale.h>

#include <qtimer.h>
#include <kurl.h>
#include "loader.h"

int main(int argc, char **argv)
{
    KAboutData aboutData("kung", 0, ki18n("KDE WSDL Interpreter"), "0.1",
                         ki18n("KDE WSDL Interpreter"), KAboutData::License_LGPL);

    aboutData.addAuthor(ki18n("Tobias Koenig"), KLocalizedString(), "tokoe@kde.org");

    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("+wsdl", ki18n("Location of WSDL file"));
    KCmdLineArgs::addCmdLineOptions(options);

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if (args->count() == 0) {
        kError() << "No WSDL file given.";
        return 1;
    }

    // KApplication requires DBUS which should be made optional
    QApplication app(argc, argv);
    KComponentData instance("kung");

    Loader loader;
    loader.setWSDLUrl(args->url(0).path());

    QTimer::singleShot(0, &loader, SLOT(run()));

    return app.exec();
}
