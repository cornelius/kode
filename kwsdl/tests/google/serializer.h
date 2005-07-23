/*
    This file is part of KDE.

    Copyright (c) 2005 Tobias Koenig <tokoe@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
    
    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <qcstring.h>
#include <qdom.h>
#include <qdatetime.h>
#include <qstring.h>
#include <qptrlist.h>

class ResultElementArray;
class DirectoryCategoryArray;
class GoogleSearchResult;
class DirectoryCategory;
class ResultElement;

class Serializer
{
  public:
    static QString marshalValue( const QString* value );
    static void demarshalValue( const QString &str, QString *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const QString* value );
    static void demarshal( const QDomElement &element, QString* value );
    static QString marshalValue( const bool* value );
    static void demarshalValue( const QString &str, bool *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const bool* value );
    static void demarshal( const QDomElement &element, bool* value );
    static QString marshalValue( const float* value );
    static void demarshalValue( const QString &str, float *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const float* value );
    static void demarshal( const QDomElement &element, float* value );
    static QString marshalValue( const int* value );
    static void demarshalValue( const QString &str, int *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const int* value );
    static void demarshal( const QDomElement &element, int* value );
    static QString marshalValue( const unsigned int* value );
    static void demarshalValue( const QString &str, unsigned int *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const unsigned int* value );
    static void demarshal( const QDomElement &element, unsigned int* value );
    static QString marshalValue( const double* value );
    static void demarshalValue( const QString &str, double *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const double* value );
    static void demarshal( const QDomElement &element, double* value );
    static QString marshalValue( const char* value );
    static void demarshalValue( const QString &str, char *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const char* value );
    static void demarshal( const QDomElement &element, char* value );
    static QString marshalValue( const unsigned char* value );
    static void demarshalValue( const QString &str, unsigned char *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const unsigned char* value );
    static void demarshal( const QDomElement &element, unsigned char* value );
    static QString marshalValue( const short* value );
    static void demarshalValue( const QString &str, short *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const short* value );
    static void demarshal( const QDomElement &element, short* value );
    static QString marshalValue( const QByteArray* value );
    static void demarshalValue( const QString &str, QByteArray *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const QByteArray* value );
    static void demarshal( const QDomElement &element, QByteArray* value );
    static QString marshalValue( const QDateTime* value );
    static void demarshalValue( const QString &str, QDateTime *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const QDateTime* value );
    static void demarshal( const QDomElement &element, QDateTime* value );
    static QString marshalValue( const QDate* value );
    static void demarshalValue( const QString &str, QDate *value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const QDate* value );
    static void demarshal( const QDomElement &element, QDate* value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const ResultElementArray* value );
    static void demarshal( const QDomElement &parent, ResultElementArray* value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const DirectoryCategoryArray* value );
    static void demarshal( const QDomElement &parent, DirectoryCategoryArray* value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const GoogleSearchResult* value );
    static void demarshal( const QDomElement &parent, GoogleSearchResult* value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const DirectoryCategory* value );
    static void demarshal( const QDomElement &parent, DirectoryCategory* value );
    static void marshal( QDomDocument &doc, QDomElement &parent, const QString &name, const ResultElement* value );
    static void demarshal( const QDomElement &parent, ResultElement* value );
  
};

#endif
