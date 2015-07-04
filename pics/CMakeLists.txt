#
# Copyright (c) 2010-2014, Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

ecm_install_icons(
    ICONS
        128-apps-kipi.png
        16-apps-kipi.png
        22-apps-kipi.png
        32-apps-kipi.png
        48-apps-kipi.png
    DESTINATION ${ICON_INSTALL_DIR}
)

install(FILES kipi-plugins_logo.png
              kipi-icon.svg
              kipi-logo.svg
        DESTINATION
              ${KDE_INSTALL_DATADIR_KF5}/kipi/pics
)
