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

#include "conversation.h"

using namespace WSCL;

Conversation::Conversation()
{
}

Conversation::~Conversation()
{
}

void Conversation::setName( const QString &name )
{
  mName = name;
}

QString Conversation::name() const
{
  return mName;
}

void Conversation::setVersion( const QString &version )
{
  mVersion = version;
}

QString Conversation::version() const
{
  return mVersion;
}

void Conversation::setDescription( const QString &description )
{
  mDescription = description;
}

QString Conversation::description() const
{
  return mDescription;
}

void Conversation::setNameSpace( const QString &nameSpace )
{
  mNameSpace = nameSpace;
}

QString Conversation::nameSpace() const
{
  return mNameSpace;
}

void Conversation::setSchema( const QString &schema )
{
  mSchema = schema;
}

QString Conversation::schema() const
{
  return mSchema;
}

void Conversation::setInitialInteraction( const QString &initialInteraction )
{
  mInitialInteraction = initialInteraction;
}

QString Conversation::initialInteraction() const
{
  return mInitialInteraction;
}

void Conversation::setFinalInteraction( const QString &finalInteraction )
{
  mFinalInteraction = finalInteraction;
}

QString Conversation::finalInteraction() const
{
  return mFinalInteraction;
}

void Conversation::setInteractions( const Interaction::List &interactions )
{
  mInteractions = interactions;
}

Interaction::List Conversation::interactions() const
{
  return mInteractions;
}

void Conversation::setTransitions( const Transition::List &transitions )
{
  mTransitions = transitions;
}

Transition::List Conversation::transitions() const
{
  return mTransitions;
}
