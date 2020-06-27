set(KXML_COMPILER_SOURCES
	classdescription.cpp
	creator.cpp
	parsercreatordom.cpp
	parserrelaxng.cpp
	parserxsd.cpp
	parserxml.cpp
	schema.cpp
	writercreator.cpp
)

set(KXML_COMPILER_HEADERS
	classdescription.h
	creator.h
	parsercreatordom.h
	parserrelaxng.h
	parserxsd.h
	parserxml.h
	schema.h
	writercreator.h
)

add_library(libkxml_compiler OBJECT ${KXML_COMPILER_SOURCES} ${KXML_COMPILER_HEADERS})
target_link_libraries(libkxml_compiler Qt5::Xml)
add_executable(kxml_compiler kxml_compiler.cpp)

target_link_libraries(kxml_compiler
	Qt5::Xml
	kode
	libkxml_compiler
	xmlschema
	xmlcommon
)

install(
	TARGETS kxml_compiler
	RUNTIME DESTINATION bin
)
if(BUILD_TESTING)
	add_subdirectory(tests)
endif(BUILD_TESTING)
