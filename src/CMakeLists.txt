#
# Copyright (c) 2010-2018, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libkipi_config.h.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/libkipi_config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

qt5_add_resources(libkipiresources_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/../pics/libkipi.qrc)

set(kipi_LIB_SRCS interface.cpp
                  imagecollection.cpp
                  imagecollectionshared.cpp
                  imageinfoshared.cpp
                  plugin.cpp
                  imageinfo.cpp
                  uploadwidget.cpp
                  imagecollectionselector.cpp
                  configwidget.cpp
                  pluginloader.cpp
                  libkipi_debug.cpp

                  ${libkipiresources_SRCS}
)

ecm_generate_headers(kipi_CamelCase_HEADERS

                     HEADER_NAMES
                     Plugin
                     PluginLoader
                     Interface
                     ImageInfo
                     ImageCollection
                     ImageInfoShared
                     ImageCollectionShared
                     ImageCollectionSelector
                     UploadWidget
                     ConfigWidget

                     PREFIX           KIPI
                     REQUIRED_HEADERS kipi_HEADERS
)

add_library(KF5Kipi SHARED ${kipi_LIB_SRCS})
add_library(KF5::Kipi ALIAS KF5Kipi)

generate_export_header(KF5Kipi BASE_NAME libkipi EXPORT_MACRO_NAME LIBKIPI_EXPORT)

# disable adding of current source directory to interface, as it causes naming clashes
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE OFF)

target_include_directories(KF5Kipi INTERFACE
                           "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KIPI;${KF5_INCLUDE_INSTALL_DIR}/KIPI/kipi>"
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

target_link_libraries(KF5Kipi
                      PUBLIC
                      Qt5::Gui
                      Qt5::Widgets

                      KF5::XmlGui
                      KF5::Service
                      KF5::ConfigCore
)

set_target_properties(KF5Kipi     PROPERTIES
                      VERSION     ${KIPI_VERSION_STRING}
                      SOVERSION   ${KIPI_SOVERSION}
                      EXPORT_NAME Kipi
)

install(TARGETS KF5Kipi
        EXPORT  KF5KipiTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)


# desktop file which define plugin service model, used by tools from kipi-plugins.
install(FILES kipiplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

install(FILES       ${kipi_CamelCase_HEADERS}
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KIPI/KIPI
        COMPONENT   Devel
)

install(FILES       ${kipi_HEADERS}
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KIPI/kipi
        COMPONENT   Devel
)


install(FILES       ${CMAKE_CURRENT_BINARY_DIR}/libkipi_export.h
                    ${CMAKE_CURRENT_BINARY_DIR}/libkipi_config.h
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KIPI/kipi
        COMPONENT   Devel
)

install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/libkipi_version.h"
        DESTINATION "${KF5_INCLUDE_INSTALL_DIR}"
        COMPONENT   Devel
)
