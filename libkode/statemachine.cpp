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

#include <QMap>
#include <QStringList>

#include "statemachine.h"

using namespace KODE;

class StateMachine::Private
{
public:
    QMap<QString, Code> mStateMap;
    QString mInitialState;
};

StateMachine::StateMachine() : d(new Private) {}

StateMachine::StateMachine(const StateMachine &other) : d(new Private)
{
    *d = *other.d;
}

StateMachine::~StateMachine()
{
    delete d;
}

StateMachine &StateMachine::operator=(const StateMachine &other)
{
    if (this == &other)
        return *this;

    *d = *other.d;

    return *this;
}

void StateMachine::setState(const QString &state, const Code &code)
{
    d->mStateMap.insert(state, code);

    if (d->mInitialState.isEmpty())
        d->mInitialState = state;
}

void StateMachine::setInitialState(const QString &state)
{
    d->mInitialState = state;
}

Code StateMachine::stateDefinition() const
{
    Code code;

    QStringList states;
    QMap<QString, Code>::ConstIterator it;
    for (it = d->mStateMap.constBegin(); it != d->mStateMap.constEnd(); ++it) {
        states.append(it.key());
    }

    code += "enum State { " + states.join(", ") + " };";
    code += "State state = " + d->mInitialState + ';';

    return code;
}

Code StateMachine::transitionLogic() const
{
    Code code;

    code += "switch( state ) {";
    code.indent();

    QMap<QString, Code>::ConstIterator it;
    for (it = d->mStateMap.constBegin(); it != d->mStateMap.constEnd(); ++it) {
        code += "case " + it.key() + ':';
        code.indent();
        code.addBlock(it.value());
        code += "break;";
        code.unindent();
    }

    code += "default:";
    code.indent();
    code += "break;";
    code.unindent();

    code.unindent();
    code += '}';

    return code;
}
