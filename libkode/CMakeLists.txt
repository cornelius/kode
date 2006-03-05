kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kode_LIB_SRCS 
   code.cpp 
   enum.cpp 
   style.cpp 
   printer.cpp 
   license.cpp 
   file.cpp 
   class.cpp 
   function.cpp 
   variable.cpp 
   membervariable.cpp 
   typedef.cpp 
   statemachine.cpp 
   automakefile.cpp )

kde4_automoc(${kode_LIB_SRCS})

kde4_add_library(kode SHARED ${kode_LIB_SRCS})

target_link_libraries(kode  ${QT_AND_KDECORE_LIBS} kdecore )

set_target_properties(kode PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} kode )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir) $(all_includes)
#KDE_CXXFLAGS = -UQT3_SUPPORT
#
#lib_LTLIBRARIES = libkode.la
#
#libkode_la_SOURCES = code.cpp enum.cpp style.cpp printer.cpp license.cpp file.cpp \
#                     class.cpp \
#                     function.cpp variable.cpp membervariable.cpp \
#                     typedef.cpp statemachine.cpp automakefile.cpp
#libkode_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) \
#                     -version-info 1:0:0 -no-undefined
#libkode_la_LIBADD = -lkdecore
#
#METASOURCES = AUTO
