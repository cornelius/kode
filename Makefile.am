INCLUDES = -I$(top_srcdir) $(all_includes)

lib_LTLIBRARIES = libkode.la

libkode_la_SOURCES = kode.cpp printer.cpp license.cpp file.cpp class.cpp \
                     function.cpp variable.cpp membervariable.cpp \
                     typedef.cpp
libkode_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) \
                     -version-info 1:0:0 -no-undefined
libkode_la_LIBADD = -lkdecore

bin_PROGRAMS = kode kxml_compiler

kode_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kode_LDADD   = libkode.la -lkabc
kode_SOURCES = kodemain.cpp

kxml_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kxml_compiler_LDADD   = libkode.la -lkdecore
kxml_compiler_SOURCES = kxml_compiler.cpp

METASOURCES = AUTO
