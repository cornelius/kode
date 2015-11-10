/*
    This file is part of KDE.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
    USA.
*/
#ifndef WRITERCREATOR_H
#define WRITERCREATOR_H

#include "schema.h"

#include <libkode/file.h>

class WriterCreator
{
  public:
    WriterCreator( KODE::File &, Schema::Document &document, const QString &dtd );

    void createFileWriter( const QString &className,
      const QString &errorStream );

    void createElementWriter(KODE::Class &c, const Schema::Element &e , const QString targetNameSpace = "");

    void addWriteStartElement(QString tag, const QString &targetNameSpace, KODE::Code &code);
protected:
    void createIndenter( KODE::File & );

    QString dataToStringConverter( const QString &data, Schema::Node::Type );

    KODE::Code createAttributeWriter( const Schema::Element &element );

  private:
    KODE::File &mFile;
    Schema::Document &mDocument;
    QString mDtd;
};

#endif
