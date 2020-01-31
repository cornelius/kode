/*
    This file is part of KDE.

    Copyright (c) 2007 André Duffeck <aduffeck@suse.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef KXFORMS_LISTPROXYMODEL_H
#define KXFORMS_LISTPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace KXForms {
class ListProxyModel : public QSortFilterProxyModel
{
public:
    ListProxyModel(QObject *parent);

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
};
}

#endif
