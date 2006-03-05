kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### next target ###############

set(testfeatures_SRCS testfeatures.cpp kde-features.cpp kde-features_parser.cpp )

kde4_automoc(${testfeatures_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(testfeatures ${testfeatures_SRCS})

target_link_libraries(testfeatures  ${QT_AND_KDECORE_LIBS} )

endif(KDE4_BUILD_TESTS)

########### install files ###############


kde4_footer()



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
