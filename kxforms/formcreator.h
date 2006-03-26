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
#ifndef FORMCREATOR_H
#define FORMCREATOR_H

#include "xmlbuilder.h"

#include <kxml_compiler/schema.h>

namespace KXForms {

class FormCreator
{
  public:
    FormCreator();

    QString create( const Schema::Document & );

  protected:
    void createForm( XmlBuilder *xml, const Schema::Element &element );

    QString humanizeString( const QString &, bool pluralize = false );

  private:
    Schema::Document mDocument;
};

}

#endif
