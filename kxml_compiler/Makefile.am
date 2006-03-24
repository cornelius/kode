SUBDIRS = tests

INCLUDES = -I$(top_srcdir)/kode $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

lib_LTLIBRARIES = libkschema.la

libkschema_la_SOURCES = parserrelaxng.cpp parserxsd.cpp schema.cpp
libkschema_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libkschema_la_LIBADD = ../schema/libschema.la $(LIB_QTCORE)

bin_PROGRAMS = kxml_compiler

kxml_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kxml_compiler_LDADD   = libkschema.la ../libkode/libkode.la -lkdecore
kxml_compiler_SOURCES = creator.cpp kxml_compiler.cpp \
  parsercreatordom.cpp parsercreatorcustom.cpp

METASOURCES = AUTO
