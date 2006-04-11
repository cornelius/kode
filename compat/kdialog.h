/*
    This file is part of Kode.

    Copyright (c) 2006 Cornelius Schumacher <schumacher@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef KDIALOG_H
#define KDIALOG_H

#include <QDialog>

class KDialog : public QDialog
{
  public:
    enum ButtonCode
    {
      Help    = 0x00000001, ///< Show Help button. (this button wil run the help set with setHelp)
      Default = 0x00000002, ///< Show Default button.
      Ok      = 0x00000004, ///< Show Ok button. (this button accept the dialog)
      Apply   = 0x00000008, ///< Show Apply button.
      Try     = 0x00000010, ///< Show Try button.
      Cancel  = 0x00000020, ///< Show Cancel-button. (this button reject the dialog)
      Close   = 0x00000040, ///< Show Close-button. (this button close the dialog)
      No      = 0x00000080, ///< Show No button. (this button close the dialog and set the result to No)
      Yes     = 0x00000100, ///< Show Yes button. (this button close the fialog and set the result to Yes)
      Details = 0x00000400, ///< Show Details button. (this button will show the detail widget set with setDetailsWidget)
      User1   = 0x00001000, ///< Show User defined button 1.
      User2   = 0x00002000, ///< Show User defined button 2.
      User3   = 0x00004000, ///< Show User defined button 3.
      Filler  = 0x40000000, ///< @internal Ignored when used in a constructor.
      Stretch = 0x80000000, ///< @internal Ignored when used in a constructor.
      NoDefault             ///< Used when specifying a default button; indicates that no button should be marked by default. 
    };
    Q_DECLARE_FLAGS(ButtonCodes, ButtonCode)

    KDialog( QWidget *parent, const QString &title, ButtonCodes buttonMask = 0);

    void setMainWidget( QWidget * );
    QWidget *mainWidget() { return mMainWidget; };

    static int marginHint() { return 8; }

  private:
    QWidget *mMainWidget;
};

#endif
