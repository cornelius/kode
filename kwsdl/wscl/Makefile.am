INCLUDES = -I$(top_srcdir) -I.. -I$(top_srcdir)/libkdepim $(all_includes)

lib_LTLIBRARIES = libwscl.la

libwscl_la_SOURCES = conversation.cpp documents.cpp interaction.cpp parser.cpp transition.cpp
libwscl_la_LDFLAGS = $(all_libraries) -version-info 1:0:0 -no-undefined
libwscl_la_LIBADD = -lqt-mt

#bin_PROGRAMS = wscltest
#wscltest_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#wscltest_SOURCES = main.cpp
#wscltest_LDADD = libwscl.la

METASOURCES = AUTO
