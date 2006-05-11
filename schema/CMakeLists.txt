kde4_header()

include_directories( ${CMAKE_SOURCE_DIR}/kode  )


########### next target ###############

set(schema_LIB_SRCS 
   annotation.cpp
   attribute.cpp 
   complextype.cpp 
   element.cpp 
   parser.cpp 
   simpletype.cpp 
   types.cpp 
   xmlelement.cpp 
   xsdtype.cpp 
   attributegroup.cpp 
	compositor.cpp)

kde4_automoc(${schema_LIB_SRCS})

kde4_add_library(schema SHARED ${schema_LIB_SRCS})

target_link_libraries(schema  ${KDE4_KIO_LIBS} kxmlcommon )

set_target_properties(schema PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} schema )


########### next target ###############

set(schematest_SRCS schematest.cpp )

kde4_automoc(${schematest_SRCS})

kde4_add_executable(schematest ${schematest_SRCS})

target_link_libraries(schematest  ${KDE4_KDECORE_LIBS} schema )

install_targets(/bin schematest )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir)/kode $(all_includes)
#KDE_CXXFLAGS = -UQT3_SUPPORT
#
#lib_LTLIBRARIES = libschema.la
#
#libschema_la_SOURCES = attribute.cpp complextype.cpp element.cpp \
#                       parser.cpp simpletype.cpp types.cpp xmlelement.cpp \
#                       xsdtype.cpp attributegroup.cpp
#libschema_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
#libschema_la_LIBADD = $(LIB_QTCORE) -lkdecore -lkio $(top_builddir)/kode/common/libkxmlcommon.la
#
#bin_PROGRAMS = schematest
#schematest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#schematest_SOURCES = main.cpp
#schematest_LDADD = libschema.la
#
#METASOURCES = AUTO
