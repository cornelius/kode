/*
    This file is part of KXForms.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

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

#include "formcreator.h"
#include "hints.h"

#include <kxml_compiler/schema.h>
#include <kxml_compiler/parserxsd.h>

#include <common/nsmanager.h>
#include <common/messagehandler.h>
#include <common/parsercontext.h>

#include <kapplication.h>
#include <dcopclient.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>
#include <kurl.h>

#include <QDir>
#include <QByteArray>

#include <iostream>

int main(int argc, char **argv)
{
    KApplication::disableAutoDcopRegistration();

    KAboutData about("schema2kxform", 0, ki18n("Schema to KXForms Converter"), "0.1",
                     ki18n("Schema to KXForms Converter"), KAboutData::License_GPL,
                     ki18n("(C) 2006 Cornelius Schumacher"), KLocalizedString(), 0,
                     "schumacher@kde.org");
    about.addAuthor(ki18n("Cornelius Schumacher"), KLocalizedString(), "schumacher@kde.org");

    KCmdLineArgs::init(argc, argv, &about);

    KCmdLineOptions options;
    options.add("ugh <file>", ki18n("UI Generation Hints"));
    options.add("+schema", ki18n("Schema of XML file"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app(false);

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if (args->count() < 1) {
        std::cerr << "Too few arguments." << std::endl;
        return 1;
    }
    if (args->count() > 1) {
        std::cerr << "Too many arguments." << std::endl;
        return 1;
    }

    QString schemaFilename = args->url(0).path();

    QFile schemaFile(schemaFilename);
    if (!schemaFile.open(QIODevice::ReadOnly)) {
        kError() << "Unable to open '" << schemaFilename << "'";
        return 1;
    }

    KXForms::Hints hints;

    if (args->isSet("ugh")) {
        QString ughFileName = args->getOption("ugh");
        QFile ughFile(ughFileName);
        if (!ughFile.open(QIODevice::ReadOnly)) {
            kError() << "Unable to open '" << ughFileName;
            return 1;
        }

        hints.parseFile(ughFile);

        foreach (KXForms::Hint h, hints.hints()) {
            kDebug() << "Hint" << h.ref() << h.label();
        }
    }

    ParserXsd parser;
    Schema::Document schemaDocument = parser.parse(schemaFile);

    KXForms::Hints schemaHints;
    schemaHints.extractHints(schemaDocument);

    KXForms::FormCreator creator;
    creator.setHints(schemaHints);
    creator.mergeHints(hints);

    QString form = creator.create(schemaDocument);

    std::cout << form.toLocal8Bit().data();
}
