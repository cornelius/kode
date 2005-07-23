/* 
    This file is part of KDE Schema Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>
                       based on wsdlpull parser by Vivek Krishna

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

#ifndef SCHEMA_ELEMENT_H
#define SCHEMA_ELEMENT_H

#include <qstring.h>
#include <qvaluelist.h>

#define UNBOUNDED 1000

namespace Schema {

class Element
{
  public:
    typedef QValueList<Element> List;
    typedef QValueList<Element*> PtrList;

    Element();

    Element( const QString &name, int type, int minOccurs = 1, int maxOccurs = 1, 
             bool qualified = false, const QString &defaultValue = QString(),
             const QString &fixedValue = QString() );

    QString name() const;

    void setType( int id );
    int type() const;

    void setTypeName( const QString &typeName );
    QString typeName() const;

    void setDocumentation( const QString &documentation );
    QString documentation() const;

    void setGroupId( int group );
    int groupId() const;

    void setMinOccurs( int minOccurs );
    int minOccurs() const;

    void setMaxOccurs( int maxOccurs );
    int maxOccurs() const;

    QString defaultValue() const;
    QString fixedValue() const;

    bool isQualified() const;

    void setOccurrence( int occurrence );
    int occurrence() const;

  private:
    QString mName;
    int mType;
    QString mTypeName;
    QString mDocumentation;
    int mMinOccurs;
    int mMaxOccurs;
    bool mQualified;
    QString mDefaultValue;
    QString mFixedValue;
    int mGroupId;
    int mOccurrence;
};

}

#endif
