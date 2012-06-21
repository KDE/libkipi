/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-06-20
 * @brief  kipi host test application
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

#include "kipitestmainwindow.moc"

// KDE includes

#include <kconfig.h>
#include <ksharedconfig.h>
#include <kaction.h>
#include <ktoggleaction.h>
#include <kstandardaction.h>
#include <kmenubar.h>
#include <kactioncollection.h>

// Local includes


class KipiTestMainWindow::KipiTestMainWindowPriv
{

public:

    KipiTestMainWindowPriv() :
        config(0),
        showMenuBarAction(0)
    {
    }

    KSharedConfig::Ptr  config;
    KToggleAction*      showMenuBarAction;
};

KipiTestMainWindow::KipiTestMainWindow()
    : KXmlGuiWindow(0), d(new KipiTestMainWindowPriv())
{
    setXMLFile("kxmlkipicmdui.rc");

    setObjectName("kxmlkipicmd");

    d->config = KGlobal::config();
    d->showMenuBarAction = KStandardAction::showMenubar(this, SLOT(slotShowMenuBar()), actionCollection());

    setupGUI(Default, xmlFile());
}

KipiTestMainWindow::~KipiTestMainWindow()
{
    delete d;
}

void KipiTestMainWindow::slotShowMenuBar()
{
    menuBar()->setVisible(d->showMenuBarAction->isChecked());
}
