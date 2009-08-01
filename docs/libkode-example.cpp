// Define file and meta information
KODE::File file;
file.setProject( "Example Project" );
file.addCopyright( QDate::currentDate().year(),
  "Cornelius Schumacher", "schumacher@kde.org" );
file.setLicense( KODE::License( Kode::License::GPL ) );

// Setup class definition
KODE::Class c( "Hello" );
c.setDocs( "This class implements a hello world example." );

// Define function
KODE::Function sayHello( "sayHello", "void" );
sayHello.addArgument( "const QString &to" );

// Define body of function
KODE::Code code;
code += "cout << \"Hello, \" << to << \"!\"";
sayHello.setBody( code );

// Add function to class
c.addFunction( sayHello );

// Generate code
KODE::Printer p;
p.printHeader( file );
p.printImplementation( file );
