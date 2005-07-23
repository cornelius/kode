/* 
    This file is part of KDE WSCL Parser

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

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

#include "transition.h"

using namespace WSCL;

Transition::Transition()
{
}

Transition::~Transition()
{
}

void Transition::setSourceInteraction( const QString &sourceInteraction )
{
  mSourceInteraction = sourceInteraction;
}

QString Transition::sourceInteraction() const
{
  return mSourceInteraction;
}

void Transition::setDestinationInteraction( const QString &destinationInteraction )
{
  mDestinationInteraction = destinationInteraction;
}

QString Transition::destinationInteraction() const
{
  return mDestinationInteraction;
}

void Transition::setSourceInteractionCondition( const QString &sourceInteractionCondition )
{
  mSourceInteractionCondition = sourceInteractionCondition;
}

QString Transition::sourceInteractionCondition() const
{
  return mSourceInteractionCondition;
}

