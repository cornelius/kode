INCLUDES = -I$(top_srcdir)/kode $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

lib_LTLIBRARIES = libschema.la

libschema_la_SOURCES = attribute.cpp complextype.cpp element.cpp \
                       parser.cpp simpletype.cpp types.cpp xmlelement.cpp \
                       xsdtype.cpp attributegroup.cpp compositor.cpp
libschema_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libschema_la_LIBADD = $(LIB_QTCORE) -lkdecore -lkio $(top_builddir)/kode/common/libkxmlcommon.la

check_PROGRAMS = schematest
schematest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
schematest_SOURCES = schematest.cpp
schematest_LDADD = libschema.la

METASOURCES = AUTO
