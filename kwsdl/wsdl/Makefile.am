INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/kode -I$(top_srcdir)/kode/kwsdl \
  $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

lib_LTLIBRARIES = libwsdl.la

libwsdl_la_SOURCES = binding.cpp bindingoperation.cpp definitions.cpp element.cpp fault.cpp import.cpp \
                     message.cpp operation.cpp param.cpp part.cpp port.cpp porttype.cpp \
                     service.cpp soapbinding.cpp type.cpp wsdl.cpp
libwsdl_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libwsdl_la_LIBADD = $(LIB_QTCORE) \
                    $(top_builddir)/kode/common/libkxmlcommon.la \
                    $(top_builddir)/kode/schema/libschema.la

METASOURCES = AUTO
