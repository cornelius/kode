SUBDIRS = . kxml_compiler

INCLUDES = -I$(top_srcdir) -I$(top_builddir)/libkdepim $(all_includes)

lib_LTLIBRARIES = libkode.la

libkode_la_SOURCES = code.cpp style.cpp printer.cpp license.cpp file.cpp \
                     class.cpp \
                     function.cpp variable.cpp membervariable.cpp \
                     typedef.cpp statemachine.cpp automakefile.cpp
libkode_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) \
                     -version-info 1:0:0 -no-undefined
libkode_la_LIBADD = -lkdecore

bin_PROGRAMS = kode

kode_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kode_LDADD   = libkode.la -lkabc
kode_SOURCES = kodemain.cpp

METASOURCES = AUTO
