/*
 * This file is part of the KDE Libraries
 * Copyright (C) 2000 Espen Sand (espen@kde.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */
#ifndef KABOUTDATA_H
#define KABOUTDATA_H

class KAboutData
{
  public:

    enum LicenseKey
    {
      License_Custom = -2,
      License_File = -1,
      License_Unknown = 0,
      License_GPL  = 1,
      License_GPL_V2 = 1,
      License_LGPL = 2,
      License_LGPL_V2 = 2,
      License_BSD  = 3,
      License_Artistic = 4,
      License_QPL = 5,
      License_QPL_V1_0 = 5
    };

    KAboutData( const char *appName,
                const char *programName,
		const char *version,
		const char *shortDescription = 0,
		enum LicenseKey licenseType = License_Unknown,
		const char *copyrightStatement = 0,
		const char *text = 0,
		const char *homePageAddress = 0,
		const char *bugsEmailAddress = "submit@bugs.kde.org"
		);

    void addAuthor( const char *name,
		    const char *task=0,
		    const char *emailAddress=0,
		    const char *webAddress=0 );
};

#endif
