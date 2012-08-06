/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-06-28
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

#include "kipisetup.moc"

// Qt includes

#include <QList>
#include <QRadioButton>
#include <QPushButton>
#include <QDir>
#include <QVBoxLayout>

// KDE includes

#include <kpagewidget.h>
#include <kapplication.h>
#include <kstandarddirs.h>
#include <kdebug.h>
#include <kcomponentdata.h>
#include <kcombobox.h>

// Libkipi includes

#include "pluginloader.h"
#include "configwidget.h"

// Local includes

#include "kipitestpluginloader.h"

using namespace KIPI;

namespace KXMLKipiCmd
{

const QString SetupXML::CONFIG_GROUP_NAME = "UI Settings";

class KipiSetup::Private
{

public:

    Private() :
        page_plugins(0),
        page_xml(0),
        pluginsPage(0),
        xmlPage(0)
    {
    }

    KPageWidgetItem* page_plugins;
    KPageWidgetItem* page_xml;

    SetupPlugins*    pluginsPage;
    SetupXML*        xmlPage;
};

KipiSetup::KipiSetup(QWidget* const parent)
    : KPageDialog(parent), d(new Private)
{
    setCaption(i18n("Configure"));
    setButtons(Ok | Cancel);
    setDefaultButton(Ok);
    setFaceType(List);
    setModal(true);
    setMinimumSize(600, 400);

    d->pluginsPage  = new SetupPlugins(this);
    d->page_plugins = addPage(d->pluginsPage, i18n("Kipi Plugins"));
    d->page_plugins->setIcon(KIcon("kipi"));

    d->xmlPage  = new SetupXML(this);
    d->page_xml = addPage(d->xmlPage, i18n("UI layouts"));
    d->page_xml->setIcon(KIcon("application-xml"));
    d->page_xml->setHeader("Configure the UI file for the KXMLKipiCmd application");

    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group("Setup Dialog");
    restoreDialogSize(group);

    int pageIndex         = group.readEntry("Setup Page", 0);
    KPageWidgetItem *page = 0;
    if (pageIndex == XmlFilesPage)
    {
        page = d->page_xml;
    }
    else
    {
        page = d->page_plugins;
    }
    setCurrentPage(page);
}

KipiSetup::~KipiSetup()
{
    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group(QString("Setup Dialog"));
    saveDialogSize(group);
    group.writeEntry("Setup Page", (int)activePageIndex());
    config->sync();
    delete d;
}

bool KipiSetup::exec(QWidget* const parent)
{
    QPointer<KipiSetup> setup = new KipiSetup(parent);
    bool success              = (setup->KPageDialog::exec() == QDialog::Accepted);
    delete setup;
    return success;
}

void KipiSetup::slotButtonClicked(int button)
{
    if (button == KDialog::Ok)
    {
        okClicked();
    }
    else
    {
        KDialog::slotButtonClicked(button);
    }
}

void KipiSetup::okClicked()
{
    kapp->setOverrideCursor(Qt::WaitCursor);

    d->pluginsPage->apply();
    d->xmlPage->apply();

    kapp->restoreOverrideCursor();
    accept();
}

int KipiSetup::activePageIndex()
{
    KPageWidgetItem* curr = currentPage();

    if (curr == d->page_xml)
    {
        return XmlFilesPage;
    }

    return KipiPluginsPage;
}

// -------------------------------------------------------------------

class SetupXML::Private
{
public:

    Private()
    {
        xmlFilesCob = 0;
    }

    QString        uiFilesPath;
    QList<QString> uiFilesList;
    KComboBox*     xmlFilesCob;
};

SetupXML::SetupXML(QWidget* const parent)
    : QScrollArea(parent), d(new Private)
{
    QWidget* panel = new QWidget(viewport());
    setWidget(panel);
    setWidgetResizable(true);

    d->uiFilesPath = KGlobal::dirs()->findDirs("data", KGlobal::mainComponent().componentName()).last();
    QDir dir(d->uiFilesPath);
    QString filter("*ui.rc");
    d->uiFilesList = dir.entryList(QStringList(filter), QDir::Files | QDir::NoSymLinks);

    d->xmlFilesCob = new KComboBox(panel);
    d->xmlFilesCob->setEditable(false);

    foreach(QString uiFile, d->uiFilesList)
    {
        d->xmlFilesCob->addItem(uiFile, uiFile);
    }

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(d->xmlFilesCob);
    panel->setLayout(mainLayout);

    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group(CONFIG_GROUP_NAME);
    QString uiFile            = group.readEntry("UiFile", "kxmlkipicmd_defaultui.rc");
    d->xmlFilesCob->setCurrentIndex(d->uiFilesList.indexOf(uiFile));

    setWidgetResizable(true);
}

SetupXML::~SetupXML()
{
    delete d;
}

void SetupXML::apply()
{
    QString uiFile            = d->xmlFilesCob->itemData(d->xmlFilesCob->currentIndex()).toString();

    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group(CONFIG_GROUP_NAME);
    QString oldUiFile         = group.readEntry("UiFile", "kxmlkipicmd_defaultui.rc");

    if (uiFile != oldUiFile)
    {
        group.writeEntry("UiFile", uiFile);
    }
    group.sync();
}

// -------------------------------------------------------------------

class SetupPlugins::Private
{

public:

    Private() :
        checkAllBtn(0),
        clearListBtn(0),
        kipiConfig(0)
    {
    }

    QPushButton*  checkAllBtn;
    QPushButton*  clearListBtn;
    ConfigWidget* kipiConfig;
};

SetupPlugins::SetupPlugins(QWidget* const parent)
    : QScrollArea(parent), d(new Private)
{
    QWidget *panel = new QWidget(viewport());
    setWidget(panel);
    setWidgetResizable(true);

    QGridLayout* mainLayout = new QGridLayout;
    d->checkAllBtn          = new QPushButton(i18n("Check all"));
    d->clearListBtn         = new QPushButton(i18n("Clear"));

    if (PluginLoader::instance())
    {
        d->kipiConfig = PluginLoader::instance()->configWidget(panel);
        d->kipiConfig->setWhatsThis(i18n("Available KIPI plugins"));
    }

    mainLayout->addWidget(d->checkAllBtn,   0,  0,  1,  1);
    mainLayout->addWidget(d->clearListBtn,  0,  1,  1,  1);
    mainLayout->addWidget(d->kipiConfig,    1,  0,  1, -1);
    mainLayout->setColumnStretch(2, 10);
    mainLayout->setMargin(KDialog::spacingHint());
    mainLayout->setSpacing(KDialog::spacingHint());

    panel->setLayout(mainLayout);

    d->checkAllBtn->setVisible(false);
    d->clearListBtn->setVisible(false);

    // --------------------------------------------------------

    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);
    panel->setAutoFillBackground(false);

    // --------------------------------------------------------

    connect(d->checkAllBtn, SIGNAL(clicked()),
            this, SLOT(slotCheckAll()));

    connect(d->clearListBtn, SIGNAL(clicked()),
            this, SLOT(slotClearList()));

}

SetupPlugins::~SetupPlugins()
{
    delete d;
}

void SetupPlugins::apply()
{
    if (PluginLoader::instance())
    {
        d->kipiConfig->apply();
    }
}

void SetupPlugins::slotCheckAll()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    d->kipiConfig->slotCheckAll();
    QApplication::restoreOverrideCursor();
}

void SetupPlugins::slotClearList()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    d->kipiConfig->slotClear();
    QApplication::restoreOverrideCursor();
}

} // namespace KXMLKipiCmd
