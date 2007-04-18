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
#include "hints.h"
#include "reference.h"

#include <kxml_compiler/schema.h>

namespace KXForms {

class FormCreator
{
  public:
    FormCreator();

    QString create( const Schema::Document & );

    /**
      Set hints. Previously set hints are removed.
    */
    void setHints( const Hints & );
    /**
      Set hints. Previously set hints are kept, unless the new hints contain a
      hint for the same reference, which overrides the old hint.
    */
    void mergeHints( const Hints & );

    Hints hints() const;

  protected:
    void createForm( XmlBuilder *xml, const Schema::Element &element );

    void createLabel( XmlBuilder *parent, const Schema::Node &node );

    QString createListItemLabel( const Reference &r, const Reference &path, const QString &itemType );
    QString createListHeader( const Reference &r );
    QString getLabel( const Reference &ref, const QString &fallback = QString(),
      bool pluralize = false );

    QString humanizeString( const QString &, bool pluralize = false );

  private:
    void parseComplexType( const Schema::Element &r, XmlBuilder *xml, bool topLevel, Reference path );
    void parseElement( const Schema::Element &element, XmlBuilder *xml );
    void parseAttributes( const Schema::Element &element, XmlBuilder *xml, Reference path = Reference() );
    void applyCommonHints( XmlBuilder *xml, const Reference &ref );
    QList< Reference > collectSubElements( const Schema::Element &element, const Reference &path, int depth, bool includeAttributes );


    bool choiceOnly( const Schema::Element &element );
  private:
    Schema::Document mDocument;

    Hints mHints;

    QStringList mCollapsedForms;
};

}

#endif
