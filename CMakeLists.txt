kde4_header()

add_subdirectory( libkode ) 
add_subdirectory( common ) 
add_subdirectory( schema ) 
add_subdirectory( kxml_compiler ) 

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kode_SRCS kodemain.cpp )

kde4_automoc(${kode_SRCS})

kde4_add_executable(kode ${kode_SRCS})

target_link_libraries(kode  ${QT_AND_KDECORE_LIBS} kode kabc )

install_targets(/bin kode )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = libkode common schema kxml_compiler .
#
#INCLUDES = -I$(top_srcdir) -I$(srcdir)/libkode $(all_includes)
#KDE_CXXFLAGS = -UQT3_SUPPORT
#
#bin_PROGRAMS = kode
#
#kode_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#kode_LDADD   = libkode/libkode.la -lkabc
#kode_SOURCES = kodemain.cpp
#
#METASOURCES = AUTO
