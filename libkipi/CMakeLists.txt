CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)

SET(kipi_LIB_SRCS pluginloader.cpp interface.cpp imagecollection.cpp 
                  imagecollectionshared.cpp imageinfoshared.cpp plugin.cpp 
                  imageinfo.cpp uploadwidget.cpp 
                  imagecollectionselector.cpp)

KDE4_ADD_LIBRARY(kipi SHARED ${kipi_LIB_SRCS})

TARGET_LINK_LIBRARIES(kipi 
                      ${KDE4_KPARTS_LIBS} 
                      ${KDE4_KFILE_LIBS} 
                      ${QT_QTGUI_LIBRARY} 
                     )

SET_TARGET_PROPERTIES(kipi PROPERTIES VERSION ${KIPI_LIB_SO_VERSION_STRING} SOVERSION ${KIPI_LIB_SO_CUR_VERSION})

INSTALL(TARGETS kipi ${INSTALL_TARGETS_DEFAULT_ARGS} )

INSTALL(FILES interface.h plugin.h pluginloader.h imageinfo.h imagecollection.h 
              imageinfoshared.h imagecollectionshared.h uploadwidget.h 
              imagecollectionselector.h 
              ${CMAKE_CURRENT_BINARY_DIR}/version.h libkipi_export.h 
        DESTINATION ${INCLUDE_INSTALL_DIR}/libkipi)
