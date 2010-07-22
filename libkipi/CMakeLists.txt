# ===========================================================
#
# This file is a part of digiKam project
# <a href="http://www.digikam.org">http://www.digikam.org</a>
#
# @date   2004-02-01
# @brief  KDE Image Plugin Interface
#
# @author Copyright (C) 2004-2010 by Gilles Caulier
#         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
#
# This program is free software; you can redistribute it
# and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation;
# either version 2, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# ============================================================

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)

SET(kipi_LIB_SRCS pluginloader.cpp interface.cpp imagecollection.cpp 
                  imagecollectionshared.cpp imageinfoshared.cpp plugin.cpp 
                  imageinfo.cpp uploadwidget.cpp 
                  imagecollectionselector.cpp exportinterface.cpp
                  button.cpp regiontaggingwidget.cpp )

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
              imagecollectionselector.h exportinterface.h extension.h
              regiontaggingwidget.h button.h
              ${CMAKE_CURRENT_BINARY_DIR}/version.h libkipi_export.h 
        DESTINATION ${INCLUDE_INSTALL_DIR}/libkipi COMPONENT Devel)
