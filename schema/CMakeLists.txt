
include_directories( ${CMAKE_SOURCE_DIR}/kode  )


########### next target ###############

set(schema_LIB_SRCS 
   annotation.cpp
   attribute.cpp 
   complextype.cpp 
   element.cpp 
   parser.cpp 
   simpletype.cpp 
   types.cpp 
   xmlelement.cpp 
   xsdtype.cpp 
   attributegroup.cpp 
	compositor.cpp)

kde4_automoc(${schema_LIB_SRCS})

kde4_add_library(schema SHARED ${schema_LIB_SRCS})

target_link_libraries(schema  ${KDE4_KIO_LIBS} kxmlcommon )

set_target_properties(schema PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install(TARGETS schema  DESTINATION ${LIB_INSTALL_DIR})


########### next target ###############

set(schematest_SRCS schematest.cpp )

kde4_automoc(${schematest_SRCS})

kde4_add_executable(schematest ${schematest_SRCS})

target_link_libraries(schematest  ${KDE4_KDECORE_LIBS} schema )

install(TARGETS schematest  DESTINATION ${BIN_INSTALL_DIR} )
