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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "statemachine.h"

using namespace KODE;

StateMachine::StateMachine()
{
}


void StateMachine::setState( const QString &state, const Code &code )
{
  mStateMap.insert( state, code );

  if ( mInitialState.isEmpty() ) mInitialState = state;
}

void StateMachine::setInitialState( const QString &state )
{
  mInitialState = state;
}

Code StateMachine::stateDefinition()
{
  Code code;
  
  QStringList states;  
  QMap<QString,Code>::ConstIterator it;
  for ( it = mStateMap.begin(); it != mStateMap.end(); ++it ) {
    states.append( it.key() );
  }
  
  code += "enum State { " + states.join( ", " ) + " };";
  code += "State state = " + mInitialState + ";";

  return code;
}

Code StateMachine::transitionLogic()
{
  Code code;

  code += "switch( state ) {";
  code.indent();

  QMap<QString,Code>::ConstIterator it;
  for ( it = mStateMap.begin(); it != mStateMap.end(); ++it ) {
    code += "case " + it.key() + ":";
    code.indent();
    code.addBlock( it.data() );
    code += "break;";
    code.unindent();
  }
  
  code += "default:";
  code.indent();
  code += "break;";
  code.unindent();
  
  code.unindent();
  code += "}";
  
  return code;
}
