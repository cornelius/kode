INCLUDES = -I$(top_srcdir)/kode $(all_includes)

bin_PROGRAMS = kxforms schema2kxform

kxforms_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kxforms_LDADD   = $(LIB_KFILE)
kxforms_SOURCES = main.cpp mainwindow.cpp manager.cpp form.cpp list.cpp \
  input.cpp formgui.cpp xformscommon.cpp guielement.cpp kresult.cpp \
  guihandler.cpp reference.cpp textarea.cpp select1.cpp listmodel.cpp

schema2kxform_LDFLAGS = $(KDE_RPATH) $(all_libraries)
schema2kxform_LDADD   = ../kxml_compiler/libkschema.la  $(LIB_KDECORE)
schema2kxform_SOURCES = schema2kxform.cpp xmlbuilder.cpp formcreator.cpp

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
