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

#include "classdescription.h"

ClassProperty::ClassProperty(const QString &type, const QString &name)
    : m_type(type), m_name(name), m_isList(false), m_targetHasId(false)
{
}

QString ClassProperty::type() const
{
    return m_type;
}

QString ClassProperty::name() const
{
    return m_name;
}

void ClassProperty::setIsList(bool v)
{
    m_isList = v;
}

bool ClassProperty::isList() const
{
    return m_isList;
}

void ClassProperty::setTargetHasId(bool v)
{
    m_targetHasId = v;
}

bool ClassProperty::targetHasId() const
{
    return m_targetHasId;
}

ClassDescription::ClassDescription(const QString &name) : m_name(name) {}

QString ClassDescription::name() const
{
    return m_name;
}

void ClassDescription::addProperty(const QString &type, const QString &name)
{
    m_properties.append(ClassProperty(type, name));
}

void ClassDescription::addProperty(const ClassProperty &p)
{
    m_properties.append(p);
}

ClassProperty::List ClassDescription::properties() const
{
    return m_properties;
}

bool ClassDescription::hasProperty(const QString &name) const
{
    return std::any_of(m_properties.begin(), m_properties.end(),
                       [name](const ClassProperty &p) { return p.name() == name; });
}

void ClassDescription::addEnum(const KODE::Enum &classEnum)
{
    m_enums.append(classEnum);
}

void ClassDescription::addEnum(const QString &name, const QStringList &enumItems)
{
    m_enums.append(KODE::Enum(name, enumItems));
}

KODE::Enum::List ClassDescription::enums() const
{
    return m_enums;
}

bool ClassDescription::hasEnum(const QString &name) const
{
    return std::any_of(m_enums.begin(), m_enums.end(),
                       [name](const KODE::Enum &e) { return e.name() == name; });
}
