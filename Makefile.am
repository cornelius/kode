SUBDIRS = libkode common schema kxml_compiler kxforms .

INCLUDES = -I$(top_srcdir) -I$(srcdir)/libkode $(all_includes)

bin_PROGRAMS = kode

kode_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kode_LDADD   = libkode/libkode.la -lkabc
kode_SOURCES = kodemain.cpp

METASOURCES = AUTO
