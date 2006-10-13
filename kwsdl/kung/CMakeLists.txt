
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

kde4_automoc(kschemawidgets ${kschemawidgets_LIB_SRCS})

kde4_add_library(kschemawidgets SHARED ${kschemawidgets_LIB_SRCS})

# FIXME: kwsdl only needed for pageinputfield!
target_link_libraries(kschemawidgets  ${KDE4_KDECORE_LIBS} ${KDE4_KPARTS_LIBS} schema wsdl )

# FIXME: Versioning like libtool?
set_target_properties(kschemawidgets PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install(TARGETS kschemawidgets  DESTINATION ${LIB_INSTALL_DIR})

########### next target ###############

set(kung_SRCS 
   dispatcher.cpp 
   generalconversationmanager.cpp 
   loader.cpp 
   main.cpp 
   transport.cpp 
   wsclconversationmanager.cpp )

kde4_automoc(kung ${kung_SRCS})

kde4_add_executable(kung ${kung_SRCS})

target_link_libraries(kung  ${KDE4_KDECORE_LIBS} kschemawidgets wscl wsdl )

install(TARGETS kung  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############





#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir) -I.. -I$(top_srcdir)/kode -I$(top_srcdir)/kode/kwsdl -I$(top_srcdir)/libkdepim $(all_includes)
#
#bin_PROGRAMS = kung
#kung_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#kung_SOURCES = binaryinputfield.cpp boolinputfield.cpp complexbaseinputfield.cpp dateinputfield.cpp dispatcher.cpp \
#               doubleinputfield.cpp enuminputfield.cpp generalconversationmanager.cpp \
#               inputdialog.cpp inputfield.cpp inputfieldfactory.cpp \
#               integerinputfield.cpp listinputfield.cpp loader.cpp main.cpp outputdialog.cpp pageinputfield.cpp simplebaseinputfield.cpp \
#               stringinputfield.cpp timeinputfield.cpp transport.cpp wsclconversationmanager.cpp
#kung_LDADD = ../../schema/libschema.la ../wscl/libwscl.la ../wsdl/libwsdl.la -lkparts
#
#METASOURCES = AUTO
