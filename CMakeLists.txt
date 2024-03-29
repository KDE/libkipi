# SPDX-FileCopyrightText: 2010-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.16.0)

project(libkipi)

message(STATUS "----------------------------------------------------------------------------------")
message(STATUS "Starting CMake configuration for: ${PROJECT_NAME}")

set(KF_MIN_VERSION      "5.91.0")
set(QT_MIN_VERSION      "5.15.2")

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
# 5.0.0 => 30.0.0 (Released with KDE Applications <= 15.12)
# 5.1.0 => 31.0.0 (Released with KDE Applications 16.04)
# 5.2.0 => 32.0.0 (Released with KDE Applications 16.05 - Fix API with pure virtual methods)

# Library API version
set(KIPI_LIB_MAJOR_VERSION "5")
set(KIPI_LIB_MINOR_VERSION "2")
set(KIPI_LIB_PATCH_VERSION "0")

# Library ABI version used by linker.
# For details : http://www.gnu.org/software/libtool/manual/libtool.html#Updating-version-info
set(KIPI_LIB_SO_CUR_VERSION "32")
set(KIPI_LIB_SO_REV_VERSION "0")
set(KIPI_LIB_SO_AGE_VERSION "0")

SET(LIBKIPI_LIB_VERSION "${KIPI_LIB_MAJOR_VERSION}.${KIPI_LIB_MINOR_VERSION}.${KIPI_LIB_PATCH_VERSION}")
SET(LIBKIPI_SO_VERSION  "${KIPI_LIB_SO_CUR_VERSION}.${KIPI_LIB_SO_REV_VERSION}.${KIPI_LIB_SO_AGE_VERSION}")

############## ECM setup ######################

find_package(ECM ${KF_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${libkkipi_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMQtDeclareLoggingCategory)
include(ECMSetupVersion)
include(ECMDeprecationSettings)
include(FeatureSummary)
include(ECMInstallIcons)
include(CMakePackageConfigHelpers)


############## Find Packages ###################

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS
             Core
             Widgets
             Gui
)

find_package(KF5 ${KF_MIN_VERSION}
             REQUIRED COMPONENTS
             Config
             XmlGui
             Service
)

ecm_setup_version(${LIBKIPI_LIB_VERSION}
                  VARIABLE_PREFIX      KIPI
                  VERSION_HEADER       "src/libkipi_version.h"
                  PACKAGE_VERSION_FILE "KF5KipiConfigVersion.cmake"
                  SOVERSION            ${LIBKIPI_SO_VERSION})

############## Targets #########################

add_definitions(-DTRANSLATION_DOMAIN=\"libkipi\")

ecm_set_disabled_deprecation_versions(
    QT 5.15.2
    KF 5.91
    KCOREADDONS 5.85 # indirectly KService::createInstance
    KSERVICE 5.85 # KService::createInstance
)

add_subdirectory(pics)
add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()

############## CMake Config Files ##############

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Kipi")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/KF5KipiConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KipiConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/KF5KipiConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/KF5KipiConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT   Devel
)

install(EXPORT      KF5KipiTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE        KF5KipiTargets.cmake
        NAMESPACE   KF5::
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
