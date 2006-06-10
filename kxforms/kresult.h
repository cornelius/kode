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
#ifndef KRESULT_H
#define KRESULT_H

#include <QString>

/**
  This class represents the result of an operation. It's meant to be used as
  return value of functions for returning status and especially error
  information.

  There are three main types of result: Ok (operation successfully completed),
  InProgress (operation still in progress) and Error (operation failed).
  InProgress is used by asynchronous operations. Functions which start an
  operation and return before the operation is finished should return the
  InProgress type result.

  An error result can include information about the type of the error and a
  detailed error message. Translated error messages for the error types are
  available through the message() function. Additional detailed error messages
  can be set by the setDetails() function. A full error message including the
  type specific message and the details is available through fullMessage().

  KResult objects can be chained using the chain function. If an operation
  executes a suboperation which indicates failure by returning a KResult object
  the operation can create a new KResult object and chain the suboperation's
  KResult object to it. The error information of chained results is available
  through the chainedMessage() function.

  Examples:

    A function returning ok:

      KResult load()
      {
        return KResultOk();
      }

    Alternative notation:

      KResult load()
      {
        return KResult::Ok;
      }

    A function returning an error with a specific error message:

      KResult load()
      {
        return KResultError( i18n("Details about error") );
      }

    A function returning an error of a sepcific type:

      KResult load()
      {
        return KResultError( KResult::InvalidUrl );
      }

    Chaining errors:

      KResult loadFile()
      {
        KResult result = mFile.load();
        if ( !result.isError() ) {
          return KResultError( "File load error" ).chain( result );
        } else {
          return result;
        }
      }

    Checking for errors:

      KResult load() { ... }

      ...
      if ( !load() ) error();
*/
class KResult
{
  public:
    /**
      Type of result:
      Ok - Operation successfully completed
      InProgress - Operation still in progress
      Error - Operation failed
    */
    enum Type { Ok, InProgress, Error };
    /**
      Specific type of error.
    */
    enum ErrorType { NotAnError, Undefined, InvalidUrl, WrongParameter,
                     ConnectionFailed, WriteError, ReadError, ParseError };

    /**
      Construct KResult object. Default type is Ok.
    */
    KResult();
    /**
      Copy constructor.
    */
    KResult( const KResult & );
    /**
      Create KResult object of given type.
    */
    KResult( Type );
    /**
      Create error KResult object of given error type and optional detailed error
      message.
    */
    KResult( ErrorType, const QString &details = QString::null );

    /**
      Destruct KResult object.
    */
    ~KResult();

    /**
      Behave like a bool in the corresponding context. Ok and InProgress are
      considered as success and return true, Error is considered as failure and
      returns false.
    */
    operator bool() const;

    /**
      Return true, if KResult is of type Ok, otherwise return false.
    */
    bool isOk() const;
    /**
      Return true, if KResult is of type InProgress, otherwise return false.
    */
    bool isInProgress() const;
    /**
      Return true, if KResult is of type Error, otherwise return false.
    */
    bool isError() const;

    /**
      Return specific error type.
    */
    ErrorType error() const;

    /**
      Return translated string describing type of result corresponding to Type
      and ErrorType.
    */
    QString message() const;

    /**
      Set detailed error message. This error message should include all
      details needed to understand and recover from the error. This could be
      information like the URL which was tried, the file which could not be
      written or which parameter was missing.
    */
    void setDetails( const QString & );
    /**
      Return detailed error message. See details().
    */
    QString details() const;

    /**
      Return full error message. This includes the type-specific message (see
      message()) and the detailed message (see details()).
    */
    QString fullMessage() const;

    /**
      Chain result objects. This can be used to remember the cause of an error.
      The full error messages including the messages from chained objects can be
      accessed through chainedMessage().
    */
    KResult &chain( const KResult & );

    /**
      Return true, if the KResult object has a chained KResult object, otherwise
      return false.
    */
    bool hasChainedResult() const;
    /**
      Return chained KResult object.
    */
    KResult chainedResult() const;

    /**
      Return error message including full details of all chained messages. This
      can constitute a backtrace of a error.
    */
    QString chainedMessage() const;

  private:
    Type mType;
    ErrorType mErrorType;
    QString mDetails;
    KResult *mChainedResult;
};

/**
  Convenience class for creating a KResult of type Ok.
*/
class KResultOk : public KResult
{
  public:
    /**
      Create KResult object of type Ok.
    */
    KResultOk() : KResult( Ok ) {}
};

/**
  Convenience class for creating a KResult of type InProgress.
*/
class KResultInProgress : public KResult
{
  public:
    /**
      Create KResult object of type InProgress.
    */
    KResultInProgress() : KResult( InProgress ) {}
};

/**
  Convenience class for creating a KResult of type Error.
*/
class KResultError : public KResult
{
  public:
    /**
      Create KResult object of type Error.
    */
    KResultError() : KResult( Error ) {}
    /**
      Create KResult object of type Error with given error type and optionally
      a detailed error message.
    */
    KResultError( ErrorType error, const QString &details = QString::null )
      : KResult( error, details ) {}
    /**
      Create KResult object of type Error with given detailed error message.
    */
    KResultError( const QString &details ) : KResult( Undefined, details ) {}
};

#endif
