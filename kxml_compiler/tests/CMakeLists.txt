
if(KDE4_BUILD_TESTS)




########### next target ###############

set(testfeatures_SRCS testfeatures.cpp)
kode_add_xml_parser(testfeatures_SRCS kde-features.rng)

kde4_automoc(${testfeatures_SRCS})

kde4_add_executable(testfeatures ${testfeatures_SRCS})

target_link_libraries(testfeatures  ${KDE4_KDECORE_LIBS} )

########### next target ###############

set(testholidays_SRCS testholidays.cpp)
kode_add_xml_parser(testholidays_SRCS kde-holidays.rng)

kde4_automoc(${testholidays_SRCS})

kde4_add_executable(testholidays ${testholidays_SRCS})

target_link_libraries(testholidays  ${KDE4_KDECORE_LIBS} )

########### install files ###############

endif(KDE4_BUILD_TESTS)




#original Makefile.am contents follow:

#INCLUDES = -I$(top_srcdir) $(all_includes)
#
#check_PROGRAMS = testfeatures
#
#runcheck: testfeatures
#	./testfeatures $(srcdir)/features.xml --output=features.out.xml \
#	  >features.out.txt
#	$(srcdir)/checkfiles.pl $(srcdir)
#
#kde-features.h kde-features.cpp kde-features_parser.cpp: kde-features.xsd kde-features.rng ../kxml_compiler
#	../kxml_compiler --external-parser $(srcdir)/kde-features.rng
#
#validate: validate-relaxng validate-schema
#
#validate-relaxng:
#	jing $(srcdir)/kde-features.rng $(srcdir)/features.xml
#
#validate-schema:
#	xmllint --schema $(srcdir)/kde-features.xsd -noout $(srcdir)/features.xml
#
#testfeatures_LDFLAGS = $(all_libraries) $(KDE_RPATH)
#testfeatures_LDADD   = $(LIB_KDECORE)
#testfeatures_SOURCES = testfeatures.cpp kde-features.cpp kde-features_parser.cpp
#testfeatures_COMPILE_FIRST = kde-features.h
