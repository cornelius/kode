set(KXML_COMPILER_SOURCES
	classdescription.cpp
	creator.cpp
	kxml_compiler.cpp
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

add_executable(kxml_compiler ${KXML_COMPILER_SOURCES} ${KXML_COMPILER_HEADERS})

target_link_libraries(kxml_compiler
	Qt5::Xml
	kode
	xmlschema
	xmlcommon
)

install(
	TARGETS kxml_compiler
	RUNTIME DESTINATION bin
)

#add_subdirectory(tests)
