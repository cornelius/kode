#include "kmessagebox.h"

void KMessageBox::sorry( QWidget *parent, const QString &text,
      const QString &caption )
{
}

void KMessageBox::error( QWidget *parent, const QString &text,
      const QString &caption )
{
}

int KMessageBox::warningContinueCancel(QWidget *parent,
                         const QString &text,
                         const QString &caption )
{
  return Cancel;
}
