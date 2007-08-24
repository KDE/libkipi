PROJECT(libkipi)

# =======================================================
# Informations to update before to release this library.

# Library version history:
# API      BIN
# 0.1.0 => 1.0.0
# 0.1.1 => 1.1.0
# 0.1.2 => 1.2.0
# 0.2.0 => 2.0.0

# Library API version
SET(KIPI_LIB_MAJOR_VERSION "0")
SET(KIPI_LIB_MINOR_VERSION "2")
SET(KIPI_LIB_PATCH_VERSION "0")

# Library BIN version used by linker.
# For details : http://www.gnu.org/software/libtool/manual.html#Updating-version-info
SET(KIPI_LIB_SO_CUR_VERSION "2")
SET(KIPI_LIB_SO_REV_VERSION "0")
SET(KIPI_LIB_SO_AGE_VERSION "0")

# =======================================================
# Set env. variables accordinly.

SET(KIPI_LIB_VERSION_STRING "${KIPI_LIB_MAJOR_VERSION}.${KIPI_LIB_MINOR_VERSION}.${KIPI_LIB_PATCH_VERSION}")
SET(KIPI_LIB_VERSION_ID "0x0${KIPI_LIB_MAJOR_VERSION}0${KIPI_LIB_MINOR_VERSION}0${KIPI_LIB_PATCH_VERSION}")

SET(KIPI_LIB_SO_VERSION_STRING "${KIPI_LIB_SO_CUR_VERSION}.${KIPI_LIB_SO_REV_VERSION}.${KIPI_LIB_SO_AGE_VERSION}")

# =======================================================


SUBDIRS(pics)
SUBDIRS(libkipi)

IF(NOT WIN32)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/libkipi.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkipi.pc)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/libkipi.lsm.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkipi.lsm)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libkipi.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig )
ENDIF(NOT WIN32)
INSTALL(FILES kipiplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
