#
# Copyright (c) 2010-2015, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif (POLICY CMP0063)

find_package(KF5KDcraw 5.0.0)
find_package(KF5KExiv2 5.0.0)

if(KF5KDcraw_FOUND)
    message(STATUS "libkdcraw detected : test Kipi interface will support Raw processing")
    add_definitions(-DHAVE_KDCRAW)
else()
    message(STATUS "libkdcraw not detected : test Kipi interface will not support Raw processing")
endif()

if(KF5KExiv2_FOUND)
    message(STATUS "libkexiv2 detected : test Kipi interface will support Metadata processing")
    add_definitions(-DHAVE_KDEXIV2)
else()
    message(STATUS "libkkexiv2 not detected : test Kipi interface will not support Metadata processing")
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/..
                    ${CMAKE_CURRENT_BINARY_DIR}/../src
                    ${CMAKE_CURRENT_SOURCE_DIR}/../src
                    ${CMAKE_CURRENT_SOURCE_DIR}/common
                    ${CMAKE_CURRENT_SOURCE_DIR}/kxmlkipicmd
)

add_subdirectory(plugins)

#----------------------------------------------------------------------------------------------------

set(kxmlkipicmd_SRCS
    common/kipiinterface.cpp
    common/kipiimagecollectionshared.cpp
    common/kipiimageinfoshared.cpp
    common/kipiimagecollectionselector.cpp
    common/kipiuploadwidget.cpp
    kxmlkipicmd/kipitestmainwindow.cpp
    kxmlkipicmd/kipitestpluginloader.cpp
    kxmlkipicmd/kipisetup.cpp
    kxmlkipicmd/main.cpp
)

add_executable(kxmlkipicmd ${kxmlkipicmd_SRCS})

target_link_libraries(kxmlkipicmd
                      Qt5::Core
                      Qt5::Gui
                      KF5Kipi
)

if(KF5KDcraw_FOUND)
    target_link_libraries(kxmlkipicmd KF5::KDcraw)
endif()

if(KF5KExiv2_FOUND)
    target_link_libraries(kxmlkipicmd KF5::KExiv2)
endif()

install(TARGETS kxmlkipicmd                           DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES   kxmlkipicmd/kxmlkipicmd_defaultui.rc  DESTINATION ${DATA_INSTALL_DIR}/kxmlkipicmd)
install(FILES   kxmlkipicmd/kxmlkipicmd_gwenviewui.rc DESTINATION ${DATA_INSTALL_DIR}/kxmlkipicmd)

#----------------------------------------------------------------------------------------------------

set(kipicmd_SRCS
    common/kipiinterface.cpp
    common/kipiimagecollectionshared.cpp
    common/kipiimageinfoshared.cpp
    common/kipiimagecollectionselector.cpp
    common/kipiuploadwidget.cpp
    kipicmd/main.cpp
)

add_executable(kipicmd ${kipicmd_SRCS})

target_link_libraries(kipicmd
                      Qt5::Core
                      Qt5::Gui
                      KF5Kipi
)

if(KF5KDcraw_FOUND)
    target_link_libraries(kipicmd KF5::KDcraw)
endif()

if(KF5KExiv2_FOUND)
    target_link_libraries(kipicmd KF5::KExiv2)
endif()
