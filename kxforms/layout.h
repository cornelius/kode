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
#ifndef KXFORMS_LAYOUT_H
#define KXFORMS_LAYOUT_H

#include "reference.h"
#include <QList>

namespace KXForms {

class GuiElement;

class Layout
{
public:
    class Element
    {
    public:
        Element(GuiElement *e) : mElement(e), mRight(0), mLeft(0), mAbove(0), mBelow(0) {}

        GuiElement *element() const { return mElement; }

        Element *rightElement() const { return mRight; }
        void setRight(Element *e)
        {
            mRight = e;
            mChildren.append(e);
        }
        Element *leftElement() const { return mLeft; }
        void setLeft(Element *e) { mLeft = e; }
        Element *aboveElement() const { return mAbove; }
        void setAbove(Element *e) { mAbove = e; }
        Element *belowElement() const { return mBelow; }
        void setBelow(Element *e)
        {
            mBelow = e;
            mChildren.append(e);
        }

        QList<Element *> children() const { return mChildren; }

        int width() const;
        int height() const;
        int space() const;

    private:
        GuiElement *mElement;
        Element *mRight;
        Element *mLeft;
        Element *mAbove;
        Element *mBelow;
        QList<Element *> mChildren;
    };

    Layout() : mOrdered(false) {}
    ~Layout();

    QList<Element *> orderedList() const;
    QList<GuiElement *> elements() const { return mElements; }

    void addElement(GuiElement *e);

    void order();
    int space() const;
    int width() const;
    int height() const;

private:
    Element *findElement(const Reference &ref) const;
    Element *findElement(Element *e, const Reference &ref) const;

private:
    QList<Element *> mOrderedList;
    QList<GuiElement *> mElements;
    bool mOrdered;
};
}

#endif
