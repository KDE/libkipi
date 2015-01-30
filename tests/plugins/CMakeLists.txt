#
# Copyright (c) 2010-2015, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/kxmlhelloworld
                    ${CMAKE_CURRENT_BINARY_DIR}/../../src
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
                    ${CMAKE_CURRENT_BINARY_DIR}
)

set(kipiplugin_kxmlhelloworld_PART_SRCS
    kxmlhelloworld/plugin_kxmlhelloworld.cpp
)

add_library(kipiplugin_kxmlhelloworld MODULE ${kipiplugin_kxmlhelloworld_PART_SRCS})

target_link_libraries(kipiplugin_kxmlhelloworld
                      KF5Kipi
)

configure_file(kxmlhelloworld/kipiplugin_kxmlhelloworld.desktop.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/kxmlhelloworld/kipiplugin_kxmlhelloworld.desktop)

install(FILES   ${CMAKE_CURRENT_BINARY_DIR}/kxmlhelloworld/kipiplugin_kxmlhelloworld.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(TARGETS kipiplugin_kxmlhelloworld                                                    DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES   kxmlhelloworld/kipiplugin_kxmlhelloworldui.rc                                DESTINATION ${KXMLGUI_INSTALL_DIR}/kipi)
