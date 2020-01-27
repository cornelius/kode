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
#ifndef KXFORMS_XFORMSCOMMON_H
#define KXFORMS_XFORMSCOMMON_H

#include <QDomElement>
#include <QString>

namespace KXForms {

class XFormsCommon
{
public:
    XFormsCommon();

    void setLabel(const QString &label) { mLabel = label; }
    QString label() const { return mLabel; }

    void setHint(const QString &hint) { mHint = hint; }
    QString hint() const { return mHint; }

    void setTip(const QString &tip) { mTip = tip; }
    QString tip() const { return mTip; }

    static XFormsCommon parseElement(const QDomElement &);

private:
    QString mLabel;
    QString mHint;
    QString mTip;
};
}

#endif
