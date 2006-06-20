
include_directories( ${CMAKE_SOURCE_DIR}/kode  )


########### next target ###############

set(kxmlcommon_LIB_SRCS 
   fileprovider.cpp 
   messagehandler.cpp 
   nsmanager.cpp 
   parsercontext.cpp 
   qname.cpp )

kde4_automoc(${kxmlcommon_LIB_SRCS})

kde4_add_library(kxmlcommon SHARED ${kxmlcommon_LIB_SRCS})

target_link_libraries(kxmlcommon  ${KDE4_KIO_LIBS} )

set_target_properties(kxmlcommon PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} kxmlcommon )
