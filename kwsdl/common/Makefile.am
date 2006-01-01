INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/kode/kwsdl -I.. -I$(top_srcdir)/libkdepim $(all_includes)

lib_LTLIBRARIES = libkxmlcommon.la

libkxmlcommon_la_SOURCES = fileprovider.cpp messagehandler.cpp nsmanager.cpp parsercontext.cpp qname.cpp
libkxmlcommon_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libkxmlcommon_la_LIBADD = $(LIB_QTCORE) -lkdecore -lkio

METASOURCES = AUTO
