/*
    This file is part of kdepim.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>

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

#include "kde-features.h"

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <kdebug.h>

#include <QFile>
#include <QTextStream>

#include <iostream>

static void displayFeature(const Feature &f)
{
    std::cout << "FEATURE: " << f.summary().toLocal8Bit().data() << std::endl;
    foreach (Responsible r, f.responsibleList()) {
        std::cout << "  RESPONSIBLE: " << r.name().toLocal8Bit().data() << " ("
                  << r.email().toLocal8Bit().data() << ")" << std::endl;
    }
    std::cout << "  TARGET: " << f.target().toLocal8Bit().data() << std::endl;
    std::cout << "  STATUS: " << f.status().toLocal8Bit().data() << std::endl;
}

static void displayCategory(Category::List categories)
{
    foreach (Category c, categories) {
        std::cout << "CATEGORY: " << c.name().toLocal8Bit().data() << std::endl;

        Feature::List features = c.featureList();
        foreach (Feature f, features) {
            displayFeature(f);
        }

        displayCategory(c.categoryList());
    }
}

int main(int argc, char **argv)
{
    KAboutData aboutData("testfeatures", 0, ki18n("Dump XML feature list to stdout"), "0.1");
    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("+featurelist", ki18n("Name of featurelist XML file"));
    options.add("output <file>", ki18n("Name of output file"));
    KCmdLineArgs::addCmdLineOptions(options);

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if (args->count() != 1) {
        args->usage("Wrong number of arguments.");
    }

    QString filename = args->arg(0);

    bool ok;
    Features features = Features::parseFile(filename, &ok);

    if (!ok) {
        kError() << "Parse error";
    } else {
        Category::List categories = features.categoryList();
        displayCategory(categories);
    }

    if (args->isSet("output")) {
        QString out = args->getOption("output");
        if (!features.writeFile(out)) {
            kError() << "Write error";
        }
    }
    return 0;
}
