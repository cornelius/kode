
include_directories( ${CMAKE_SOURCE_DIR}/libkdepim  )


########### next target ###############

set(wscl_LIB_SRCS 
   conversation.cpp 
   documents.cpp 
   interaction.cpp 
   parser.cpp 
   transition.cpp )


kde4_add_library(wscl SHARED ${wscl_LIB_SRCS})

target_link_libraries(wscl ${KDE4_KDECORE_LIBS} ${QT_QTXML_LIBRARY} )

set_target_properties(wscl PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS wscl ${INSTALL_TARGETS_DEFAULT_ARGS})


