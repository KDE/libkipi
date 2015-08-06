#
# Copyright (c) 2010-2015, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

cmake_minimum_required(VERSION 2.8.9)
project(libkipi)

message(STATUS "----------------------------------------------------------------------------------")
message(STATUS "Starting CMake configuration for: libkipi")

find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)
include(MacroOptionalAddSubdirectory)
include(MacroOptionalFindPackage)
include(FindPackageHandleStandardArgs)

add_definitions(${QT_DEFINITIONS} ${QT_QTDBUS_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

include_directories (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(LIBKIPI_AREA_CODE_GENERAL 51000)
add_definitions(-DKDE_DEFAULT_DEBUG_AREA=${LIBKIPI_AREA_CODE_GENERAL})

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
# 0.2.0 => 5.0.0  (released with KDE 4.1.0).
# 0.3.0 => 6.0.0  (released with KDE 4.2.0).
# 0.4.0 => 6.1.0  (released with KDE 4.3.0).
# 1.0.0 => 7.0.0  (released with KDE 4.4.0).
# 1.1.0 => 7.1.0  (released with KDE 4.5.0).
# 1.2.0 => 8.0.0  (released with KDE 4.6.0).
# 1.3.0 => 8.0.1  (released with KDE 4.7.1  - Set deprecated ImageInfo::title() and ImageInfo::setTitle(). Add ImageInfo::name() and ImageInfo::setName().
# 1.4.0 => 8.0.2  (released with KDE 4.8.0  - Add 2 new slots in ConfigWidget class).
# 1.5.0 => 9.0.0  (released with KDE 4.8.1  - Add Progress Manager feature support from Host application. Remove deprecated methods.
#                                             Add new deprecated methods in ImageInfo class: description(), setDescription(),
#                                             angle(), and setAngle()).
# 1.6.0 => 9.1.0  (released with KDE 4.8.1  - Minor API fix).
# 2.0.0 => 10.0.0 (released with KDE 4.10.0 - Including GoSC2012).
# 2.1.0 => 11.0.0 (released with KDE 4.11.0 - Add edit hints).
# 2.2.0 => 11.1.0 (released with KDE 4.14.0 - Apply patch #89165 from bug #340030).

# Library API version
set(KIPI_LIB_MAJOR_VERSION "2")
set(KIPI_LIB_MINOR_VERSION "2")
set(KIPI_LIB_PATCH_VERSION "0")

# Suffix to add at end of version string. Usual values are:
# "-git"   : alpha code unstable from git. Do not use in production
# "-beta1" : beta1 release.
# "-beta2" : beta2 release.
# "-beta3" : beta3 release.
# "-rc"    : release candidate.
# ""       : final relase. Can be used in production.
set(KIPI_LIB_SUFFIX_VERSION "")

# Library ABI version used by linker.
# For details : http://www.gnu.org/software/libtool/manual/libtool.html#Updating-version-info
set(KIPI_LIB_SO_CUR_VERSION "11")
set(KIPI_LIB_SO_REV_VERSION "1")
set(KIPI_LIB_SO_AGE_VERSION "0")

# =======================================================
# Set env. variables accordinly.

set(KIPI_LIB_VERSION_STRING    "${KIPI_LIB_MAJOR_VERSION}.${KIPI_LIB_MINOR_VERSION}.${KIPI_LIB_PATCH_VERSION}${KIPI_LIB_SUFFIX_VERSION}")
set(KIPI_LIB_VERSION_ID        "0x0${KIPI_LIB_MAJOR_VERSION}0${KIPI_LIB_MINOR_VERSION}0${KIPI_LIB_PATCH_VERSION}")
set(KIPI_LIB_SO_VERSION_STRING "${KIPI_LIB_SO_CUR_VERSION}.${KIPI_LIB_SO_REV_VERSION}.${KIPI_LIB_SO_AGE_VERSION}")

# =======================================================

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

add_subdirectory(pics)
add_subdirectory(libkipi)

# kipi host test application to check the plugins
if(KDE4_BUILD_TESTS)
    add_subdirectory(tests)
endif()
