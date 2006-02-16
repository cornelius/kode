INCLUDES = -I$(top_srcdir) $(all_includes)
KDE_CXXFLAGS = -UQT3_SUPPORT

lib_LTLIBRARIES = libkode.la

libkode_la_SOURCES = code.cpp enum.cpp style.cpp printer.cpp license.cpp file.cpp \
                     class.cpp \
                     function.cpp variable.cpp membervariable.cpp \
                     typedef.cpp statemachine.cpp automakefile.cpp
libkode_la_LDFLAGS = $(KDE_RPATH) $(all_libraries) \
                     -version-info 1:0:0 -no-undefined
libkode_la_LIBADD = -lkdecore

METASOURCES = AUTO
