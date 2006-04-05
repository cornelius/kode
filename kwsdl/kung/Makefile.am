INCLUDES = -I$(top_srcdir) -I.. -I$(top_srcdir)/kode -I$(top_srcdir)/kode/kwsdl -I$(top_srcdir)/libkdepim $(all_includes)

lib_LTLIBRARIES = libkschemawidgets.la
libkschemawidgets_la_SOURCES = 
	binaryinputfield.cpp \
	boolinputfield.cpp \
	complexbaseinputfield.cpp \
	dateinputfield.cpp \
	doubleinputfield.cpp \
	enuminputfield.cpp \
	inputdialog.cpp \
	inputfield.cpp \
	inputfieldfactory.cpp \
	integerinputfield.cpp \
	listinputfield.cpp \
	pageinputfield.cpp \
	outputdialog.cpp \
	simplebaseinputfield.cpp \
	stringinputfield.cpp \
	timeinputfield.cpp
libkschemawidgets_la_LIBADD = ../../schema/libschema.la -lkparts

bin_PROGRAMS = kung
kung_LDFLAGS = $(all_libraries) $(KDE_RPATH)
kung_SOURCES = \
	dispatcher.cpp \
	generalconversationmanager.cpp \
	loader.cpp \
	main.cpp \
	transport.cpp \
	wsclconversationmanager.cpp
kung_LDADD = libkschemawidgets.la ../wscl/libwscl.la ../wsdl/libwsdl.la

METASOURCES = AUTO
