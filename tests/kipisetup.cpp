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

// Loacal includes

#include "kipitestpluginloader.h"

using namespace KIPI;

namespace KXMLKipiCmd
{

class KipiSetup::KipiSetupPriv
{

public:

    KipiSetupPriv() :
        page_plugins(0),
        page_xml(0),
        pluginsPage(0),
        xmlPage(0)
    {
    }

    KPageWidgetItem* page_plugins;
    KPageWidgetItem* page_xml;

    ConfigWidget*    pluginsPage;
    SetupXML*        xmlPage;
};

KipiSetup::KipiSetup(QWidget* const parent)
    : KPageDialog(parent), d(new KipiSetupPriv)
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

    d->xmlPage  = new SetupXML(this);
    d->page_xml = addPage(d->xmlPage, i18n("UI layouts"));
    d->page_xml->setIcon(KIcon("application-xml"));
    d->page_xml->setHeader("Configure the UI file for the KXMLKipiCmd application");
}

KipiSetup::~KipiSetup()
{
    delete d;
}

bool KipiSetup::exec(QWidget *parent)
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

// -------------------------------------------------------------------

class SetupXML::SetupXMLPriv
{
public:

    SetupXMLPriv()
    {
        xmlFilesCob = 0;
    }

    QString        uiFilesPath;
    QList<QString> uiFilesList;
    KComboBox*     xmlFilesCob;
};

SetupXML::SetupXML(QWidget* const parent)
    : QScrollArea(parent), d(new SetupXMLPriv)
{
    QWidget* panel = new QWidget(viewport());
    setWidget(panel);
    setWidgetResizable(true);

    d->uiFilesPath = KGlobal::dirs()->findDirs("data", KGlobal::mainComponent().componentName()).last();
    QDir dir(d->uiFilesPath);
    QString filter("*ui.rc");
    d->uiFilesList = dir.entryList(QStringList(filter), QDir::Files | QDir::NoSymLinks);
    kDebug() << d->uiFilesList;

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

    setWidgetResizable(true);
}

SetupXML::~SetupXML()
{
    delete d;
}

void SetupXML::apply()
{
}

} // namespace KXMLKipiCmd
