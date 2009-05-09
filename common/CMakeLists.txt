########### next target ###############
set( kxmlcommon_LIB_SRCS
   fileprovider.cpp
   messagehandler.cpp
   nsmanager.cpp
   parsercontext.cpp
   qname.cpp
)


kde4_add_library( kxmlcommon SHARED ${kxmlcommon_LIB_SRCS} )

target_link_libraries( kxmlcommon  ${KDE4_KIO_LIBS} ${QT_QTXML_LIBRARY} )
target_link_libraries( kxmlcommon  LINK_INTERFACE_LIBRARIES ${QT_QTXML_LIBRARY} )

set_target_properties( kxmlcommon PROPERTIES VERSION ${GENERIC_LIB_VERSION}
                                            SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS kxmlcommon  ${INSTALL_TARGETS_DEFAULT_ARGS} )
