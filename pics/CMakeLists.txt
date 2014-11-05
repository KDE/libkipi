#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

KDE4_INSTALL_ICONS(${ICON_INSTALL_DIR})

install(FILES kipi-plugins_logo.png
              kipi-icon.svg
              kipi-logo.svg
        DESTINATION ${DATA_INSTALL_DIR}/kipi/data)

