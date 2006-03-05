kde4_header()

include_directories( ${CMAKE_SOURCE_DIR}/kode ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kxmlcommon_LIB_SRCS 
   fileprovider.cpp 
   messagehandler.cpp 
   nsmanager.cpp 
   parsercontext.cpp 
   qname.cpp )

kde4_automoc(${kxmlcommon_LIB_SRCS})

kde4_add_library(kxmlcommon SHARED ${kxmlcommon_LIB_SRCS})

target_link_libraries(kxmlcommon  ${QT_AND_KDECORE_LIBS} kdecore kio )

set_target_properties(kxmlcommon PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} kxmlcommon )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/kode $(all_includes)
#
#lib_LTLIBRARIES = libkxmlcommon.la
#KDE_CXXFLAGS = -UQT3_SUPPORT
#
#libkxmlcommon_la_SOURCES = fileprovider.cpp messagehandler.cpp nsmanager.cpp parsercontext.cpp qname.cpp
#libkxmlcommon_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
#libkxmlcommon_la_LIBADD = $(LIB_QTCORE) -lkdecore -lkio
#
#METASOURCES = AUTO
