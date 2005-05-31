SUBDIRS = . schema

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/libkdepim $(all_includes)

bin_PROGRAMS = kwsdl_compiler

kwsdl_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kwsdl_compiler_LDADD   = ../libkode.la -lkdecore schema/libschema.la
kwsdl_compiler_SOURCES = binding.cpp compiler.cpp converter.cpp creator.cpp main.cpp message.cpp \
                         namemapper.cpp parser.cpp port.cpp service.cpp typemapper.cpp

METASOURCES = AUTO
