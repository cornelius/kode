check: ./features.kxform ./project.kxform
	$(top_srcdir)/kode/kxml_compiler/tests/checkfiles.pl $(srcdir)

reference:
	$(top_srcdir)/kode/kxml_compiler/tests/checkfiles.pl $(srcdir) --reference

./features.kxform: $(srcdir)/features.xsd ../schema2kxform
	../schema2kxform $(srcdir)/features.xsd >features.kxform

./project.kxform: $(srcdir)/project.xsd ../schema2kxform
	../schema2kxform $(srcdir)/project.xsd >project.kxform
