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

#ifndef WSCL_CONVERSATION_H
#define WSCL_CONVERSATION_H

#include "interaction.h"
#include "transition.h"

namespace WSCL {

class Conversation
{
  public:
    Conversation();
    ~Conversation();

    void setName( const QString &name );
    QString name() const;

    void setVersion( const QString &version );
    QString version() const;

    void setDescription( const QString &description );
    QString description() const;

    void setNameSpace( const QString &nameSpace );
    QString nameSpace() const;

    void setSchema( const QString &schema );
    QString schema() const;

    void setInitialInteraction( const QString &initialInteraction );
    QString initialInteraction() const;

    void setFinalInteraction( const QString &finalInteraction );
    QString finalInteraction() const;

    void setInteractions( const Interaction::List &interactions );
    Interaction::List interactions() const;

    void setTransitions( const Transition::List &transitions );
    Transition::List transitions() const;

  private:
    QString mName;
    QString mVersion;
    QString mDescription;
    QString mNameSpace;
    QString mSchema;
    QString mInitialInteraction;
    QString mFinalInteraction;

    Interaction::List mInteractions;
    Transition::List mTransitions;
};

}

#endif
