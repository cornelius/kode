INCLUDES = -I$(top_srcdir) -I.. -I$(top_srcdir)/kode -I$(top_srcdir)/kode/kwsdl -I$(top_srcdir)/libkdepim $(all_includes)

bin_PROGRAMS = kung
kung_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kung_SOURCES = boolinputfield.cpp complexbaseinputfield.cpp dateinputfield.cpp dispatcher.cpp \
               doubleinputfield.cpp enuminputfield.cpp inputfield.cpp inputfieldfactory.cpp \
               integerinputfield.cpp loader.cpp main.cpp pageinputfield.cpp simplebaseinputfield.cpp \
               stringinputfield.cpp timeinputfield.cpp wsclconversationmanager.cpp
kung_LDADD = ../schema/libschema.la ../wscl/libwscl.la ../libwsdl.la

METASOURCES = AUTO
