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

KDE4_INSTALL_ICONS ( ${ICON_INSTALL_DIR} )
INSTALL(FILES kipi-plugins_logo.png 
              kipi-icon.svg 
              kipi-logo.svg 
        DESTINATION ${DATA_INSTALL_DIR}/kipi/data)

INSTALL(FILES button-approve-normal.png
              button-approve-pressed.png
              button-reject-normal.png
              button-reject-pressed.png
        DESTINATION ${DATA_INSTALL_DIR}/kipi/data)

