#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libkipi_config.h.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/libkipi_config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/..)

set(kipi_LIB_SRCS interface.cpp
                  imagecollection.cpp
                  imagecollectionshared.cpp
                  imageinfoshared.cpp
                  plugin.cpp
                  imageinfo.cpp
                  uploadwidget.cpp
                  imagecollectionselector.cpp
                  configwidget.cpp
                  configwidget_p.cpp
                  pluginloader.cpp
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

install(FILES       ${kipi_CamelCase_HEADERS}
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KIPI/KIPI
        COMPONENT   Devel
)

install(FILES       ${kipi_HEADERS}
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KIPI/kipi
        COMPONENT   Devel
)

add_library(KF5Kipi SHARED ${kipi_LIB_SRCS})

generate_export_header(KF5Kipi BASE_NAME kipi EXPORT_MACRO_NAME LIBKIPI_EXPORT)

install(FILES       ${CMAKE_CURRENT_BINARY_DIR}/kipi_export.h
                    ${CMAKE_CURRENT_BINARY_DIR}/libkipi_config.h
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KIPI
        COMPONENT   Devel
)

add_library(KF5::Kipi ALIAS KF5Kipi)

target_include_directories(KF5Kipi INTERFACE
                           "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KIPI;${KF5_INCLUDE_INSTALL_DIR}/KIPI/kipi>"
)

target_link_libraries(KF5Kipi
                      PUBLIC
                      KF5::Parts
                      KF5::KDELibs4Support
                      Qt5::Gui
)

install(TARGETS KF5Kipi
        EXPORT  KF5KipiTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

set_target_properties(KF5Kipi     PROPERTIES
                      VERSION     ${KIPI_VERSION_STRING}
                      SOVERSION   ${KIPI_SOVERSION}
                      EXPORT_NAME Kipi
)

# desktop file which define plugin service model, used by tool from kipi-plugins.
install(FILES kipiplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
