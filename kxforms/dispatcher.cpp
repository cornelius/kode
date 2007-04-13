/*
    This file is part of KXForms.

    Copyright (c) 2007 Andre Duffeck <aduffeck@suse.de>

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

#include "dispatcher.h"

#include <kdebug.h>

using namespace KXForms;

Dispatcher::Dispatcher()
{
  connect( this, SIGNAL(valueChanged(QString,QString)),
      SLOT(slotValueChanged(QString,QString)) );
}

void Dispatcher::registerElement( QObject *element )
{
  connect( element, SIGNAL(valueChanged(QString,QString)),
      SIGNAL(valueChanged(QString,QString)) );
  connect( this, SIGNAL(valueChanged(QString,QString)),
      element, SLOT(slotValueChanged(QString,QString)) );
}

void Dispatcher::slotValueChanged(const QString &ref, const QString &value )
{
  kDebug() << "Value changed: " << ref << " -> " << value << endl;
}

#include "dispatcher.moc"
