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

#ifndef WSCL_TRANSITION_H
#define WSCL_TRANSITION_H

#include <qstring.h>
#include <qvaluelist.h>

namespace WSCL {

class Transition
{
  public:
    typedef QValueList<Transition> List;

    Transition();
    ~Transition();

    void setSourceInteraction( const QString &sourceInteraction );
    QString sourceInteraction() const;

    void setDestinationInteraction( const QString &destinationInteraction );
    QString destinationInteraction() const;

    void setSourceInteractionCondition( const QString &sourceInteractionCondition );
    QString sourceInteractionCondition() const;

  private:
    QString mSourceInteraction;
    QString mDestinationInteraction;
    QString mSourceInteractionCondition;
};

}

#endif
