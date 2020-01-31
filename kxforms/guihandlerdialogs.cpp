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

#include "guihandlerdialogs.h"

#include "manager.h"

#include <kdebug.h>
#include <kmessagebox.h>
#include <klocale.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>

using namespace KXForms;

FormDialog::FormDialog(QWidget *parent, const QString &title, Manager *m)
    : KDialog(parent), mFormGui(0), mManager(m)
{
    QFrame *topFrame = new QFrame(this);
    setMainWidget(topFrame);
    mTopLayout = new QVBoxLayout(topFrame);

    setCaption(title);
    setButtons(Ok);
    connect(this, SIGNAL(okClicked()), SLOT(slotOk()));
}

void FormDialog::setGui(FormGui *gui)
{
    mTopLayout->addWidget(gui);
    mFormGui = gui;
}

void FormDialog::slotOk()
{
    kDebug() << "FormDialog::slotOk()";

    mFormGui->saveData();

    mManager->unregisterGui(mFormGui);

    emit aboutToClose();

    accept();
}

GuiHandlerDialogs::GuiHandlerDialogs(Manager *m) : GuiHandler(m), mRootGui(0) {}

QWidget *GuiHandlerDialogs::createRootGui(QWidget *parent)
{
    kDebug() << "GuiHandlerDialogs::createRootGui()";

    Form *f = manager()->rootForm();

    if (!f) {
        KMessageBox::sorry(parent, i18n("Root form not found."));
        return 0;
    }

    FormGui *gui = createGui(f, parent);

    gui->setRef('/' + f->ref());
    gui->parseElement(f->element());

    if (manager()->hasData()) {
        kDebug() << "Manager::createGui() Load data on creation";
        manager()->loadData(gui);
    }

    mRootGui = gui;
    mWidgetStack.push_back(gui);
    return gui;
}

void GuiHandlerDialogs::createGui(const Reference &ref, QWidget *parent)
{
    kDebug() << "GuiHandlerDialogs::createGui() ref: '" << ref.toString() << "'";

    if (ref.isEmpty()) {
        KMessageBox::sorry(parent, i18n("No reference."));
        return;
    }

    QString r = ref.segments().last().name();

    Form *f = manager()->form(r);

    if (!f) {
        KMessageBox::sorry(parent, i18n("Form '%1' not found.", ref.toString()));
        return;
    }

    FormDialog dlg(parent, i18n("Edit %1", ref.toString()), manager());
    connect(&dlg, SIGNAL(aboutToClose()), this, SLOT(slotDialogClosed()));

    FormGui *gui = createGui(f, dlg.mainWidget());
    if (!gui) {
        KMessageBox::sorry(parent, i18n("Unable to create GUI for '%1'.", ref.toString()));
        return;
    }

    gui->setRef(ref);
    gui->parseElement(f->element());

    dlg.setGui(gui);

    if (manager()->hasData()) {
        kDebug() << "Manager::createGui() Load data on creation";
        manager()->loadData(gui);
    }

    mWidgetStack.push_back(gui);
    dlg.exec();
}

FormGui *GuiHandlerDialogs::createGui(Form *form, QWidget *parent)
{
    if (!form) {
        kError() << "KXForms::Manager::createGui(): form is null.";
        return 0;
    }

    kDebug() << "Manager::createGui() form: '" << form->ref() << "'";

    if (mRootGui)
        mRootGui->saveData();

    FormGui *gui = new FormGui(form->label(), manager(), parent);
    if (gui)
        manager()->registerGui(gui);

    return gui;
}

void GuiHandlerDialogs::slotDialogClosed()
{
    mWidgetStack.pop_back();
    if (mRootGui)
        manager()->loadData(mRootGui);
}

#include "guihandlerdialogs.moc"
