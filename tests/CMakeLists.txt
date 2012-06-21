# ===========================================================
#
# This file is a part of digiKam project
# <a href="http://www.digikam.org">http://www.digikam.org</a>
#
# @date   2010-06-29
# @brief  kipi host test application
#
# @author Copyright (C) 2009-2010 by Michael G. Hansen
#         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
# @author Copyright (C) 2012 by Victor Dodon
#         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
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

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../libkipi)

SET(kxmlkipicmd_SRCS
    kipiinterface.cpp
    kipiimagecollectionshared.cpp
    kipiimageinfoshared.cpp
    kipiimagecollectionselector.cpp
    kipiuploadwidget.cpp
    kipitestpluginloader.cpp
    kipitestmainwindow.cpp
    main.cpp
   )

KDE4_ADD_EXECUTABLE(kxmlkipicmd ${kxmlkipicmd_SRCS})

TARGET_LINK_LIBRARIES(kxmlkipicmd
                      kipi
                      ${QT_QTGUI_LIBRARY}
                      ${KDE4_UI_LIBS}
                      ${KDE4_KPARTS_LIBS}
                     )

# for now we do not need HelloWorld plugin
ADD_SUBDIRECTORY(helloworld)

INSTALL(TARGETS kxmlkipicmd      DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
INSTALL(FILES   kxmlkipicmdui.rc DESTINATION ${DATA_INSTALL_DIR}/kxmlkipicmd)
