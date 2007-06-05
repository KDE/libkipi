prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: libkipi
Description: KDE library for shared plugins between graphical photo management applications
URL: http://www.kipi-plugins.org
Requires:
Version: 0.2.0
Libs: -L${LIB_INSTALL_DIR} -lkipi
Cflags: -I${INCLUDE_INSTALL_DIR}
