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
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "kresult.h"

#include <klocale.h>
#include <kdebug.h>

KResult::KResult()
  : mType( Ok ), mErrorType( NotAnError ), mChainedResult( 0 )
{
}

KResult::KResult( Type type )
  : mType( type ), mChainedResult( 0 )
{
  if ( mType == Error ) mErrorType = Undefined;
  else mErrorType = NotAnError;
}

KResult::KResult( ErrorType error, const QString &details )
  : mType( Error ), mErrorType( error ), mDetails( details ),
    mChainedResult( 0 )
{
}

KResult::~KResult()
{
  delete mChainedResult;
}

KResult::KResult( const KResult &o )
{
  mType = o.mType;
  mErrorType = o.mErrorType;
  mDetails = o.mDetails;
  if ( o.mChainedResult ) mChainedResult = new KResult( *o.mChainedResult );
  else mChainedResult = 0;
}

KResult::operator bool() const
{
  return !isError();
}

bool KResult::isOk() const
{
  return mType == Ok;
}

bool KResult::isInProgress() const
{
  return mType == InProgress;
}

bool KResult::isError() const
{
  return mType == Error;
}


KResult::ErrorType KResult::error() const
{
  return mErrorType;
}


QString KResult::message() const
{
  switch ( mType ) {
    case Ok:
      return i18n("Ok");
    case InProgress:
      return i18n("In progress");
    case Error:
      switch ( mErrorType ) {
        case NotAnError:
          return i18n("Not an error");
        case Undefined:
          return i18n("Error");
        case InvalidUrl:
          return i18n("Invalid URL");
        case ConnectionFailed:
          return i18n("Connection failed");
        case WriteError:
          return i18n("Write error");
        case ReadError:
          return i18n("Read error");
        case WrongParameter:
          return i18n("Wrong Parameter");
        case ParseError:
          return i18n("Parse Error");
      }
  }

  kError() << "KResult::message(): Unhandled case" << endl;
  return QString::null;
}


void KResult::setDetails( const QString &details )
{
  mDetails = details;
}

QString KResult::details() const
{
  return mDetails;
}


KResult &KResult::chain( const KResult &result )
{
  mChainedResult = new KResult( result );
  return *this;
}

bool KResult::hasChainedResult() const
{
  return mChainedResult;
}

KResult KResult::chainedResult() const
{
  return *mChainedResult;
}


QString KResult::fullMessage() const
{
  QString msg = message();
  if ( !details().isEmpty() ) msg += ": " + details();
  return msg;
}

QString KResult::chainedMessage() const
{
  QString msg = fullMessage();
  if ( hasChainedResult() ) msg += "\n" + chainedResult().chainedMessage();
  return msg;
}
