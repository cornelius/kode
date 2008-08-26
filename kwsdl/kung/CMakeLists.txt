
include_directories( ${CMAKE_SOURCE_DIR}/kode ${CMAKE_SOURCE_DIR}/kode/kwsdl ${CMAKE_SOURCE_DIR}/libkdepim  )

########### next target ###############

set(kschemawidgets_LIB_SRCS 
   binaryinputfield.cpp 
   boolinputfield.cpp 
   complexbaseinputfield.cpp 
   dateinputfield.cpp 
   doubleinputfield.cpp 
   enuminputfield.cpp 
   inputdialog.cpp 
   inputfield.cpp 
   inputfieldfactory.cpp 
   integerinputfield.cpp 
   listinputfield.cpp 
   pageinputfield.cpp 
   outputdialog.cpp 
   simplebaseinputfield.cpp 
   stringinputfield.cpp 
   timeinputfield.cpp )


kde4_add_library(kschemawidgets SHARED ${kschemawidgets_LIB_SRCS})

# FIXME: kwsdl only needed for pageinputfield!
target_link_libraries(kschemawidgets  ${KDE4_KPARTS_LIBS} ${KDE4_KDE3SUPPORT_LIBRARY} schema wsdl kxmlcommon )

# FIXME: Versioning like libtool?
set_target_properties(kschemawidgets PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install(TARGETS kschemawidgets  ${INSTALL_TARGETS_DEFAULT_ARGS})

########### next target ###############

set(kung_SRCS 
   dispatcher.cpp 
   generalconversationmanager.cpp 
   loader.cpp 
   main.cpp 
   transport.cpp 
   wsclconversationmanager.cpp )


kde4_add_executable(kung ${kung_SRCS})

target_link_libraries(kung  ${KDE4_KIO_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KDECORE_LIBS} kschemawidgets schema wscl wsdl kxmlcommon )

install(TARGETS kung  ${INSTALL_TARGETS_DEFAULT_ARGS} )
