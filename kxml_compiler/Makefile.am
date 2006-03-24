SUBDIRS = tests

INCLUDES = -I$(top_srcdir)/kode $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

bin_PROGRAMS = kxml_compiler

kxml_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kxml_compiler_LDADD   = ../libkode/libkode.la ../schema/libschema.la -lkdecore
kxml_compiler_SOURCES = parserrelaxng.cpp creator.cpp kxml_compiler.cpp \
  schema.cpp parsercreatordom.cpp parsercreatorcustom.cpp parserxsd.cpp

METASOURCES = AUTO
