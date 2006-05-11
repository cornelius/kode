kde4_header()

include_directories( ${QT_INCLUDES} ${CMAKE_SOURCE_DIR}/kode ${KDE4_INCLUDE_DIR} )
########### next target ###############

set(kxforms_SRCS 
   main.cpp 
   mainwindow.cpp 
   manager.cpp 
   form.cpp 
   formcreator.cpp
   hints.cpp
   list.cpp 
   listmodel.cpp
   input.cpp 
   formgui.cpp 
   remotefile.cpp
   xformscommon.cpp 
   xmlbuilder.cpp
   guielement.cpp 
   kresult.cpp 
   guihandler.cpp 
   guihandlerflat.cpp
   guihandlerdialogs.cpp
   reference.cpp 
   textarea.cpp 
   select1.cpp )

kde4_automoc(${kxforms_SRCS})

kde4_add_kcfg_files(kxforms_SRCS prefs.kcfgc )

kde4_add_executable(kxforms ${kxforms_SRCS})

target_link_libraries(kxforms  ${KDE4_KIO_LIBS} kschema )

install_targets(/bin kxforms )


########### next target ###############

set(testreferences_SRCS testreferences.cpp reference.cpp )

kde4_automoc(${testreferences_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(testreferences ${testreferences_SRCS})

target_link_libraries(testreferences  ${KDE4_KDECORE_LIBS} )

endif(KDE4_BUILD_TESTS)

########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kxforms FILES  kxformsui.rc )

install_files( ${KCFG_INSTALL_DIR} FILES kxforms.kcfg )

kde4_footer()



#original Makefile.am contents follow:

#INCLUDES         = $(all_includes)
#
#bin_PROGRAMS = kxforms
#
#kxforms_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#kxforms_LDADD   = $(LIB_KFILE)
#kxforms_SOURCES = main.cpp mainwindow.cpp manager.cpp form.cpp list.cpp \
#  input.cpp formgui.cpp xformscommon.cpp guielement.cpp kresult.cpp \
#  guihandler.cpp reference.cpp textarea.cpp select1.cpp
#
#check_PROGRAMS = testreferences
#
#testreferences_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#testreferences_LDADD   = $(LIB_KFILE)
#testreferences_SOURCES = testreferences.cpp reference.cpp
#
#METASOURCES = AUTO
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/kxforms.pot
#
#xdg_apps_data = kxforms.desktop
#
## this is where the XML-GUI resource file goes
#rcdir = $(kde_datadir)/kxforms
#rc_DATA = kxformsui.rc
