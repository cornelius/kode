INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/kode/kwsdl $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

lib_LTLIBRARIES = libschema.la

libschema_la_SOURCES = attribute.cpp complextype.cpp element.cpp \
                       parser.cpp simpletype.cpp types.cpp xmlelement.cpp \
                       xsdtype.cpp attributegroup.cpp
libschema_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libschema_la_LIBADD = $(LIB_QTCORE) -lkdecore -lkio $(top_builddir)/kode/kwsdl/common/libkxmlcommon.la

bin_PROGRAMS = schematest
schematest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
schematest_SOURCES = main.cpp
schematest_LDADD = libschema.la

METASOURCES = AUTO
