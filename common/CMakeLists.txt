
include_directories( ${CMAKE_SOURCE_DIR}/kode  )


########### next target ###############

set(kxmlcommon_LIB_SRCS 
   fileprovider.cpp 
   messagehandler.cpp 
   nsmanager.cpp 
   parsercontext.cpp 
   qname.cpp )


kde4_add_library(kxmlcommon SHARED ${kxmlcommon_LIB_SRCS})

target_link_libraries(kxmlcommon  ${KDE4_KIO_LIBS} )


set_target_properties(kxmlcommon PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} ${KDE4_DISABLE_PROPERTY_}LINK_INTERFACE_LIBRARIES "${QT_QTXML_LIBRARY};${QT_QTCORE_LIBRARY}" )
install(TARGETS kxmlcommon  DESTINATION ${LIB_INSTALL_DIR})
