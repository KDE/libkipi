# ===========================================================
#
# This file is a part of digiKam project
# <a href="http://www.digikam.org">http://www.digikam.org</a>
#
# @date   2004-02-01
# @brief  KDE Image Plugin Interface
#
# @author Copyright (C) 2004-2012 by Gilles Caulier
#         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
# @author Copyright (C) 2012 by Victor Dodon
#         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
#
# This program is free software; you can redistribute it
# and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation;
# either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# ============================================================

PROJECT(libkipi)

MESSAGE(STATUS "----------------------------------------------------------------------------------")
MESSAGE(STATUS "Starting CMake configuration for: libkipi")

FIND_PACKAGE(KDE4 REQUIRED)

INCLUDE(KDE4Defaults)
INCLUDE(MacroLibrary)
INCLUDE(MacroOptionalAddSubdirectory)
INCLUDE(MacroOptionalFindPackage)
INCLUDE(FindPackageHandleStandardArgs)

ADD_DEFINITIONS(${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS})
ADD_DEFINITIONS(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

INCLUDE_DIRECTORIES (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

SET(LIBKIPI_AREA_CODE_GENERAL 51000)
ADD_DEFINITIONS(-DKDE_DEFAULT_DEBUG_AREA=${LIBKIPI_AREA_CODE_GENERAL})

# =======================================================
# Information to update before to release this library.

# Library version history:
# API      ABI
# 0.1.0 => 0.0.0
# 0.1.1 => 0.0.0
# 0.1.2 => 1.0.1
# 0.1.3 => 1.0.1
# 0.1.4 => 1.0.1
# 0.1.5 => 1.1.1
# 0.1.6 => 2.0.0
# 0.2.0 => 5.0.0  (released with KDE 4.1.0)
# 0.3.0 => 6.0.0  (released with KDE 4.2.0)
# 0.4.0 => 6.1.0  (released with KDE 4.3.0)
# 1.0.0 => 7.0.0  (released with KDE 4.4.0)
# 1.1.0 => 7.1.0  (released with KDE 4.5.0)
# 1.2.0 => 8.0.0  (released with KDE 4.6.0)
# 1.3.0 => 8.0.1  (released with KDE 4.7.1  - Set deprecated ImageInfo::title() and ImageInfo::setTitle(). Add ImageInfo::name() and ImageInfo::setName().
# 1.4.0 => 8.0.2  (released with KDE 4.8.0  - Add 2 new slots in ConfigWidget class).
# 1.5.0 => 9.0.0  (released with KDE 4.8.1  - Add Progress Manager feature support from Host application. Remove deprecated methods.
#                                             Add new deprecated methods in ImageInfo class: description(), setDescription(),
#                                             angle(), and setAngle()).
# 1.6.0 => 9.1.0  (released with KDE 4.8.1  - Minor API fix)
# 2.0.0 => 10.0.0 (released with KDE 4.10.0 - Including GoSC2012)
# 2.1.0 => 11.0.0 (released with KDE 4.11.0 - Add edit hints)

# Library API version
SET(KIPI_LIB_MAJOR_VERSION "2")
SET(KIPI_LIB_MINOR_VERSION "1")
SET(KIPI_LIB_PATCH_VERSION "0")

# Suffix to add at end of version string. Usual values are:
# "-git"   : alpha code unstable from git. Do not use in production
# "-beta1" : beta1 release.
# "-beta2" : beta2 release.
# "-beta3" : beta3 release.
# "-rc"    : release candidate.
# ""       : final relase. Can be used in production.
SET(KIPI_LIB_SUFFIX_VERSION "")

# Library ABI version used by linker.
# For details : http://www.gnu.org/software/libtool/manual/libtool.html#Updating-version-info
SET(KIPI_LIB_SO_CUR_VERSION "11")
SET(KIPI_LIB_SO_REV_VERSION "0")
SET(KIPI_LIB_SO_AGE_VERSION "0")

# =======================================================
# Set env. variables accordinly.

SET(KIPI_LIB_VERSION_STRING "${KIPI_LIB_MAJOR_VERSION}.${KIPI_LIB_MINOR_VERSION}.${KIPI_LIB_PATCH_VERSION}${KIPI_LIB_SUFFIX_VERSION}")
SET(KIPI_LIB_VERSION_ID "0x0${KIPI_LIB_MAJOR_VERSION}0${KIPI_LIB_MINOR_VERSION}0${KIPI_LIB_PATCH_VERSION}")

SET(KIPI_LIB_SO_VERSION_STRING "${KIPI_LIB_SO_CUR_VERSION}.${KIPI_LIB_SO_REV_VERSION}.${KIPI_LIB_SO_AGE_VERSION}")

# =======================================================

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

ADD_SUBDIRECTORY(pics)
ADD_SUBDIRECTORY(libkipi)

# kipi host test application to check the plugins
IF(KDE4_BUILD_TESTS)
    ADD_SUBDIRECTORY(tests)
ENDIF(KDE4_BUILD_TESTS)

IF(NOT WIN32)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/libkipi.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkipi.pc)
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/libkipi.lsm.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkipi.lsm)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libkipi.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig )
ENDIF(NOT WIN32)

# desktop file which define plugin service model, used by tool from kipi-plugins.
INSTALL(FILES kipiplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
