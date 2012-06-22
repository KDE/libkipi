/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-06-20
 * @brief  kipi host test application using KDE XML-GUI technology
 *
 * @author Copyright (C) 2012 by Victor Dodon
 *         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef KIPITESTMAINWINDOW_H
#define KIPITESTMAINWINDOW_H

// KDE includes

#include <kxmlguiwindow.h>

namespace KXMLKipiCmd
{

class KipiTestMainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:

    KipiTestMainWindow();
    ~KipiTestMainWindow();

    static KipiTestMainWindow* instance();

private:

    void loadPlugins();

private Q_SLOTS:

    void slotShowMenuBar();
    void slotEditKeys();
    void slotConfToolbars();
    void slotSetup();

private:

    class KipiTestMainWindowPriv;
    KipiTestMainWindowPriv* const d;

    static KipiTestMainWindow* m_instance;
};

} // namespace KXMLKipiCmd

#endif // KIPITESTMAINWINDOW_H
