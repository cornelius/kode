kde4_header()

include_directories( ${CMAKE_SOURCE_DIR}/kode ${CMAKE_SOURCE_DIR}/kode/kwsdl ${CMAKE_SOURCE_DIR}/libkdepim ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kung_SRCS 
   binaryinputfield.cpp 
   boolinputfield.cpp 
   complexbaseinputfield.cpp 
   dateinputfield.cpp 
   dispatcher.cpp 
   doubleinputfield.cpp 
   enuminputfield.cpp 
   generalconversationmanager.cpp 
   inputdialog.cpp 
   inputfield.cpp 
   inputfieldfactory.cpp 
   integerinputfield.cpp 
   listinputfield.cpp 
   loader.cpp 
   main.cpp 
   outputdialog.cpp 
   pageinputfield.cpp 
   simplebaseinputfield.cpp 
   stringinputfield.cpp 
   timeinputfield.cpp 
   transport.cpp 
   wsclconversationmanager.cpp )

kde4_automoc(${kung_SRCS})

kde4_add_executable(kung ${kung_SRCS})

target_link_libraries(kung  ${QT_AND_KDECORE_LIBS} schema wscl wsdl kparts )

install_targets(/bin kung )


########### install files ###############


kde4_footer()



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
