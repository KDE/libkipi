project(libkipi)

subdirs(pics)
subdirs(libkipi)

if(NOT WIN32)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libkipi.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libkipi.pc)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libkipi.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig )
endif(NOT WIN32)
install(FILES kipiplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
