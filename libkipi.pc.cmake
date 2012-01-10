prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: libkipi
Description: KDE Image Plugin Interface, a C++ library to share plugins among graphic applications.
URL: http://www.digikam.org
Requires:
Version: ${KIPI_LIB_VERSION_STRING}
Libs: -L${LIB_INSTALL_DIR} -lkipi
Cflags: -I${INCLUDE_INSTALL_DIR}
