/*
    This file is part of KDE.

    Copyright (c) 2006 Michaël Larouche <michael.larouche@kdemail.net>

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
#ifndef KODE_EXPORT_H
#define KODE_EXPORT_H

#include <qglobal.h>

#ifndef KODE_EXPORT
#    if defined(MAKE_KODE_LIB)
/* We are building this library */
#        define KODE_EXPORT Q_DECL_EXPORT
#    else
/* We are using this library */
#        define KODE_EXPORT Q_DECL_IMPORT
#    endif
#endif

#ifndef KXMLCOMMON_EXPORT
#    if defined(MAKE_KXMLCOMMON_LIB)
/* We are building this library */
#        define KXMLCOMMON_EXPORT Q_DECL_EXPORT
#    else
/* We are using this library */
#        define KXMLCOMMON_EXPORT Q_DECL_IMPORT
#    endif
#endif

#ifndef SCHEMA_EXPORT
#    if defined(MAKE_SCHEMA_LIB)
/* We are building this library */
#        define SCHEMA_EXPORT Q_DECL_EXPORT
#    else
/* We are using this library */
#        define SCHEMA_EXPORT Q_DECL_IMPORT
#    endif
#endif

#ifndef KSCHEMA_EXPORT
#    if defined(MAKE_KSCHEMA_LIB)
/* We are building this library */
#        define KSCHEMA_EXPORT Q_DECL_EXPORT
#    else
/* We are using this library */
#        define KSCHEMA_EXPORT Q_DECL_IMPORT
#    endif
#endif

#ifndef KUNG_EXPORT
#    if defined(MAKE_KUNG_LIB)
/* We are building this library */
#        define KUNG_EXPORT Q_DECL_EXPORT
#    else
/* We are using this library */
#        define KUNG_EXPORT Q_DECL_IMPORT
#    endif
#endif

#ifndef WSCL_EXPORT
#    if defined(MAKE_WSCL_LIB)
/* We are building this library */
#        define WSCL_EXPORT Q_DECL_EXPORT
#    else
/* We are using this library */
#        define WSCL_EXPORT Q_DECL_IMPORT
#    endif
#endif

#ifndef KSCHEMAWIDGETS_EXPORT
#    if defined(MAKE_KSCHEMAWIDGETS_LIB)
/* We are building this library */
#        define KSCHEMAWIDGETS_EXPORT Q_DECL_EXPORT
#    else
/* We are using this library */
#        define KSCHEMAWIDGETS_EXPORT Q_DECL_IMPORT
#    endif
#endif

#endif /* KODE_EXPORT_H */
