SUBDIRS = lib . kxml_compiler

INCLUDES = -I$(top_srcdir) $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

bin_PROGRAMS = kode

kode_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kode_LDADD   = lib/libkode.la -lkabc
kode_SOURCES = kodemain.cpp

METASOURCES = AUTO
