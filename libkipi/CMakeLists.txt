INCLUDE_DIRECTORIES(${KDCRAW_INCLUDE_DIR} ${KDE4_INCLUDES} ${QT4_INCLUDES})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)

SET(kipi_LIB_SRCS pluginloader.cpp interface.cpp imagecollection.cpp 
                  imagecollectionshared.cpp imageinfoshared.cpp plugin.cpp 
                  imageinfo.cpp batchprogressdialog.cpp uploadwidget.cpp imagedialog.cpp 
                  imagecollectionselector.cpp)

KDE4_ADD_LIBRARY(kipi SHARED ${kipi_LIB_SRCS})

TARGET_LINK_LIBRARIES(kipi 
                      ${KDCRAW_LIBRARIES} 
                      ${KDE4_KPARTS_LIBS} 
                      ${KDE4_KFILE_LIBS} 
                      ${QT_QTGUI_LIBRARY} 
                      ${QT_QT3SUPPORT_LIBRARY}
                      ${KDE4_KDE3SUPPORT_LIBS}
                     )

SET_TARGET_PROPERTIES(kipi PROPERTIES VERSION ${KIPI_LIB_SO_VERSION_STRING} SOVERSION ${KIPI_LIB_SO_CUR_VERSION})

INSTALL(TARGETS kipi DESTINATION ${LIB_INSTALL_DIR} )

INSTALL(FILES interface.h plugin.h pluginloader.h imageinfo.h imagecollection.h 
              imageinfoshared.h imagecollectionshared.h imagedialog.h uploadwidget.h 
              batchprogressdialog.h imagecollectionselector.h 
              ${CMAKE_CURRENT_BINARY_DIR}/version.h libkipi_export.h 
        DESTINATION ${INCLUDE_INSTALL_DIR}/libkipi)
