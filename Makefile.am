SUBDIRS = libkode common schema kxml_compiler .

INCLUDES = -I$(top_srcdir) -I$(srcdir)/libkode $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

bin_PROGRAMS = kode

kode_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kode_LDADD   = libkode/libkode.la -lkabc
kode_SOURCES = kodemain.cpp

METASOURCES = AUTO
