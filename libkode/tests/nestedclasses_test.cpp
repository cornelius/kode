#include <libkode/class.h>
#include <libkode/file.h>
#include <libkode/printer.h>

#include <QtCore/QLatin1String>
#include <QtCore/QString>

int main(int argc, char **argv)
{
	KODE::Class mainClass( QLatin1String("MainClass") );
	KODE::Class nestedClass( QLatin1String("NestedClass") );
	nestedClass.setUseDPointer(true);

	// Create MainClass functions
	KODE::Function mainFunction1( QLatin1String("mainFunction1"), QLatin1String("void") );
	mainClass.addFunction( mainFunction1 );

	// Create NestedClass functions
	KODE::Function nestedFunction1( QLatin1String("nestedFunction1"), QLatin1String("void") );
	nestedClass.addFunction( nestedFunction1 );

	KODE::MemberVariable nestedMember1( QLatin1String("name"), QLatin1String("QString") );
	nestedClass.addMemberVariable( nestedMember1 );

	// Add NestedClass to MainClass
	mainClass.addNestedClass( nestedClass );

	KODE::Printer printer;
	KODE::File file;
	
	file.setFilename( QLatin1String("generatednestedclass") );
	
	file.insertClass( mainClass );
	
	printer.printHeader( file );
	printer.printImplementation( file );

	return 0;
}
