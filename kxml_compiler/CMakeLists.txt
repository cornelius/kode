set(KSCHEMA_SOURCES
	parserrelaxng.cpp
	parserxsd.cpp
	parserxml.cpp
	schema.cpp
)
set(KSCHEMA_HEADERS
	parserrelaxng.h
	parserxsd.h
	parserxml.h
	schema.h
)

add_library(kschema SHARED ${KSCHEMA_SOURCES} ${KSCHEMA_HEADERS})
target_link_libraries(kschema
	Qt5::Xml
	xmlschema
	xmlcommon
)

#set_target_properties(kschema PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})

install(
	TARGETS kschema
	LIBRARY DESTINATION lib
)

########### next target ###############

set(KXML_COMPILER_SOURCES
	classdescription.cpp
	creator.cpp
	kxml_compiler.cpp
	parsercreatordom.cpp
	writercreator.cpp
)

set(KXML_COMPILER_HEADERS
	classdescription.h
	creator.h
	parsercreatordom.h
	writercreator.h
)

add_executable(kxml_compiler ${KXML_COMPILER_SOURCES} ${KXML_COMPILER_HEADERS})

target_link_libraries(kxml_compiler
	kode
	kschema
)

install(
	TARGETS kxml_compiler
	RUNTIME DESTINATION bin
)

#add_subdirectory(tests)
