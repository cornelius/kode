INCLUDES = -I$(top_srcdir)/kode $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

bin_PROGRAMS = kxml_compiler

kxml_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kxml_compiler_LDADD   = ../libkode/libkode.la -lkdecore
kxml_compiler_SOURCES = parserrelaxng.cpp creator.cpp kxml_compiler.cpp

METASOURCES = AUTO
