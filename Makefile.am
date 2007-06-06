SUBDIRS = libkipi

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libkipi.pc
EXTRA_DIST = libkipi.pc.in

# i18n translation messages
messages: 
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
		$(XGETTEXT) $$LIST -o $(podir)/libkipi.pot; \
	fi


include $(top_srcdir)/admin/Doxyfile.am

kde_servicetypes_DATA = kipiplugin.desktop
