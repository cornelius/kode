kde4_header()

include_directories( ${CMAKE_SOURCE_DIR}/kode ${CMAKE_SOURCE_DIR}/kode/kwsdl ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(wsdl_LIB_SRCS 
   binding.cpp 
   bindingoperation.cpp 
   definitions.cpp 
   element.cpp 
   fault.cpp 
   import.cpp 
   message.cpp 
   operation.cpp 
   param.cpp 
   part.cpp 
   port.cpp 
   porttype.cpp 
   service.cpp 
   soapbinding.cpp 
   type.cpp 
   wsdl.cpp )

kde4_automoc(${wsdl_LIB_SRCS})

kde4_add_library(wsdl SHARED ${wsdl_LIB_SRCS})

target_link_libraries(wsdl  ${QT_AND_KDECORE_LIBS} kxmlcommon schema )

set_target_properties(wsdl PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install_targets(${LIB_INSTALL_DIR} wsdl )


########### install files ###############


kde4_footer()



#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/kode -I$(top_srcdir)/kode/kwsdl \
#  $(all_includes)
#KDE_CXXFLAGS = -UQT3_SUPPORT
#
#lib_LTLIBRARIES = libwsdl.la
#
#libwsdl_la_SOURCES = binding.cpp bindingoperation.cpp definitions.cpp element.cpp fault.cpp import.cpp \
#                     message.cpp operation.cpp param.cpp part.cpp port.cpp porttype.cpp \
#                     service.cpp soapbinding.cpp type.cpp wsdl.cpp
#libwsdl_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
#libwsdl_la_LIBADD = $(LIB_QTCORE) \
#                    $(top_builddir)/kode/common/libkxmlcommon.la \
#                    $(top_builddir)/kode/schema/libschema.la
#
#METASOURCES = AUTO
