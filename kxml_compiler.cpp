/*
    This file is part of KDE.

    Copyright (c) 2004 Cornelius Schumacher <schumacher@kde.org>

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

#include <kode/kode.h>
#include <kode/printer.h>
#include <kode/typedef.h>

#include <kaboutdata.h>
#include <kapplication.h>
#include <kdebug.h>
#include <klocale.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <kconfig.h>
#include <ksimpleconfig.h>
#include <kstandarddirs.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qdom.h>
#include <qregexp.h>
#include <qmap.h>

#include <iostream>

class Pattern
{
  public:
    Pattern()
      : optional( false ), zeroOrMore( false ), oneOrMore( false ),
        choice( false )
    {
    }
  
    bool isEmpty()
    {
      return !optional && !zeroOrMore && !oneOrMore && !choice;
    }

    QString asString()
    {
      if ( isEmpty() ) return "";
      QString str = "( ";
      if ( optional ) str += "optional ";      
      if ( zeroOrMore ) str += "zeroOrMore ";
      if ( oneOrMore ) str += "oneOrMore ";
      if ( choice ) str += "choice ";
      str += ")";
      return str;
    }

    void merge( Pattern p )
    {
      if ( p.optional ) optional = true;
      if ( p.zeroOrMore ) zeroOrMore = true;
      if ( p.oneOrMore ) oneOrMore = true;
      if ( p.choice ) choice = true;
    }
  
    bool optional;
    bool zeroOrMore;
    bool oneOrMore;
    bool choice;
};

class Reference
{
  public:
    QString name;
    Pattern pattern;
};

class Attribute
{
  public:
    QString name;
    QValueList<QString> choices;
    QString defaultValue;
    Pattern pattern;
};

class Element
{
  public:
    Element() : hasText( false ) {}
  
    QString name;
    QValueList<Element *> elements;
    QValueList<Attribute *> attributes;
    QValueList<Reference *> references;
    Pattern pattern;
    bool hasText;
};

class Parser
{
  public:
    Parser()
    {
    }

    Element *parse( const QDomElement &docElement )
    {
      Element *start = 0;

      QDomNode n1;
      for( n1 = docElement.firstChild(); !n1.isNull(); n1 = n1.nextSibling() ) {
        QDomElement e1 = n1.toElement();
        kdDebug() << "TOP LEVEL element " << e1.tagName() << endl;
        if ( e1.tagName() == "define" ) {
          Element *d = new Element;
          d->name = e1.attribute( "name" );
          parseElement( e1, d, Pattern() );
          QValueList<Element *> definitions;
          QMap<QString,QValueList<Element *> >::ConstIterator it; 
          it = mDefinitionMap.find( d->name );
          if ( it != mDefinitionMap.end() ) definitions = *it;
          definitions.append( d );
          mDefinitionMap.replace( d->name, definitions );
        } else if ( e1.tagName() == "start" ) {
          start = new Element;
          parseElement( e1, start, Pattern() );
        } else {
          kdDebug() << "parseGrammar: Unrecognized tag: " << e1.tagName() << endl;
        }
      }
      
      return start;
    }

    Reference *parseReference( const QDomElement &referenceElement )
    {
      Reference *r = new Reference;
      r->name = referenceElement.attribute( "name" );
      return r;
    }

    bool parseAttribute( const QDomElement &attributeElement,
                               Attribute *a )
    {
      a->name = attributeElement.attribute( "name" );

      return true;
    }

    bool parseElement( const QDomElement &elementElement, Element *e,
                       Pattern pattern )
    {
      kdDebug() << "parseElement " << e->name << endl;

      QDomNode n1;
      for( n1 = elementElement.firstChild(); !n1.isNull(); n1 = n1.nextSibling() ) {
        QDomElement e1 = n1.toElement();
        if ( e1.tagName() == "element" ) {
          Element *element = new Element;
          element->name = e1.attribute( "name" );
          element->pattern = pattern;
          parseElement( e1, element, Pattern() );
          e->elements.append( element );
        } else if ( e1.tagName() == "attribute" ) {
          Attribute *a = new Attribute;
          a->name = e1.attribute( "name" );
          a->pattern = pattern;
          kdDebug() << "ATTRIBUTE: " << a->name << " " << a->pattern.asString()
                    << endl;
          parseAttribute( e1, a );
          e->attributes.append( a );
        } else if ( e1.tagName() == "ref" ) {
          Reference *r = parseReference( e1 );
          r->pattern = pattern;
          e->references.append( r );
        } else if ( e1.tagName() == "text" ) {
          e->hasText = true;
        } else {
          Pattern p = pattern;
          if ( e1.tagName() == "optional" ) p.optional = true;
          else if ( e1.tagName() == "zeroOrMore" ) p.zeroOrMore = true;
          else if ( e1.tagName() == "oneOrMore" ) p.oneOrMore = true;
          else if ( e1.tagName() == "choice" ) p.choice = true;
          else {
            kdDebug() << "Unsupported pattern '" << e1.tagName() << "'" << endl;
          }
          parseElement( e1, e, p );
        }
      }

      return true;
    }

    void substituteReferences( Element *s )
    {
      kdDebug() << "substituteReferences for '" << s->name << "'" << endl;
      QValueList<Reference *>::Iterator it = s->references.begin();
      while( it != s->references.end() ) {
        Reference *r = *it;
        kdDebug() << "REF " << r->name << endl;
        if ( r->name == s->name ) {
          kdDebug() << "Don't resolve self reference" << endl;
          return;
        }
        QMap<QString,QValueList<Element *> >::ConstIterator it1;
        it1 = mDefinitionMap.find( r->name );
        if ( it1 != mDefinitionMap.end() ) {
          QValueList<Element *> elements = *it1;
          QValueList<Element *>::ConstIterator it4;
          for( it4 = elements.begin(); it4 != elements.end(); ++it4 ) {
            Element *d = *it4;
            substituteReferences( d );
            QValueList<Element *>::ConstIterator it2;
            for( it2 = d->elements.begin(); it2 != d->elements.end(); ++it2 ) {
              Element *e = *it2;
              e->pattern.merge( r->pattern );
              substituteReferences( e );
              s->elements.append( e );
            }
            QValueList<Attribute *>::ConstIterator it3;
            for( it3 = d->attributes.begin(); it3 != d->attributes.end();
                 ++it3 ) {
              Attribute *a = *it3;
              a->pattern.merge( r->pattern );
              s->attributes.append( a );
            }
          }
          it = s->references.erase( it );
        } else {
          kdDebug() << "Reference not found" << endl;
          ++it;
        }
      }
    }

    void doIndent( int cols )
    {
      for( int i = 0; i < cols; ++i ) std::cout << " ";
    }

    void dumpPattern( Pattern pattern )
    {
      std::cout << pattern.asString().utf8();
    }

    void dumpReferences( const QValueList<Reference *> &references, int indent )
    {
      QValueList<Reference *>::ConstIterator it;
      for( it = references.begin(); it != references.end(); ++it ) {
        Reference *r = *it;
        doIndent( indent );
        std::cout << "REFERENCE " << r->name.utf8();
        dumpPattern( r->pattern );
        std::cout << std::endl;
      }
    }

    void dumpAttributes( const QValueList<Attribute *> &attributes, int indent )
    {
      QValueList<Attribute *>::ConstIterator it;
      for( it = attributes.begin(); it != attributes.end(); ++it ) {
        Attribute *a = *it;
        doIndent( indent );
        std::cout << "ATTRIBUTE " << a->name.utf8();
        dumpPattern( a->pattern );
        std::cout << std::endl;
      }
    }

    void dumpElements( const QValueList<Element *> &elements, int indent )
    {
      QValueList<Element *>::ConstIterator it;
      for( it = elements.begin(); it != elements.end(); ++it ) {
        Element *e = *it;
        doIndent( indent );
        std::cout << "ELEMENT " << e->name.utf8();
        dumpPattern( e->pattern );
        std::cout << std::endl;
        
        if ( e->hasText ) {
          doIndent( indent + 2 );
          std::cout << "TEXT" << std::endl;
        }
        
        dumpAttributes( e->attributes, indent + 2 );
        dumpElements( e->elements, indent + 2 );
        dumpReferences( e->references, indent + 2 );
      }
    }

    void dumpTree( Element *s )
    {
      std::cout << "START " << s->name.utf8() << std::endl;
      dumpElements( s->elements, 2 );
      dumpReferences( s->references, 2 );
    }

  private:
    QMap<QString,QValueList<Element *> > mDefinitionMap;
};


class Creator
{
  public:
    Creator()
    {
    }

    KODE::File &file() { return mFile; }

    QString upperFirst( const QString &str )
    {
      if ( str.isEmpty() ) return str;
      return str[ 0 ].upper() + str.mid( 1 );
    }
    
    QString lowerFirst( const QString &str )
    {
      if ( str.isEmpty() ) return str;
      return str[ 0 ].lower() + str.mid( 1 );
    }
    
    void createProperty( KODE::Class &c, const QString &type,
                         const QString &name )
    {
      KODE::MemberVariable v( name, type );
      c.addMemberVariable( v );

      KODE::Function mutator( "set" + upperFirst( name ), "void" );
      mutator.addArgument( "const " + type + " &v" );
      mutator.addBodyLine( v.name() + " = v;" );
      c.addFunction( mutator );

      KODE::Function accessor( name, type );
      accessor.setConst( true );
      accessor.addBodyLine( "return " + v.name() + ";" );
      c.addFunction( accessor );
    }

    void createElementFunctions( KODE::Class &c, Element *e )
    {
      if ( e->hasText ) {
        createProperty( c, "QString", e->name );
        return;
      }
    
      QString type = upperFirst( e->name );

      if ( !mFile.hasClass( type ) ) {
        createClass( e );
      }

      QString name = lowerFirst( e->name );

      if ( e->pattern.oneOrMore || e->pattern.zeroOrMore ) {
        registerListTypedef( type );

        c.addInclude( "qvaluelist.h" );
        type = type + "::List";
        QString className = upperFirst( name );
        name = name + "List";

        KODE::Function adder( "add" + className, "void" );
        adder.addArgument( className + " *v" );

        KODE::Code code;
        code += "m" + upperFirst( name ) + ".append( v );";
        
        adder.setBody( code );

        c.addFunction( adder );
      }

      createProperty( c, type, name );
    }
    
    void createClass( Element *element )
    {
      QString className = upperFirst( element->name );

      if ( mProcessedClasses.find( className ) != mProcessedClasses.end() ) {
        return;
      }

      KODE::Class c( className );
      
      mProcessedClasses.append( className );

      QValueList<Attribute *>::ConstIterator itA;
      for( itA = element->attributes.begin();
           itA != element->attributes.end(); ++itA ) {
        Attribute *a = *itA;
        
        createProperty( c, "QString", a->name );
      }
      
      QValueList<Element *>::ConstIterator itE;
      for( itE = element->elements.begin(); itE != element->elements.end();
           ++itE ) {
        createElementFunctions( c, *itE );
      }

      QValueList<Reference *>::ConstIterator itR;
      for( itR = element->references.begin(); itR != element->references.end();
           ++itR ) {
        Element e;
        e.name = (*itR)->name;
        e.pattern = (*itR)->pattern;
        createElementFunctions( c, &e );
      }

      createElementParser( c, element );

      mFile.insertClass( c );
    }

    void createElementParser( KODE::Class &c, Element *e )
    {
      KODE::Function parser( "parseElement", c.name() + " *" );
      parser.setStatic( true );
      parser.setDocs( "Parse XML object from DOM element." );

      parser.addArgument( "const QDomElement &element" );
      
      KODE::Code code;

      code += "if ( element.tagName() != \"" + e->name + "\" ) {";
      code.indent();
      code += "kdError() << \"Expected '" + e->name + "', got '\" << " +
              "element.tagName() << \"'.\" << endl;";
      code += "return 0;";
      code.unindent();
      code += "}";
      code.newLine();

      code += c.name() + " *result = new " + c.name() + "();";
      code.newLine();

      code += "QDomNode n;";
      code += "for( n = element.firstChild(); !n.isNull();"
                  " n = n.nextSibling() ) {";
      code.indent();
      code += "QDomElement e = n.toElement();";
      
      QValueList<Element *>::ConstIterator it;
      for( it = e->elements.begin(); it != e->elements.end(); ++it ) {
        QString condition;
        if ( it != e->elements.begin() ) condition = "else ";
        condition += "if";

        code += condition + " ( e.tagName() == \"" + (*it)->name + "\" ) {";
        code.indent();

        QString className = upperFirst( (*it)->name );
        
        if ( (*it)->hasText ) {
          code += "result->set" + className + "( e.text() );"; 
        } else {
          code += className + " *o = " + className + "::parseElement( e );";
          code += "if ( o ) result->add" + className + "( o );";
        }

        code.unindent();
        code += "}";
      }

      code.newLine();

      QValueList<Reference *>::ConstIterator it3;
      for( it3 = e->references.begin(); it3 != e->references.end(); ++it3 ) {
        QString condition;
        if ( it3 != e->references.begin() ) condition = "else ";
        condition += "if";

        code += condition + " ( e.tagName() == \"" + (*it3)->name + "\" ) {";
        code.indent();

        QString className = upperFirst( (*it3)->name );
        
        code += className + " *o = " + className + "::parseElement( e );";
        code += "if ( o ) result->add" + className + "( o );";

        code.unindent();
        code += "}";
      }

      code.unindent();
      code += "}";
      code.newLine();

      QValueList<Attribute *>::ConstIterator it2;
      for( it2 = e->attributes.begin(); it2 != e->attributes.end(); ++it2 ) {
        code += "result->set" + upperFirst( (*it2)->name ) +
                "( element.attribute( \"" + (*it2)->name + "\" ) );";
      }
      code.newLine();
      
      code += "return result;";

      parser.setBody( code );

      c.addFunction( parser );
    }

    void registerListTypedef( const QString &type )
    {
      if ( !mListTypedefs.contains( type ) ) mListTypedefs.append( type );
    }

    void createListTypedefs()
    {
      QStringList::ConstIterator it;
      for( it = mListTypedefs.begin(); it != mListTypedefs.end(); ++it ) {
        KODE::Class c = mFile.findClass( *it );
        if ( !c.isValid() ) continue;
        c.addTypedef( KODE::Typedef( "QValueList<" + *it + " *>", "List" ) );
        mFile.insertClass( c );
      }
    }

    void createFileParser( Element *element )
    {
      QString className = upperFirst( element->name );

      KODE::Class c = mFile.findClass( className );

      KODE::Function parser( "parseFile", className + " *" );
      parser.setStatic( true );

      parser.addArgument( "const QString &filename" );

      c.addInclude( "qfile.h" );
      c.addInclude( "qdom.h" );
      c.addInclude( "kdebug.h" );

      KODE::Code code;
      
      code += "QFile file( filename );";
      code += "if ( !file.open( IO_ReadOnly ) ) {";
      code += "  kdError() << \"Unable to open file '\" << filename << \"'\" << endl;";
      code += "  return 0;";
      code += "}";
      code += "";
      code += "QString errorMsg;";
      code += "int errorLine, errorCol;";
      code += "QDomDocument doc;";
      code += "if ( !doc.setContent( &file, false, &errorMsg, &errorLine, &errorCol ) ) {";
      code += "  kdError() << errorMsg << \" at \" << errorLine << \",\" << errorCol << endl;";
      code += "  return 0;";
      code += "}";
      code += "";
      code += "kdDebug() << \"CONTENT:\" << doc.toString() << endl;";

      code += "";
      code += className + " *c = parseElement( doc.documentElement() );";
      code += "return c;";

      parser.setBody( code );

      c.addFunction( parser );

      mFile.insertClass( c );
    }

  private:
    KODE::File mFile;
    QStringList mProcessedClasses;
    QStringList mListTypedefs;    
};

static const KCmdLineOptions options[] =
{
  { "d", 0, 0 },
  { "directory <dir>", I18N_NOOP("Directory to generate files in"), "." },
  { "+dtd", I18N_NOOP("DTD of XML file"), 0 },
  KCmdLineLastOption
};

int main( int argc, char **argv )
{
  KAboutData aboutData( "kxml_compiler", I18N_NOOP("KDE xml compiler"), "0.1",
  	I18N_NOOP("KDE XML Compiler") , KAboutData::License_LGPL );
  aboutData.addAuthor( "Cornelius Schumacher", 0, "schumacher@kde.org" );

  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options );

  KInstance app( &aboutData );

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if ( args->count() < 1 ) {
    kdError() << "Too few arguments." << endl;
    return 1;
  }
  if ( args->count() > 1 ) {
    kdError() << "Too many arguments." << endl;
    return 1;
  }

  QString baseDir = QFile::decodeName( args->getOption( "directory" ) );
  if ( !baseDir.endsWith( "/" ) ) baseDir.append( "/" );

  QString dtdFilename = args->url( 0 ).path();

  QString baseName = args->url( 0 ).fileName();
  int pos = baseName.findRev( '.' );
  if ( pos > 0 ) baseName = baseName.left( pos );


  QFile dtdFile( dtdFilename );
  if ( !dtdFile.open( IO_ReadOnly ) ) {
    kdError() << "Unable to open '" << dtdFilename << "'" << endl;
    return 1;
  }

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &dtdFile, false, &errorMsg, &errorLine, &errorCol ) ) {
    kdError() << errorMsg << " at " << errorLine << "," << errorCol << endl;
    return 1;
  }

  kdDebug() << "Begin parsing" << endl;

  Parser p;
  Element *start = p.parse( doc.documentElement() );
  if ( !start ) {
    kdError() << "Could not find start element" << endl;
    return 1;
  }

  p.substituteReferences( start );
  
  std::cout << "--- TREE:" << std::endl;
  p.dumpTree( start );


  kdDebug() << "Begin creating code" << endl;
  Creator c;

  kdDebug() << "Create classes" << endl;
  QValueList<Element *>::ConstIterator it;
  for( it = start->elements.begin(); it != start->elements.end(); ++it ) {
    c.createClass( *it );
  }
  kdDebug() << "Create parser" << endl;
  for( it = start->elements.begin(); it != start->elements.end(); ++it ) {
    c.createFileParser( *it );
  }

  c.createListTypedefs();

#if 0
  QValueList<Reference *>::ConstIterator it2;
  for( it2 = start->references.begin(); it2 != start->references.end();
       ++it2 ) {
    Element e;
    e.name = (*it2)->name;
    e.pattern = (*it2)->pattern;
    c.createClass( &e );
  }
#endif

  kdDebug() << "Begin printing code" << endl;

  KODE::File f = c.file();
  f.setFilename( baseName );

  KODE::Printer printer;
  printer.setCreationWarning( true );
  printer.setGenerator( "kxml_compiler" );
  printer.setOutputDirectory( baseDir );
  printer.setSourceFile( args->url( 0 ).fileName() );

  kdDebug() << "Print header" << endl;
  printer.printHeader( f );

  kdDebug() << "Print implementation" << endl;
  printer.printImplementation( f );

  kdDebug() << "Finished." << endl;
}
