/*
    This file is part of KDE.

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
#ifndef PARSERXSD_H
#define PARSERXSD_H

#include "schema.h"

#include <schema/simpletype.h>
#include <schema/parser.h>
#include <kode_export.h>

#include <QFile>

namespace RNG {

class SCHEMA_EXPORT ParserXsd
{
public:
    ParserXsd();

    Schema::Document parse(QFile &);
    Schema::Document parse(const QString &);
    void setVerbose(bool verbose);

protected:
    Schema::Document parse(const XSD::Parser &parser);

    void setType(Schema::Node &node, const XSD::SimpleType &simpleType);

    void setAnnotations(Schema::Annotatable &annotatable, const XSD::Annotation::List &annotations);

private:
    Schema::Document mDocument;
    bool mVerbose;
};
}

#endif
