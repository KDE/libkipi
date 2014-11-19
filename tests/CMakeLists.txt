#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src
                    ${CMAKE_CURRENT_SOURCE_DIR}/../src
                   )

set(kxmlkipicmd_SRCS
    kipiinterface.cpp
    kipiimagecollectionshared.cpp
    kipiimageinfoshared.cpp
    kipiimagecollectionselector.cpp
    kipiuploadwidget.cpp
    kipisetup.cpp
    kipitestpluginloader.cpp
    kipitestmainwindow.cpp
    main.cpp
   )

add_executable(kxmlkipicmd ${kxmlkipicmd_SRCS})

target_link_libraries(kxmlkipicmd
                      kipi
                      Qt5::Gui
                      KF5::KDELibs4Support
                      KF5::Parts
                     )

add_subdirectory(kxmlhelloworld)

install(TARGETS kxmlkipicmd               DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES   kxmlkipicmd_defaultui.rc  DESTINATION ${DATA_INSTALL_DIR}/kxmlkipicmd)
install(FILES   kxmlkipicmd_gwenviewui.rc DESTINATION ${DATA_INSTALL_DIR}/kxmlkipicmd)
