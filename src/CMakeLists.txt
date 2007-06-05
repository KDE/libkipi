include_directories(${KDE4_INCLUDES} ${QT4_INCLUDES})

set(kipi_LIB_SRCS pluginloader.cpp interface.cpp imagecollection.cpp imageinfo.cpp plugin.cpp KDStream.cpp  imageinfoshared.cpp imagecollectionshared.cpp imagedialog.cpp uploadwidget.cpp batchprogressdialog.cpp imagecollectionselector.cpp)

kde4_automoc(${kipi_LIB_SRCS})

kde4_add_library(kipi SHARED ${kipi_LIB_SRCS})

target_link_libraries(kipi ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${QT_QTGUI_LIBRARY})

set_target_properties(kipi PROPERTIES VERSION 2.0.0 SOVERSION 2)

install(TARGETS kipi DESTINATION ${LIB_INSTALL_DIR} )

install(FILES interface.h plugin.h pluginloader.h imageinfo.h imagecollection.h imageinfoshared.h imagecollectionshared.h imagedialog.h uploadwidget.h batchprogressdialog.h imagecollectionselector.h version.h libkipi_export.h DESTINATION ${INCLUDE_INSTALL_DIR}/libkipi)

