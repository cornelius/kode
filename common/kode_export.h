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

#include <kdemacros.h>

#ifdef Q_WS_WIN

#ifndef KODE_EXPORT
# ifdef MAKE_KODE_LIB
#  define KODE_EXPORT KDE_EXPORT
# elif KDE_MAKE_LIB
#  define KODE_EXPORT KDE_IMPORT
# else
#  define KODE_EXPORT
# endif
#endif


#ifndef KXMLCOMMON_EXPORT
# ifdef MAKE_KXMLCOMMON_LIB
#  define KXMLCOMMON_EXPORT KDE_EXPORT
# elif KDE_MAKE_LIB
#  define KXMLCOMMON_EXPORT KDE_IMPORT
# else
#  define KXMLCOMMON_EXPORT
# endif
#endif


#ifndef LIBKODE_EXPORT
# ifdef MAKE_LIBKODE_LIB
#  define LIBKODE_EXPORT KDE_EXPORT
# elif KDE_MAKE_LIB
#  define LIBKODE_EXPORT KDE_IMPORT
# else
#  define LIBKODE_EXPORT
# endif
#endif


#ifndef KODE_SCHEMA_EXPORT
# ifdef MAKE_KODE_SCHEMA_LIB
#  define KODE_SCHEMA_EXPORT KDE_EXPORT
# elif KDE_MAKE_LIB
#  define KODE_SCHEMA_EXPORT KDE_IMPORT
# else
#  define KODE_SCHEMA_EXPORT
# endif
#endif


#ifndef KSCHEMA_EXPORT
# ifdef MAKE_KSCHEMA_LIB
#  define KSCHEMA_EXPORT KDE_EXPORT
# elif KDE_MAKE_LIB
#  define KSCHEMA_EXPORT KDE_IMPORT
# else
#  define KSCHEMA_EXPORT
# endif
#endif

#ifndef KWSDL_EXPORT
# ifdef MAKE_KWSDL_LIB
#  define KWSDL_EXPORT KDE_EXPORT
# elif KDE_MAKE_LIB
#  define KWSDL_EXPORT KDE_IMPORT
# else
#  define KWSDL_EXPORT
# endif
#endif

#else // not windows

#define KODE_EXPORT KDE_EXPORT
#define KXMLCOMMON_EXPORT KDE_EXPORT
#define LIBKODE_EXPORT KDE_EXPORT
#define KODE_SCHEMA_EXPORT KDE_EXPORT
#define KSCHEMA_EXPORT KDE_EXPORT
#define KWSDL_EXPORT KDE_EXPORT
#endif /* not windows */

#endif
