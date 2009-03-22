
add_subdirectory( tests )

########### next target ###############

set(kschema_LIB_SRCS
	parserrelaxng.cpp
	parserxsd.cpp
        parserxml.cpp
	schema.cpp
	)


kde4_add_library(kschema SHARED ${kschema_LIB_SRCS})

target_link_libraries(kschema ${KDE4_KDECORE_LIBS} schema kxmlcommon ${QT_QTXML_LIBRARY})
target_link_libraries(kschema LINK_INTERFACE_LIBRARIES schema kxmlcommon )

set_target_properties(kschema PROPERTIES VERSION ${GENERIC_LIB_VERSION} 
                                         SOVERSION ${GENERIC_LIB_SOVERSION})

install(TARGETS kschema  ${INSTALL_TARGETS_DEFAULT_ARGS} )

########### next target ###############

set(kxml_compiler_SRCS 
	creator.cpp 
	kxml_compiler.cpp 
	parsercreatordom.cpp 
	parsercreatorcustom.cpp)


kde4_add_executable(kxml_compiler NOGUI RUN_UNINSTALLED ${kxml_compiler_SRCS})

target_link_libraries(kxml_compiler  ${KDE4_KDECORE_LIBS} kode kschema )

install(TARGETS kxml_compiler  ${INSTALL_TARGETS_DEFAULT_ARGS} )

