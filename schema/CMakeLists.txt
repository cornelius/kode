include_directories( ${kode_SOURCE_DIR} )

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


add_library( schema SHARED ${schema_LIB_SRCS} )

target_link_libraries( schema kxmlcommon )
target_link_libraries( schema LINK_INTERFACE_LIBRARIES kxmlcommon )

set_target_properties( schema PROPERTIES VERSION ${GENERIC_LIB_VERSION}
                                        SOVERSION ${GENERIC_LIB_SOVERSION} )

install( 
    TARGETS schema  ${INSTALL_TARGETS_DEFAULT_ARGS}
    LIBRARY DESTINATION lib  # <-- Add this line
)

########### next target ###############

set( schematest_SRCS schematest.cpp )

add_executable( schematest ${schematest_SRCS} )
qt5_use_modules(schematest Core)
qt5_use_modules(schematest Xml)
qt5_use_modules(schematest Network)

target_link_libraries(schematest Qt5::Core)
target_link_libraries(schematest Qt5::Xml)
target_link_libraries(schematest Qt5::Network)

target_link_libraries( schematest schema )

install( 
    TARGETS schematest ${INSTALL_TARGETS_DEFAULT_ARGS} 
    RUNTIME DESTINATION bin
)
