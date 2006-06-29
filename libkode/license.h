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
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef KODE_LICENSE_H
#define KODE_LICENSE_H

#include <kode_export.h>

class QString;

namespace KODE {

/**
 * @brief Represent a license clause.
 * Use this class to setup a license clause for your generated file.
 *
 * Possible type of licenses are GPL, LGPL and BSD.
 *
 * @author Cornelius Schumacher <schumacher@kde.org>
 */
class LIBKODE_EXPORT License
{
  public:
    /**
     * Possible types of licenses
     */
    enum Type {
       GPL, ///< GNU General Public License v2, the classic free software license.
       LGPL, ///< GNU Library/Lesser General Public License v2, GNU license for library.
       BSD, ///< Berkeley Software Distribution
    };

    /**
     * @brief Create a new license clause definition
     */
    License();

    /**
     * @brief Create a new license clause definition for the given type.
     * @param type License Type.
     */
    License( Type type );

    /**
     * Creates a new license from @param other.
     */
    License( const License &other );

    /**
     * @brief Destroys the license.
     */
    ~License();

    /**
     * @brief Assignment operator.
     */
    License& operator=( const License &other );

    /**
     * @brief Sets whether a Qt expection should be appended to
     * the license statement.
     *
     * This is only useful for Qt3 based code.
     */
    void setQtException( bool useQtException );

    /**
     * @brief Get the textual presentation of the license.
     * @return textual presentation of the license.
     */
    QString text() const;

  private:
    class Private;
    Private *d;
};

}

#endif
