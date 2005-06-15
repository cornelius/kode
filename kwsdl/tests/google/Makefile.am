INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/libkdepim $(all_includes)
METASOURCES = AUTO

bin_PROGRAMS = googletest

googletest_SOURCES = main.cc googlesearch.cc transport.cpp resultelementarray.cpp directorycategoryarray.cpp googlesearchresult.cpp directorycategory.cpp resultelement.cpp serializer.cpp googlesearchservice.cpp
googletest_LDADD = -lkdecore -lkio
googletest_LDFLAGS = $(all_libraries) $(KDE_RPATH)

