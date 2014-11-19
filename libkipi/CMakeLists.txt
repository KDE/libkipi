#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)

set(kipi_LIB_SRCS pluginloader.cpp
                  interface.cpp
                  imagecollection.cpp
                  imagecollectionshared.cpp
                  imageinfoshared.cpp
                  plugin.cpp
                  imageinfo.cpp
                  uploadwidget.cpp
                  configwidget.cpp
                  configwidget_p.cpp
                  imagecollectionselector.cpp
   )

add_library(kipi SHARED ${kipi_LIB_SRCS})

target_link_libraries(kipi
                      KF5::Parts
                      KF5::KDELibs4Support
                      Qt5::Gui
                     )

set_target_properties(kipi PROPERTIES VERSION ${KIPI_LIB_SO_VERSION_STRING} SOVERSION ${KIPI_LIB_SO_CUR_VERSION})

install(TARGETS kipi ${INSTALL_TARGETS_DEFAULT_ARGS} )

# desktop file which define plugin service model, used by tool from kipi-plugins.
install(FILES kipiplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

# Check if compilation is done through digiKam SC or not. If yes, do not install header files.
if(NOT DIGIKAMSC_USE_PRIVATE_SHAREDLIBS)

    install(FILES plugin.h
                  pluginloader.h
                  interface.h
                  imageinfo.h
                  imagecollection.h
                  imageinfoshared.h
                  imagecollectionshared.h
                  imagecollectionselector.h
                  uploadwidget.h
                  configwidget.h
                  libkipi_export.h
                  ${CMAKE_CURRENT_BINARY_DIR}/version.h
            DESTINATION ${INCLUDE_INSTALL_DIR}/libkipi
            COMPONENT Devel
           )
endif()
