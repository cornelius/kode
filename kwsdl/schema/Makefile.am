INCLUDES = -I$(top_srcdir) -I.. -I$(top_srcdir)/libkdepim $(all_includes)

lib_LTLIBRARIES = libschema.la

libschema_la_SOURCES = attribute.cpp complextype.cpp element.cpp fileprovider.cpp \
                       parser.cpp qualifiedname.cpp simpletype.cpp types.cpp typestable.cpp
libschema_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libschema_la_LIBADD = -lqt-mt -lkdecore -lkio

#bin_PROGRAMS = schematest
#schematest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#schematest_SOURCES = main.cpp
#schematest_LDADD = libschema.la

METASOURCES = AUTO
