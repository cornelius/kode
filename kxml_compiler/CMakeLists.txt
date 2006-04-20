kde4_header()

add_subdirectory( tests ) 

include_directories( ${CMAKE_SOURCE_DIR}/kode/libkode ${CMAKE_SOURCE_DIR}/kode ${KDE4_INCLUDES} )


########### next target ###############

set(kxml_compiler_SRCS 
	creator.cpp 
	kxml_compiler.cpp 
	parsercreatordom.cpp 
	parsercreatorcustom.cpp)

kde4_automoc(${kxml_compiler_SRCS})

kde4_add_executable(kxml_compiler NOGUI ${kxml_compiler_SRCS})

target_link_libraries(kxml_compiler  ${KDE4_KDECORE_LIBS} kode kschema )

install_targets(/bin kxml_compiler )


########### next target ###############

########### next target ###############

set(kschema_LIB_SRCS
	parserrelaxng.cpp 
	parserxsd.cpp 
	schema.cpp
	)

kde4_automoc(${kschema_LIB_SRCS})

kde4_add_library(kschema SHARED ${kschema_LIB_SRCS})

target_link_libraries(kschema ${KDE4_KDECORE_LIBS} schema )

set_target_properties(kschema PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} kschema )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = tests
#
#INCLUDES = -I$(top_srcdir)/kode $(all_includes)
#KDE_CXXFLAGS = -UQT3_SUPPORT
#
#bin_PROGRAMS = kxml_compiler
#
#kxml_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#kxml_compiler_LDADD   = ../libkode/libkode.la -lkdecore
#kxml_compiler_SOURCES = parserrelaxng.cpp creator.cpp kxml_compiler.cpp
#
#METASOURCES = AUTO
