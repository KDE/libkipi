METASOURCES = AUTO
INCLUDES = -I.. -I$(srcdir)/.. $(LIBKIPI_CFLAGS) $(all_includes)

lib_LTLIBRARIES = libkipi.la

libkipi_la_SOURCES = interface.cpp plugin.cpp pluginloader.cpp KDStream.cpp imageinfo.cpp imagecollection.cpp \
		     imageinfoshared.cpp imagecollectionshared.cpp imagedialog.cpp uploadwidget.cpp \
		     batchprogressdialog.cpp imagecollectionselector.cpp

noinst_HEADERS = KDStream.h

libkipiinclude_HEADERS = interface.h plugin.h pluginloader.h imageinfo.h imagecollection.h imageinfoshared.h \
		         imagecollectionshared.h imagedialog.h uploadwidget.h batchprogressdialog.h \
		         imagecollectionselector.h version.h libkipi_export.h 

libkipi_la_LDFLAGS = $(all_libraries) $(KDE_RPATH) \
		-version-info 1:1:1 -no-undefined

libkipi_la_LIBADD = $(LIB_KIO) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT)

libkipiincludedir = $(includedir)/libkipi


