INCLUDES = -I$(top_srcdir) $(all_includes) -I$(top_srcdir)/libkdepim

bin_PROGRAMS = kxml_compiler

kxml_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kxml_compiler_LDADD   = ../libkode.la -lkdecore
kxml_compiler_SOURCES = parser.cpp creator.cpp kxml_compiler.cpp

METASOURCES = AUTO
