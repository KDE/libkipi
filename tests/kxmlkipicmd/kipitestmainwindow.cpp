/*
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>
    SPDX-FileCopyrightText: 2012-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kipitestmainwindow.h"

// Qt includes

#include <QPointer>
#include <QDebug>
#include <QMenuBar>

// KF includes

#include <KConfigGroup>
#include <KSharedConfig>
#include <KToggleAction>
#include <KStandardAction>
#include <KActionCollection>
#include <KShortcutsDialog>
#include <KEditToolBar>
#include <KWindowConfig>

// LibKIPI includes

#include "pluginloader.h"
#include "libkipi_version.h"

// Local includes

#include "kipitestpluginloader.h"
#include "kipiinterface.h"
#include "kipisetup.h"

namespace KXMLKipiCmd
{

KipiTestMainWindow* KipiTestMainWindow::m_instance = nullptr;

class KipiTestMainWindow::Private
{

public:

    Private() :
        config(nullptr),
        quitAction(nullptr),
        showMenuBarAction(nullptr),
        kipiInterface(nullptr),
        uiFile(QString())
    {
    }

    KSharedConfig::Ptr config;
    QAction*           quitAction;
    KToggleAction*     showMenuBarAction;
    KipiInterface*     kipiInterface;
    QString            uiFile;
};

KipiTestMainWindow::KipiTestMainWindow(const QList<QUrl>& selectedImages,
                                       const QList<QUrl>& selectedAlbums,
                                       const QList<QUrl>& allAlbums)
    : KXmlGuiWindow(nullptr),
      d(new Private())
{
    m_instance           = this;
    d->config            = KSharedConfig::openConfig();
    d->kipiInterface     = new KipiInterface(this, QLatin1String("kxmlkipicmd_KIPI_interface"));
    KConfigGroup uiGroup = d->config->group(QLatin1String("UI Settings"));
    QString uiFile       = uiGroup.readEntry(QString::fromLatin1("UiFile"), QString::fromLatin1("kxmlkipicmd_defaultui.rc"));

    d->uiFile = uiFile;
    setObjectName(QString::fromLatin1("kxmlkipicmd"));

    setMinimumSize(QSize(800, 600));
    KConfigGroup mainWindowGroup = d->config->group(QLatin1String("MainWindow Dialog"));
    KWindowConfig::restoreWindowSize(windowHandle(), mainWindowGroup);

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
    KConfigGroup group = d->config->group(QLatin1String("MainWindow Dialog"));
    KWindowConfig::saveWindowSize(windowHandle(), group);
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
    actionCollection()->addAction(QString::fromLatin1("app_exit"), d->quitAction);

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
    dialog.addCollection(actionCollection(), QLatin1String("General"));
    dialog.addCollection(KipiTestPluginLoader::instance()->pluginsActionCollection(),
                         QLatin1String("KIPI-Plugins"));
    dialog.configure();
}

void KipiTestMainWindow::slotConfToolbars()
{
    KConfigGroup grp = d->config->group(QLatin1String("General Settings"));
    saveMainWindowSettings(grp);
    KEditToolBar dlg(factory(), this);

    connect(&dlg, &KEditToolBar::newToolBarConfig,
            this, &KipiTestMainWindow::slotNewToolbarConfig);

    dlg.exec();
}

void KipiTestMainWindow::slotNewToolbarConfig()
{
    applyMainWindowSettings(d->config->group(QLatin1String("General Settings")));
}

void KipiTestMainWindow::slotSetup()
{
    KipiSetup::runSetupDialog(this);
}

} // namespace KXMLKipiCmd

#include "moc_kipitestmainwindow.cpp"
