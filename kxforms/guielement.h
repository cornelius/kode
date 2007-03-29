/*
    This file is part of KXForms.

    Copyright (c) 2005,2006 Cornelius Schumacher <schumacher@kde.org>

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
#ifndef KXFORMS_GUIELEMENT_H
#define KXFORMS_GUIELEMENT_H

#include "reference.h"

#include <QList>
#include <QDomElement>
#include <QLabel>
#include <QMap>

namespace KXForms {

class Manager;

class GuiElement : public QObject
{
  Q_OBJECT
  public:

    enum Appearance { Undefined, Minimal, Compact, Full };
    struct Properties {
        Properties() : readonly( false ), position( -1 ),
            halign( 0 ), valign( Qt::AlignTop ), appearance( Undefined ) {}
        QString type;
        QString constraint;
        bool readonly;
        QMap<QString, QString> relevance;
        QString page;
        int position;
        Qt::Alignment halign;
        Qt::Alignment valign;
        Appearance appearance;

        Qt::Alignment alignment() { return (halign | valign); }
    };

    typedef QList<GuiElement *> List;

    GuiElement( QWidget *parent, Manager *m, Properties *p );
    virtual ~GuiElement();

    virtual void parseElement( const QDomElement & ) {}
    static void parseProperties( const QDomElement &, Properties * );

    void setRef( const Reference & );
    Reference ref() const;

    QDomElement context() const;

    QDomElement createElement( const Reference & );

    void loadData( const QDomElement &context );

    virtual QWidget *widget() const { return mWidget; }
    virtual QWidget *labelWidget() const { return mLabel; }

    virtual void loadData() = 0;
    virtual void saveData() = 0;
    virtual bool isValid() const { return true; }

    Properties *properties() const { return mProperties; }

    void setTip( const QString &tip );

  signals:
    void valueChanged( const QString &, const QString & );

  public slots:
    void slotValueChanged( const QString &, const QString & );
    void setRelevant( bool );

  protected:
    virtual void applyProperties();

  protected:
    QWidget *mParent;
    QLabel *mLabel;
    QWidget *mWidget;
    Manager *mManager;
    Properties *mProperties;

  private:
    Reference mRef;
    QDomElement mContext;
};

}

#endif
