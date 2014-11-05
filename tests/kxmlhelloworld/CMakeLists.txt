#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/plugin
                    ${CMAKE_CURRENT_SOURCE_DIR}/widgets
                    ${CMAKE_CURRENT_BINARY_DIR}/../../libkipi
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libkipi
                    ${CMAKE_CURRENT_BINARY_DIR}
                   )

set(kipiplugin_kxmlhelloworld_PART_SRCS
    plugin/plugin_kxmlhelloworld.cpp)

KDE4_ADD_PLUGIN(kipiplugin_kxmlhelloworld ${kipiplugin_kxmlhelloworld_PART_SRCS})

target_link_libraries(kipiplugin_kxmlhelloworld
                      ${KDE4_KPARTS_LIBS}
                      ${QT_AND_KDECORE_LIBS}
                      ${KIPI_LIBRARIES}
                      kipi
                     )

configure_file(plugin/kipiplugin_kxmlhelloworld.desktop.cmake ${CMAKE_CURRENT_BINARY_DIR}/plugin/kipiplugin_kxmlhelloworld.desktop)

install(FILES   ${CMAKE_CURRENT_BINARY_DIR}/plugin/kipiplugin_kxmlhelloworld.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(TARGETS kipiplugin_kxmlhelloworld                                            DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES   plugin/kipiplugin_kxmlhelloworldui.rc                                DESTINATION ${DATA_INSTALL_DIR}/kipi)
