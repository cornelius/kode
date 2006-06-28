/*
    This file is part of KDE.

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
#ifndef KODE_STATEMACHINE_H
#define KODE_STATEMACHINE_H

#include "code.h"

#include <kode_export.h>

class QString;

namespace KODE {

class LIBKODE_EXPORT StateMachine
{
  public:
    StateMachine();
    StateMachine( const StateMachine &other );

    ~StateMachine();

    StateMachine& operator=( const StateMachine &other );

    void setState( const QString &state, const Code &code );

    void setInitialState( const QString &state );

    Code stateDefinition();
    Code transitionLogic();

  private:
    class Private;
    Private *d;
};

}

#endif
