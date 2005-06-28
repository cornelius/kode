SUBDIRS = schema . wscl tests kung

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/libkdepim $(all_includes)

lib_LTLIBRARIES = libwsdl.la

libwsdl_la_SOURCES = binding.cpp message.cpp parser.cpp port.cpp service.cpp \
                     typemapper.cpp wsdl.cpp
libwsdl_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libwsdl_la_LIBADD =  -lkdecore schema/libschema.la

bin_PROGRAMS = kwsdl_compiler

kwsdl_compiler_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kwsdl_compiler_LDADD   = ../libkode.la libwsdl.la
kwsdl_compiler_SOURCES =  compiler.cpp converter.cpp creator.cpp main.cpp  \
                          namemapper.cpp 

METASOURCES = AUTO
