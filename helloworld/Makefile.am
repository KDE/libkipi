INCLUDES    = -I$(top_srcdir)/libkipi $(all_includes)
METASOURCES = AUTO

# Install this plugin in the KDE modules directory
kde_module_LTLIBRARIES = libkipi_hello.la

# Srcs for the plugin
libkipi_hello_la_SOURCES = helloplugin.cpp

# Libs needed by the plugin
libkipi_hello_la_LIBADD = $(LIB_KPARTS) $(top_builddir)/libkipi/libkipi/libkipi.la

# LD flags for the plugin
# -module says: this is a module, i.e. something you're going to dlopen
# so e.g. it has no version number like a normal shared lib would have.
libkipi_hello_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)

# Install the desktop file needed to detect the plugin
kde_services_DATA = libkipi_hello.desktop

# i18n translation messages
messages: rc.cpp
        $(XGETTEXT) *.cpp *.h -o $(podir)/libkipi_hello.pot
