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

#include <QString>

#include "membervariable.h"

using namespace KODE;

class Variable::Private
{
public:
    Private() : mIsStatic(false) {}

    QString mType;
    QString mName;
    bool mIsStatic;
    QString mInitializer;
};

Variable::Variable() : d(new Private) {}

Variable::Variable(const Variable &other) : d(new Private)
{
    *d = *other.d;
}

Variable::Variable(const QString &name, const QString &type, bool isStatic) : d(new Private)
{
    d->mType = type;
    d->mIsStatic = isStatic;

    Q_ASSERT(!name.isEmpty());

    if (name.isEmpty()) {
        d->mName = "mUndefined";
    } else {
        d->mName = name;
    }
}

Variable::~Variable()
{
    delete d;
}

Variable &Variable::operator=(const Variable &other)
{
    if (this == &other)
        return *this;

    *d = *other.d;

    return *this;
}

void Variable::setName(const QString &name)
{
    d->mName = name;
}

QString Variable::name() const
{
    return d->mName;
}

void Variable::setType(const QString &type)
{
    d->mType = type;
}

QString Variable::type() const
{
    return d->mType;
}

void Variable::setStatic(bool isStatic)
{
    d->mIsStatic = isStatic;
}

bool Variable::isStatic() const
{
    return d->mIsStatic;
}

void Variable::setInitializer(const QString &initializer)
{
    d->mInitializer = initializer;
}

QString Variable::initializer() const
{
    return d->mInitializer;
}
