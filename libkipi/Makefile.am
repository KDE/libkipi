METASOURCES = AUTO
INCLUDES=  -I$(top_srcdir)/libkipi $(all_includes)

EXTRA_DIST = hi32-app-kipi.png

lib_LTLIBRARIES = libkipi.la

libkipi_la_SOURCES = interface.cpp plugin.cpp pluginloader.cpp KDStream.cpp imageinfo.cpp imagecollection.cpp \
		     imageinfoshared.cpp imagecollectionshared.cpp imagecollectiondialog.cpp uploadwidget.cpp

noinst_HEADERS = KDStream.h

libkipiinclude_HEADERS = interface.h plugin.h pluginloader.h imageinfo.h imagecollection.h imageinfoshared.h \
		         imagecollectionshared.h imagecollectiondialog.h uploadwidget.h

libkipi_la_LDFLAGS = $(all_libraries) $(KDE_RPATH) \
		-version-info 0:0:0 -no-undefined

libkipi_la_LIBADD = $(LIB_QT) $(LIB_KDECORE) $(LIB_KDEUI) $(LIB_KFILE)


libkipiincludedir = $(includedir)/libkipi

kde_servicetypes_DATA = kipiplugin.desktop

# i18n translation messages
messages: rc.cpp
	$(XGETTEXT) *.cpp *.h -o $(podir)/libkipi.pot

install-data-local:
	$(mkinstalldirs) $(kde_icondir)/hicolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/hi32-app-kipi.png $(kde_icondir)/hicolor/32x32/apps/kipi.png

uninstall-local:
	-rm -f $(kde_icondir)/hicolor/32x32/apps/hi32-app-kipi.png
