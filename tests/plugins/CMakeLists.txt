# SPDX-FileCopyrightText: 2010-2015 Gilles Caulier <caulier dot gilles at gmail dot com>
#
# SPDX-License-Identifier: BSD-3-Clause

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/kxmlhelloworld
                    ${CMAKE_CURRENT_BINARY_DIR}/../../src
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
                    ${CMAKE_CURRENT_BINARY_DIR}
)

add_library(kipiplugin_kxmlhelloworld MODULE kxmlhelloworld/plugin_kxmlhelloworld.cpp)

target_link_libraries(kipiplugin_kxmlhelloworld
                      KF5Kipi
)

configure_file(kxmlhelloworld/kipiplugin_kxmlhelloworld.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/kxmlhelloworld/kipiplugin_kxmlhelloworld.desktop)

install(FILES   ${CMAKE_CURRENT_BINARY_DIR}/kxmlhelloworld/kipiplugin_kxmlhelloworld.desktop DESTINATION ${KDE_INSTALL_KSERVICESDIR})
install(TARGETS kipiplugin_kxmlhelloworld                                                    DESTINATION ${KDE_INSTALL_PLUGINDIR})
install(FILES   kxmlhelloworld/kipiplugin_kxmlhelloworldui.rc                                DESTINATION ${KDE_INSTALL_KXMLGUIDIR}/kipi)
