METASOURCES = AUTO
INCLUDES=  -I$(top_srcdir)/libkipi $(all_includes)

KDE_ICON = kipi

lib_LTLIBRARIES = libkipi.la

libkipi_la_SOURCES = interface.cpp plugin.cpp pluginloader.cpp KDStream.cpp imageinfo.cpp imagecollection.cpp \
		     imageinfoshared.cpp imagecollectionshared.cpp imagedialog.cpp uploadwidget.cpp \
		     batchprogressdialog.cpp imagecollectionselector.cpp

noinst_HEADERS = KDStream.h

libkipiinclude_HEADERS = interface.h plugin.h pluginloader.h imageinfo.h imagecollection.h imageinfoshared.h \
		         imagecollectionshared.h imagedialog.h uploadwidget.h batchprogressdialog.h \
		         imagecollectionselector.h version.h 

libkipi_la_LDFLAGS = $(all_libraries) $(KDE_RPATH) \
		-version-info 0:0:0 -no-undefined

libkipi_la_LIBADD = $(LIB_KIO)

libkipiincludedir = $(includedir)/libkipi

kde_servicetypes_DATA = kipiplugin.desktop

tip_DATA = tips
tipdir = $(kde_datadir)/kipi

# i18n translation messages
messages: rc.cpp
	$(EXTRACTRC) `find . -name "*.rc"` >> rc.cpp
	$(PREPARETIPS) > tips.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
		$(XGETTEXT) $$LIST -o $(podir)/libkipi.pot; \
	fi
	rm -f tips.cpp

check_PROGRAMS = testimagecollectionselector
testimagecollectionselector_SOURCES = testimagecollectionselector.cpp
testimagecollectionselector_LDADD = \
	$(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_KDEPRINT) $(LIB_QT) \
	$(LIBJPEG) libkipi.la
testimagecollectionselector_LDFLAGS = $(all_libraries) 
