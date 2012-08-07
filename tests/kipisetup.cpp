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
#include <klineedit.h>

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
        pluginFilter(0),
        page_plugins(0),
        page_xml(0),
        pluginsPage(0),
        xmlPage(0)
    {
    }

    KLineEdit*       pluginFilter;

    KPageWidgetItem* page_plugins;
    KPageWidgetItem* page_xml;

    ConfigWidget*    pluginsPage;
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

    d->pluginsPage  = PluginLoader::instance()->configWidget(this);
    d->page_plugins = addPage(d->pluginsPage, i18n("Kipi Plugins"));
    d->page_plugins->setIcon(KIcon("kipi"));
    d->pluginFilter = new KLineEdit(d->pluginsPage);
    d->pluginFilter->setClearButtonShown(true);
    d->pluginFilter->setClickMessage(i18n("filter..."));
    d->pluginsPage->setFilterWidget(d->pluginFilter);

    d->xmlPage  = new SetupXML(this);
    d->page_xml = addPage(d->xmlPage, i18n("UI layouts"));
    d->page_xml->setIcon(KIcon("application-xml"));
    d->page_xml->setHeader("Configure the UI file for the KXMLKipiCmd application");

    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup group        = config->group("Setup Dialog");
    restoreDialogSize(group);

    int pageIndex         = group.readEntry("Setup Page", 0);
    KPageWidgetItem* page = 0;

    if (pageIndex == XmlFilesPage)
    {
        page = d->page_xml;
    }
    else
    {
        page = d->page_plugins;
    }

    setCurrentPage(page);

    connect(d->pluginFilter, SIGNAL(userTextChanged(QString)),
            this, SLOT(slotFilterChanged(QString)));
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

void KipiSetup::slotFilterChanged(const QString& filter)
{
     d->pluginsPage->slotSetFilter(filter, Qt::CaseInsensitive);
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

} // namespace KXMLKipiCmd
