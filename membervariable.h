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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
#ifndef KODE_MEMBERVARIABLE_H
#define KODE_MEMBERVARIABLE_H

#include "variable.h"

#include <qvaluelist.h>
#include <qstring.h>
#include <kdepimmacros.h>

namespace KODE {

class KDE_EXPORT MemberVariable : public Variable
{
  public:
    typedef QValueList<MemberVariable> List;

    MemberVariable();
    MemberVariable( const QString &name, const QString &type,
                    bool isStatic = false );
};

}

#endif
