kde4_header()

add_subdirectory( tests ) 

include_directories( ${CMAKE_SOURCE_DIR}/kode ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kxml_compiler_SRCS parserrelaxng.cpp creator.cpp kxml_compiler.cpp )

kde4_automoc(${kxml_compiler_SRCS})

kde4_add_executable(kxml_compiler ${kxml_compiler_SRCS})

target_link_libraries(kxml_compiler  ${QT_AND_KDECORE_LIBS} kode kdecore )

install_targets(/bin kxml_compiler )


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
