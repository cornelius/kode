INCLUDES = -I$(top_srcdir) -I.. -I$(top_srcdir)/kode -I$(top_srcdir)/kode/kwsdl -I$(top_srcdir)/libkdepim $(all_includes)

bin_PROGRAMS = kung
kung_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kung_SOURCES = binaryinputfield.cpp boolinputfield.cpp complexbaseinputfield.cpp dateinputfield.cpp dispatcher.cpp \
               doubleinputfield.cpp enuminputfield.cpp generalconversationmanager.cpp \
               inputdialog.cpp inputfield.cpp inputfieldfactory.cpp \
               integerinputfield.cpp listinputfield.cpp loader.cpp main.cpp outputdialog.cpp pageinputfield.cpp simplebaseinputfield.cpp \
               stringinputfield.cpp timeinputfield.cpp transport.cpp wsclconversationmanager.cpp
kung_LDADD = ../schema/libschema.la ../wscl/libwscl.la ../libwsdl.la -lkparts

METASOURCES = AUTO
