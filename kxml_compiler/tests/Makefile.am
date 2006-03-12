INCLUDES = -I$(top_srcdir) $(all_includes)

check_PROGRAMS = testfeatures

runcheck: testfeatures
	./testfeatures $(srcdir)/features.xml --output=features.out.xml \
	  >features.out.txt
	$(srcdir)/checkfiles.pl $(srcdir)

reference:
	$(srcdir)/checkfiles.pl $(srcdir) --reference

kde-features.h kde-features.cpp kde-features_parser.cpp: kde-features.xsd kde-features.rng ../kxml_compiler
	../kxml_compiler --external-parser $(srcdir)/kde-features.rng

validate: validate-relaxng validate-schema

validate-relaxng:
	jing $(srcdir)/kde-features.rng $(srcdir)/features.xml

validate-schema:
	xmllint --schema $(srcdir)/kde-features.xsd -noout $(srcdir)/features.xml

testfeatures_LDFLAGS = $(all_libraries) $(KDE_RPATH)
testfeatures_LDADD   = $(LIB_KDECORE)
testfeatures_SOURCES = testfeatures.cpp kde-features.cpp kde-features_parser.cpp
testfeatures_COMPILE_FIRST = kde-features.h
