/*
    This file is part of KXForms.

    Copyright (c) 2005,2006 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef KXFORMS_FORM_H
#define KXFORMS_FORM_H

#include <QList>
#include <QDomElement>

class QWidget;

namespace KXForms {

class Manager;
class FormGui;

class Form
{
  public:
    typedef QList<Form *> List;

    Form( Manager * );

    void setRef( const QString & );
    QString ref() const;

    bool isValid() const;

    void setElement( const QDomElement & );
    QDomElement element() const;

    QString label() const;

  private:
    Manager *mManager;

    QString mRef;
    QDomElement mElement;
};

}

#endif
