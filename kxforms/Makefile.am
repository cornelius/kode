SUBDIRS = tests

INCLUDES = -I$(top_srcdir)/kode $(all_includes)

lib_LTLIBRARIES = libkxforms.la

libkxforms_la_SOURCES = formcreator.cpp xmlbuilder.cpp hints.cpp
libkxforms_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libkxforms_la_LIBADD = ../kxml_compiler/libkschema.la $(LIB_KDECORE)


bin_PROGRAMS = kxforms schema2kxform

kxforms_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kxforms_LDADD   = libkxforms.la $(LIB_KFILE)
kxforms_SOURCES = main.cpp mainwindow.cpp manager.cpp form.cpp list.cpp \
  input.cpp formgui.cpp xformscommon.cpp guielement.cpp kresult.cpp \
  guihandler.cpp reference.cpp textarea.cpp select1.cpp listmodel.cpp \
  guihandlerdialogs.cpp guihandlerflat.cpp prefs.cpp remotefile.cpp

schema2kxform_LDFLAGS = $(KDE_RPATH) $(all_libraries)
schema2kxform_LDADD   = libkxforms.la  $(LIB_KDECORE)
schema2kxform_SOURCES = schema2kxform.cpp

check_PROGRAMS = testreferences

testreferences_LDFLAGS = $(KDE_RPATH) $(all_libraries)
testreferences_LDADD   = $(LIB_KFILE)
testreferences_SOURCES = testreferences.cpp reference.cpp

METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kxforms.pot

xdg_apps_data = kxforms.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/kxforms
rc_DATA = kxformsui.rc
