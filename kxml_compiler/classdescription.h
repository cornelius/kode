/*
    This file is part of KDE.

    Copyright (c) 2010 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef CLASSDESCRIPTION_H
#define CLASSDESCRIPTION_H

#include <QString>
#include <QStringList>
#include <QList>

#include <libkode/enum.h>

class ClassProperty
{
public:
    typedef QList<ClassProperty> List;

    ClassProperty(const QString &type, const QString &name);

    QString type() const;
    QString name() const;

    void setIsList(bool);
    bool isList() const;

    void setTargetHasId(bool);
    bool targetHasId() const;

private:
    QString m_type;
    QString m_name;

    bool m_isList;
    bool m_targetHasId;
};

class ClassDescription
{
public:
    ClassDescription(const QString &name);

    QString name() const;

    void addProperty(const ClassProperty &);
    void addProperty(const QString &type, const QString &name);

    ClassProperty::List properties() const;

    bool hasProperty(const QString &name) const;

    void addEnum(const KODE::Enum &classEnum);
    void addEnum(const QString &name, const QStringList &enumItems);

    KODE::Enum::List enums() const;

    bool hasEnum(const QString &name) const;

private:
    QString m_name;

    ClassProperty::List m_properties;
    KODE::Enum::List m_enums;
};

#endif
