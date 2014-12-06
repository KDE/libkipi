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

#include "kipisetup.h"

// Qt includes

#include <QList>
#include <QRadioButton>
#include <QPushButton>
#include <QDir>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QStandardPaths>

// KDE includes

#include <klocalizedstring.h>
#include <ksharedconfig.h>
#include <kconfiggroup.h>
#include <kwindowconfig.h>

// Libkipi includes

#include "pluginloader.h"
#include "configwidget.h"

// Local includes

#include "kipitestpluginloader.h"

using namespace KIPI;

namespace KXMLKipiCmd
{

const QString SetupXML::CONFIG_GROUP_NAME = QString::fromLatin1("UI Settings");

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

    QLineEdit*       pluginFilter;

    KPageWidgetItem* page_plugins;
    KPageWidgetItem* page_xml;

    ConfigWidget*    pluginsPage;
    SetupXML*        xmlPage;
};

KipiSetup::KipiSetup(QWidget* const parent)
    : KPageDialog(parent), d(new Private)
{
    setWindowTitle(i18n("Configure"));
    setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    setFaceType(List);
    setModal(true);
    setMinimumSize(600, 400);

    d->pluginsPage  = PluginLoader::instance()->configWidget(this);
    d->page_plugins = addPage(d->pluginsPage, i18n("Kipi Plugins"));
    d->page_plugins->setIcon(QIcon::fromTheme(QString::fromLatin1("kipi")));
    d->pluginFilter = new QLineEdit(d->pluginsPage);
    d->pluginFilter->setClearButtonEnabled(true);
    d->pluginFilter->setToolTip(i18n("Plugins list filter."));
    d->pluginsPage->setFilterWidget(d->pluginFilter);

    d->xmlPage  = new SetupXML(this);
    d->page_xml = addPage(d->xmlPage, i18n("UI layouts"));
    d->page_xml->setIcon(QIcon::fromTheme(QString::fromLatin1("application-xml")));
    d->page_xml->setHeader(QString::fromLatin1("Configure the UI file for the KXMLKipiCmd application"));

    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group        = config->group("Setup Dialog");
    KWindowConfig::restoreWindowSize(windowHandle(), group);

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

    connect(d->pluginFilter, &QLineEdit::textChanged,
            this, &KipiSetup::slotFilterChanged);

    connect(buttonBox()->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &KipiSetup::slotOkClicked);
}

KipiSetup::~KipiSetup()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group        = config->group(QString::fromLatin1("Setup Dialog"));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.writeEntry("Setup Page", (int)activePageIndex());
    config->sync();

    delete d;
}

void KipiSetup::slotFilterChanged(const QString& filter)
{
     d->pluginsPage->slotSetFilter(filter, Qt::CaseInsensitive);
}

bool KipiSetup::runSetupDialog(QWidget* const parent)
{
    QPointer<KipiSetup> setup = new KipiSetup(parent);
    bool success              = (setup->KPageDialog::exec() == QDialog::Accepted);
    delete setup;

    return success;
}

void KipiSetup::slotOkClicked()
{
    qDebug() << "Ok clicked";

    QApplication::setOverrideCursor(Qt::WaitCursor);

    d->pluginsPage->apply();
    d->xmlPage->apply();

    QApplication::restoreOverrideCursor();
    accept();
}

int KipiSetup::activePageIndex()
{
    KPageWidgetItem* const curr = currentPage();

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
    QComboBox*     xmlFilesCob;
};

SetupXML::SetupXML(QWidget* const parent)
    : QScrollArea(parent), d(new Private)
{
    QWidget* const panel = new QWidget(viewport());
    setWidget(panel);
    setWidgetResizable(true);

    d->uiFilesPath = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QApplication::applicationName(), QStandardPaths::LocateDirectory).last();
    qDebug() << d->uiFilesPath;
    QDir dir(d->uiFilesPath);
    QString filter = QString::fromLatin1("*ui.rc");
    d->uiFilesList = dir.entryList(QStringList(filter), QDir::Files | QDir::NoSymLinks);

    d->xmlFilesCob = new QComboBox(panel);
    d->xmlFilesCob->setEditable(false);

    foreach(QString uiFile, d->uiFilesList)
    {
        d->xmlFilesCob->addItem(uiFile, uiFile);
    }

    QVBoxLayout* const mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(d->xmlFilesCob);
    panel->setLayout(mainLayout);

    KSharedConfig::Ptr config = KSharedConfig::openConfig();
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

    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group        = config->group(CONFIG_GROUP_NAME);
    QString oldUiFile         = group.readEntry("UiFile", "kxmlkipicmd_defaultui.rc");

    if (uiFile != oldUiFile)
    {
        group.writeEntry("UiFile", uiFile);
    }
    group.sync();
}

} // namespace KXMLKipiCmd
