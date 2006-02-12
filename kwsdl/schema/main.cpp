#include <qfile.h>
#include <qxml.h>

#include <schema/parser.h>

#include <common/nsmanager.h>
#include <common/messagehandler.h>
#include <common/parsercontext.h>

int main( int argc, char **argv )
{
  if ( argc != 2 ) {
    qDebug( "Missing argument: filename of schema" );
    return 1;
  }

  QString filename = argv[ 1 ];

  QFile file( filename );

  if ( !file.open( QIODevice::ReadOnly ) ) {
    qDebug( "Can't open file %s", qPrintable( file.fileName() ) );
    return 1;
  }

  QXmlInputSource source( &file );
  QXmlSimpleReader reader;
  reader.setFeature( "http://xml.org/sax/features/namespace-prefixes", true );

  QDomDocument document( "KWSDL" );

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &source, &reader, &errorMsg, &errorLine, &errorCol ) ) {
    qDebug( "%s at (%d,%d)", qPrintable( errorMsg ), errorLine, errorCol );
    return 2;
  }

  NSManager namespaceManager;
  MessageHandler messageHandler;
  ParserContext context;
  context.setNamespaceManager( &namespaceManager );
  context.setMessageHandler( &messageHandler );

  QDomElement element = doc.documentElement();
  if ( element.tagName() != "xs:schema" ) {
    qDebug( "document element is '%s'", qPrintable( element.tagName() ) );
    return 3;
  }

  Schema::Parser parser;
  parser.parseSchemaTag( &context, element );

  Schema::Types types = parser.types();

  const Schema::SimpleType::List simpleTypes = types.simpleTypes();
  for ( int i = 0; i < simpleTypes.count(); ++i ) {
    qDebug( "SimpleType: %s %s", qPrintable( simpleTypes[ i ].name() ), qPrintable( simpleTypes[ i ].baseTypeName().qname() ) );
  }

  const Schema::ComplexType::List complexTypes = types.complexTypes();
  for ( int i = 0; i < complexTypes.count(); ++i ) {
    qDebug( "ComplexType: %s %s", qPrintable( complexTypes[ i ].name() ), qPrintable( complexTypes[ i ].baseTypeName().qname() ) );
    const Schema::Element::List elements = complexTypes[ i ].elements();
    for ( int j = 0; j < elements.count(); ++j ) {
      qDebug( "\tElement: %s %s", qPrintable( elements[ j ].name() ), qPrintable( elements[ j ].type().qname() ) );
    }
    const Schema::Attribute::List attributes = complexTypes[ i ].attributes();
    for ( int j = 0; j < attributes.count(); ++j ) {
      qDebug( "\tAttribute: %s %s", qPrintable( attributes[ j ].name() ), qPrintable( attributes[ j ].type().qname() ) );
    }
  }

  const Schema::Element::List elements = types.elements();
  for ( int i = 0; i < elements.count(); ++i ) {
    qDebug( "Element: %s %s", qPrintable( elements[ i ].name() ), qPrintable( elements[ i ].type().qname() ) );
  }

  const Schema::Attribute::List attributes = types.attributes();
  for ( int i = 0; i < attributes.count(); ++i ) {
    qDebug( "Attribute: %s %s", qPrintable( attributes[ i ].name() ), qPrintable( attributes[ i ].type().qname() ) );
  }
  
  const Schema::AttributeGroup::List attributeGroups = types.attributeGroups();
  for ( int i = 0; i < attributeGroups.count(); ++i ) {
    qDebug( "AttributeGroup: %s", qPrintable( attributeGroups[ i ].name() ) );
  }

  return 0;
}
