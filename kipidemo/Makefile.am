# Add the interfaces subdirectory to our include path, so we can comkpile
# against it without a make install first.
INCLUDES = -I$(top_srcdir)/libkipi $(all_includes)

# Program files
bin_PROGRAMS = kipidemo

METASOURCES = AUTO
kipidemo_SOURCES = main.cpp mainwindow.cpp demointerface.cpp
kipidemo_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kipidemo_LDADD = $(LIB_KDEUI) $(LIB_KPARTS) $(top_builddir)/libkipi/libkipi/libkipi.la

# translation
messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kipidemo.pot

xdg_apps_DATA = kipidemo.desktop

# this is where the XML-GUI resource file goes
rcdir   = $(kde_datadir)/kipidemo
rc_DATA = kipidemoui.rc
