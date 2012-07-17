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

// Qt includes

#include <QPointer>

// KDE includes

#include <kdialog.h>
#include <kconfig.h>
#include <ksharedconfig.h>
#include <kaction.h>
#include <ktoggleaction.h>
#include <kstandardaction.h>
#include <kmenubar.h>
#include <kactioncollection.h>
#include <kshortcutsdialog.h>
#include <klocale.h>
#include <kedittoolbar.h>
#include <kdebug.h>
#include <kxmlguifactory.h>

// LibKIPI includes

#include "pluginloader.h"
#include "version.h"

// Local includes

#include "kipitestpluginloader.h"
#include "kipiinterface.h"
#include "kipisetup.h"

namespace KXMLKipiCmd
{

KipiTestMainWindow* KipiTestMainWindow::m_instance = 0;

class KipiTestMainWindow::KipiTestMainWindowPriv
{

public:

    KipiTestMainWindowPriv() :
        config(0),
        quitAction(0),
        showMenuBarAction(0),
        kipiInterface(0),
        uiFile(QString())
    {
    }

    KSharedConfig::Ptr config;
    KAction*           quitAction;
    KToggleAction*     showMenuBarAction;
    KipiInterface*     kipiInterface;
    QString            uiFile;

};

KipiTestMainWindow::KipiTestMainWindow(const KUrl::List& selectedImages,
                                       const KUrl::List& selectedAlbums,
                                       const KUrl::List& allAlbums)
    : KXmlGuiWindow(0), d(new KipiTestMainWindowPriv())
{
    m_instance           = this;
    d->config            = KGlobal::config();
    d->kipiInterface     = new KipiInterface(this, "kxmlkipicmd_KIPI_interface");
    KConfigGroup uiGroup = d->config->group("UI Settings");
    QString uiFile       = uiGroup.readEntry("UiFile", QString("kxmlkipicmd_defaultui.rc"));

    d->uiFile = uiFile;
    setObjectName("kxmlkipicmd");

    setMinimumSize(QSize(800, 600));
    KConfigGroup mainWindowGroup = d->config->group("MainWindow Dialog");
    restoreWindowSize(mainWindowGroup);

    if (!selectedImages.empty())
    {
        d->kipiInterface->addSelectedImages(selectedImages);
    }
    if (!selectedAlbums.empty())
    {
        d->kipiInterface->addSelectedAlbums(selectedAlbums);
    }
    if (!allAlbums.empty())
    {
        d->kipiInterface->addAlbums(allAlbums);
    }

    setupActions();

    // Provides a menu entry that allows showing/hiding the toolbar(s)
    setStandardToolBarMenuEnabled(true);

    loadPlugins();
}

KipiTestMainWindow::~KipiTestMainWindow()
{
    KConfigGroup group = d->config->group("MainWindow Dialog");
    saveWindowSize(group);
    group.sync();
    delete d;
}

KipiTestMainWindow* KipiTestMainWindow::instance()
{
    return m_instance;
}

void KipiTestMainWindow::setupActions()
{
    d->showMenuBarAction = KStandardAction::showMenubar(this, SLOT(slotShowMenuBar()), actionCollection());
    d->quitAction        = KStandardAction::quit(this,        SLOT(close()),           actionCollection());
    actionCollection()->addAction("app_exit", d->quitAction);

    KStandardAction::keyBindings(this,       SLOT(slotEditKeys()),     actionCollection());
    KStandardAction::configureToolbars(this, SLOT(slotConfToolbars()), actionCollection());
    KStandardAction::preferences(this,       SLOT(slotSetup()),        actionCollection());

    createGUI(d->uiFile);
}

void KipiTestMainWindow::loadPlugins()
{
    new KipiTestPluginLoader(this, d->kipiInterface);
}

void KipiTestMainWindow::slotShowMenuBar()
{
    menuBar()->setVisible(d->showMenuBarAction->isChecked());
}

void KipiTestMainWindow::slotEditKeys()
{
    KShortcutsDialog dialog(KShortcutsEditor::AllActions,
                            KShortcutsEditor::LetterShortcutsAllowed, this);
    dialog.addCollection(actionCollection(), i18nc("general keyboard shortcuts", "General"));
    dialog.addCollection(KipiTestPluginLoader::instance()->pluginsActionCollection(),
                         i18nc("KIPI-Plugins keyboard shortcuts", "KIPI-Plugins"));
    dialog.configure();
}

void KipiTestMainWindow::slotConfToolbars()
{
    saveMainWindowSettings(d->config->group("General Settings"));
    KEditToolBar dlg(factory(), this);

    connect(&dlg, SIGNAL(newToolBarConfig()),
            this, SLOT(slotNewToolbarConfig()));

    dlg.exec();
}

void KipiTestMainWindow::slotNewToolbarConfig()
{
    applyMainWindowSettings(d->config->group("General Settings"));
}

void KipiTestMainWindow::slotSetup()
{
    KipiSetup::exec(this);
}

} // namespace KXMLKipiCmd
