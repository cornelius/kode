kde4_header()

add_subdirectory( doc ) 
add_subdirectory( wsdl ) 
add_subdirectory( wscl ) 

include_directories( ${CMAKE_SOURCE_DIR}/kode ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(kwsdl_compiler_SRCS 
   compiler.cpp 
   converter.cpp 
   converter_attribute.cpp 
   converter_clientstub.cpp 
   converter_complextype.cpp 
   converter_element.cpp 
   converter_serverstub.cpp 
   converter_simpletype.cpp 
   converter_transport.cpp 
   converter_utils.cpp 
   creator.cpp 
   main.cpp 
   namemapper.cpp 
   settings.cpp 
   typemap.cpp )

kde4_automoc(${kwsdl_compiler_SRCS})

kde4_add_executable(kwsdl_compiler ${kwsdl_compiler_SRCS})

target_link_libraries(kwsdl_compiler  ${QT_AND_KDECORE_LIBS} kode wsdl )

install_targets(/bin kwsdl_compiler )


########### install files ###############

install_files( ${XDG_APPS_DIR} FILES kwsdl_compiler.desktop )

kde4_footer()



#original Makefile.am contents follow:

#SUBDIRS = doc wsdl . wscl #kung
#
#INCLUDES = -I$(top_srcdir)/kode $(all_includes)
#KDE_CXXFLAGS = -UQT3_SUPPORT
#
#bin_PROGRAMS = kwsdl_compiler
#
#kwsdl_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH) -no-undefined
#kwsdl_compiler_LDADD   = ../libkode/libkode.la wsdl/libwsdl.la
#kwsdl_compiler_SOURCES =  compiler.cpp converter.cpp converter_attribute.cpp \
#                          converter_clientstub.cpp converter_complextype.cpp \
#                          converter_element.cpp converter_serverstub.cpp \
#                          converter_simpletype.cpp converter_transport.cpp \
#                          converter_utils.cpp creator.cpp main.cpp  \
#                          namemapper.cpp settings.cpp typemap.cpp
#
#xdg_apps_DATA = kwsdl_compiler.desktop
#
#METASOURCES = AUTO
