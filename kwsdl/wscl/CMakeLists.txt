kde4_header()

include_directories( ${CMAKE_SOURCE_DIR}/libkdepim ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(wscl_LIB_SRCS 
   conversation.cpp 
   documents.cpp 
   interaction.cpp 
   parser.cpp 
   transition.cpp )

kde4_automoc(${wscl_LIB_SRCS})

kde4_add_library(wscl SHARED ${wscl_LIB_SRCS})

target_link_libraries(wscl  ${QT_AND_KDECORE_LIBS} )

set_target_properties(wscl PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} wscl )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir) -I.. -I$(top_srcdir)/libkdepim $(all_includes)
#KDE_CXXFLAGS = -UQT3_SUPPORT
#lib_LTLIBRARIES = libwscl.la
#
#libwscl_la_SOURCES = conversation.cpp documents.cpp interaction.cpp parser.cpp transition.cpp
#libwscl_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
#libwscl_la_LIBADD =  $(LIB_QTCORE) $(LIB_QTXML)
#
##bin_PROGRAMS = wscltest
##wscltest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
##wscltest_SOURCES = main.cpp
##wscltest_LDADD = libwscl.la
#
#METASOURCES = AUTO
