project(libkipi)

subdirs(pics)
subdirs(libkipi)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libkipi.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkipi.pc)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libkipi.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig )
install(FILES kipiplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
