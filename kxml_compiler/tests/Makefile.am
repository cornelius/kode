INCLUDES = -I$(top_srcdir) $(all_includes)

check_PROGRAMS = testfeatures testholidays

runcheck: testfeatures testholidays
	./testfeatures $(srcdir)/features.xml --output=features.out.xml \
	  >features.out.txt
	$(srcdir)/checkfiles.pl $(srcdir)

reference:
	$(srcdir)/checkfiles.pl $(srcdir) --reference

kde-features.h kde-features.cpp kde-features_parser.cpp: kde-features.xsd kde-features.rng ../kxml_compiler
	../kxml_compiler --external-parser $(srcdir)/kde-features.xsd

kde-holidays.h kde-holidays.cpp kde-holidays_parser.cpp: kde-holidays.rng ../kxml_compiler
	../kxml_compiler --external-parser $(srcdir)/kde-holidays.rng

validate: validate-relaxng validate-schema

validate-relaxng:
	jing $(srcdir)/kde-features.rng $(srcdir)/features.xml

validate-schema:
	xmllint --schema $(srcdir)/kde-features.xsd -noout $(srcdir)/features.xml

testfeatures_LDFLAGS = $(all_libraries) $(KDE_RPATH)
testfeatures_LDADD   = $(LIB_KDECORE)
testfeatures_SOURCES = testfeatures.cpp kde-features.cpp kde-features_parser.cpp
testfeatures_COMPILE_FIRST = kde-features.h

testholidays_LDFLAGS = $(all_libraries) $(KDE_RPATH)
testholidays_LDADD   = $(LIB_KDECORE)
testholidays_SOURCES = testholidays.cpp kde-holidays.cpp kde-holidays_parser.cpp
testholidays_COMPILE_FIRST = kde-holidays.h
