kde4_header()

add_subdirectory( libkode ) 
add_subdirectory( common ) 
add_subdirectory( schema ) 
add_subdirectory( kxml_compiler ) 

include_directories( ${CMAKE_SOURCE_DIR}/kode/libkode ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kode_bin_SRCS kodemain.cpp )

kde4_automoc(${kode_bin_SRCS})

kde4_add_executable(kode_bin ${kode_bin_SRCS})
set_target_properties(kode_bin PROPERTIES OUTPUT_NAME kode)

target_link_libraries(kode_bin  ${QT_AND_KDECORE_LIBS} kode kabc )

install_targets(/bin kode_bin )


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