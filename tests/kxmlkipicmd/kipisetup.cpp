/*
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>
    SPDX-FileCopyrightText: 2012-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kipisetup.h"

// Qt includes

#include <QList>
#include <QRadioButton>
#include <QPushButton>
#include <QDir>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QScrollArea>
#include <QApplication>
#include <QComboBox>
#include <QLineEdit>
#include <QStyle>
#include <QStandardPaths>
#include <QDialogButtonBox>

// KF includes

#include <KSharedConfig>
#include <KConfigGroup>
#include <KWindowConfig>

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
        pluginFilter(nullptr),
        buttons(nullptr),
        tabView(nullptr),
        pluginsList(nullptr),
        xmlPage(nullptr)
    {
    }

    QLineEdit*        pluginFilter;

    QDialogButtonBox* buttons;
    QTabWidget*       tabView;

    ConfigWidget*     pluginsList;
    SetupXML*         xmlPage;
};

KipiSetup::KipiSetup(QWidget* const parent)
    : QDialog(parent),
      d(new Private)
{
    setWindowTitle(QLatin1String("Configure"));
    setModal(true);
    setMinimumSize(600, 400);

    const int spacing = QApplication::style()->pixelMetric(QStyle::PM_DefaultLayoutSpacing);

    d->buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    d->tabView = new QTabWidget(this);

    QScrollArea* const sv   = new QScrollArea(this);
    QWidget* const panel    = new QWidget(sv->viewport());
    QGridLayout* const grid = new QGridLayout(panel);

    d->pluginFilter = new QLineEdit(panel);
    d->pluginFilter->setClearButtonEnabled(true);
    d->pluginFilter->setPlaceholderText(QLatin1String("Plugins list filter."));

    d->pluginsList  = PluginLoader::instance()->configWidget(d->tabView);
    d->pluginsList->setToolTip(QLatin1String("Configure plugins"));

    QStringList labels;
    labels.append(QLatin1String("Name"));
    labels.append(QLatin1String("Categories"));
    labels.append(QLatin1String("Description"));
    labels.append(QLatin1String("Author"));
    d->pluginsList->setHeaderLabels(labels);

    grid->addWidget(d->pluginFilter, 0, 0, 1, 1);
    grid->addWidget(d->pluginsList,  1, 0, 1, 1);
    grid->setRowStretch(1, 10);
    grid->setContentsMargins(spacing, spacing, spacing, spacing);
    grid->setSpacing(spacing);

    sv->setWidget(panel);
    sv->setWidgetResizable(true);

    d->tabView->insertTab(KipiPluginsPage, sv, QIcon(QLatin1String(":/icons/kipi-icon.svg")), QLatin1String("Kipi Plugins"));

    d->xmlPage = new SetupXML(d->tabView);
    d->xmlPage->setToolTip(QLatin1String("Configure the UI file for the KXMLKipiCmd application"));
    d->tabView->insertTab(XmlFilesPage, d->xmlPage, QIcon::fromTheme(QString::fromLatin1("application-xml")), QLatin1String("UI layouts"));

    QVBoxLayout* const vbx = new QVBoxLayout(this);
    vbx->addWidget(d->tabView);
    vbx->addWidget(d->buttons);
    setLayout(vbx);

    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group        = config->group("Setup Dialog");
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    d->tabView->setCurrentIndex(group.readEntry("Setup Page", 0));

    connect(d->pluginFilter, &QLineEdit::textChanged,
            this, &KipiSetup::slotFilterChanged);

    connect(d->buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked,
            this, &KipiSetup::slotOkClicked);

    connect(d->buttons->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
            this, &KipiSetup::close);
}

KipiSetup::~KipiSetup()
{
    KSharedConfig::Ptr config = KSharedConfig::openConfig();
    KConfigGroup group        = config->group(QString::fromLatin1("Setup Dialog"));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.writeEntry("Setup Page", d->tabView->currentIndex());
    config->sync();

    delete d;
}

void KipiSetup::slotFilterChanged(const QString& filter)
{
     d->pluginsList->setFilter(filter, Qt::CaseInsensitive);
}

bool KipiSetup::runSetupDialog(QWidget* const parent)
{
    QPointer<KipiSetup> setup = new KipiSetup(parent);
    bool success              = (setup->exec() == QDialog::Accepted);
    delete setup;

    return success;
}

void KipiSetup::slotOkClicked()
{
    qDebug() << "Ok clicked";

    QApplication::setOverrideCursor(Qt::WaitCursor);

    d->pluginsList->apply();
    d->xmlPage->apply();

    QApplication::restoreOverrideCursor();
    accept();
}

// -------------------------------------------------------------------

class SetupXML::Private
{
public:

    Private()
    {
        xmlFilesCob = nullptr;
    }

    QString        uiFilesPath;
    QList<QString> uiFilesList;
    QComboBox*     xmlFilesCob;
};

SetupXML::SetupXML(QWidget* const parent)
    : QScrollArea(parent),
      d(new Private)
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

    for (const QString &uiFile : std::as_const(d->uiFilesList))
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
