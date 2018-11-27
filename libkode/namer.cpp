/*
    This file is part of KDE.

    Copyright (c) 2004,2010 Cornelius Schumacher <schumacher@kde.org>

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

#include "namer.h"

#include <libkode/style.h>

#include <QRegularExpression>

QStringList Namer::m_reservedKeywords = QStringList()
                                                       << "alignas"
                                                       << "alignof"
                                                       << "and"
                                                       << "and_eq"
                                                       << "asm"
                                                       << "atomic_cancel"
                                                       << "atomic_commit"
                                                       << "atomic_noexcept"
                                                       << "audit"
                                                       << "auto"
                                                       << "axiom"
                                                       << "bitand"
                                                       << "bitor"
                                                       << "bool"
                                                       << "break"
                                                       << "case"
                                                       << "catch"
                                                       << "char"
                                                       << "char16_t"
                                                       << "char32_t"
                                                       << "class"
                                                       << "co_await"
                                                       << "co_return"
                                                       << "co_yield"
                                                       << "compl"
                                                       << "concept"
                                                       << "const"
                                                       << "const_cast"
                                                       << "constexpr"
                                                       << "continue"
                                                       << "decltype"
                                                       << "default"
                                                       << "define"
                                                       << "defined"
                                                       << "delete"
                                                       << "do"
                                                       << "double"
                                                       << "dynamic_cast"
                                                       << "elif"
                                                       << "else"
                                                       << "endif"
                                                       << "enum"
                                                       << "error"
                                                       << "explicit"
                                                       << "export"
                                                       << "extern"
                                                       << "false"
                                                       << "final"
                                                       << "float"
                                                       << "for"
                                                       << "friend"
                                                       << "goto"
                                                       << "if"
                                                       << "ifdef"
                                                       << "ifndef"
                                                       << "import"
                                                       << "include"
                                                       << "inline"
                                                       << "int"
                                                       << "line"
                                                       << "long"
                                                       << "module"
                                                       << "mutable"
                                                       << "namespace"
                                                       << "new"
                                                       << "noexcept"
                                                       << "not"
                                                       << "not_eq"
                                                       << "nullptr"
                                                       << "operator"
                                                       << "or"
                                                       << "or_eq"
                                                       << "override"
                                                       << "pragma"
                                                       << "private"
                                                       << "protected"
                                                       << "public"
                                                       << "reflexpr"
                                                       << "register"
                                                       << "reinterpret_cast"
                                                       << "requires"
                                                       << "return"
                                                       << "short"
                                                       << "signals"
                                                       << "signed"
                                                       << "sizeof"
                                                       << "slots"
                                                       << "static"
                                                       << "static_assert"
                                                       << "static_cast"
                                                       << "struct"
                                                       << "switch"
                                                       << "synchronized"
                                                       << "template"
                                                       << "this"
                                                       << "thread_local"
                                                       << "throw"
                                                       << "transaction_safe"
                                                       << "transaction_safe_dynamic"
                                                       << "true"
                                                       << "try"
                                                       << "typedef"
                                                       << "typeid"
                                                       << "typename"
                                                       << "undef"
                                                       << "union"
                                                       << "unsigned"
                                                       << "using"
                                                       << "virtual"
                                                       << "void"
                                                       << "volatile"
                                                       << "wchar_t"
                                                       << "while"
                                                       << "xor"
                                                       << "xor_eq";

QString Namer::upperFirst( const QString &str )
{
  return KODE::Style::upperFirst( str );
}

QString Namer::lowerFirst( const QString &str )
{
  return KODE::Style::lowerFirst( str );
}

QString Namer::getClassName( const QString &elementName )
{
  QString name;
  QStringList parts = removeInvalidCharacters(elementName).split( "_" );
  foreach( QString part, parts ) {
    name += upperFirst( part );
  }

  return name;
}

QString Namer::getAccessor( const QString &elementName )
{
  return substituteKeywords( lowerFirst( getClassName( elementName ) ) );
}

QString Namer::getListAccessor( const QString &elementName )
{
  return QString("%1List").arg(lowerFirst(getClassName(elementName)));
}

QString Namer::getMutator( const QString &elementName )
{
  return "set" + getClassName( elementName );
}

QString Namer::removeInvalidCharacters(const QString &name)
{
  QString ret = name;
  return ret.replace(QRegularExpression("[\\;\\-\\:\\,\\+\\*\\/\\<\\>\\|]"), "_");
}

QString Namer::substituteKeywords(const QString &name)
{
  if (m_reservedKeywords.contains(name)) {
    QString ret = name;
    return ret.append("_");
  }
  return name;
}

QString Namer::sanitize(const QString &name)
{
  return substituteKeywords(removeInvalidCharacters(name));
}
