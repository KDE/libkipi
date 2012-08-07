/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-08-06
 * @brief  plugin config widget
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2012 by Victor Dodon
 *         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "configwidget.moc"

// Qt include.

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>

// KDE includes

#include <kdialog.h>

// Local includes

#include "configwidget_p.h"
#include "pluginloader.h"

namespace KIPI
{

class ConfigWidget::Private
{

public:

    Private() :
        pluginsNumber(0),
        pluginsNumberActivated(0),
        checkAllBtn(0),
        clearBtn(0),
        grid(0),
        pluginsList(0)
    {
    }

    void updateInfo();

public:

    QLabel*         pluginsNumber;
    QLabel*         pluginsNumberActivated;

    QPushButton*    checkAllBtn;
    QPushButton*    clearBtn;

    QGridLayout*    grid;

    PluginListView* pluginsList;
};

void ConfigWidget::Private::updateInfo()
{
    if (pluginsList->filter().isEmpty())
    {
        pluginsNumber->setText(i18np("1 Kipi plugin found",
                                     "%1 Kipi plugins found",
                                     pluginsList->count()));

        pluginsNumberActivated->setText(i18nc("%1: number of plugins activated",
                                              "(%1 activated)",
                                              pluginsList->actived()));
    }
    else
    {
        pluginsNumber->setText(i18n("List filtered..."));
        pluginsNumberActivated->setText(QString());
    }
}

ConfigWidget::ConfigWidget(QWidget* const parent)
    : QScrollArea(parent), d(new Private)
{
    QWidget* panel            = new QWidget(viewport());
    d->grid                   = new QGridLayout(panel);
    d->pluginsNumber          = new QLabel(panel);
    d->pluginsNumberActivated = new QLabel(panel);
    d->checkAllBtn            = new QPushButton(i18n("Check all"), panel);
    d->clearBtn               = new QPushButton(i18n("Clear"), panel);
    d->pluginsList            = new PluginListView(panel);
    d->pluginsList->setWhatsThis(i18n("List of available Kipi plugins."));

    d->grid->addWidget(d->pluginsNumber,             0, 1, 1, 1);
    d->grid->addWidget(d->pluginsNumberActivated,    0, 2, 1, 1);
    d->grid->addWidget(d->checkAllBtn,               0, 4, 1, 1);
    d->grid->addWidget(d->clearBtn,                  0, 5, 1, 1);
    d->grid->addWidget(d->pluginsList,               1, 0, 1, -1);
    d->grid->setColumnStretch(3, 5);
    d->grid->setMargin(KDialog::spacingHint());
    d->grid->setSpacing(KDialog::spacingHint());

    // --------------------------------------------------------

    setWidget(panel);
    setWidgetResizable(true);
    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);
    panel->setAutoFillBackground(false);

    // --------------------------------------------------------

    connect(d->checkAllBtn, SIGNAL(clicked()),
            this, SLOT(slotCheckAll()));

    connect(d->clearBtn, SIGNAL(clicked()),
            this, SLOT(slotClearList()));

    connect(d->pluginsList, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(slotItemClicked()));

    connect(d->pluginsList, SIGNAL(signalItemsFiltered(int)),
            this, SIGNAL(signalItemsFiltered(int)));

    // --------------------------------------------------------

    d->updateInfo();
}

ConfigWidget::~ConfigWidget()
{
    delete d;
}

void ConfigWidget::setFilterWidget(QWidget* const wdg)
{
    d->grid->addWidget(wdg, 0, 0, 1, 1);
}

void ConfigWidget::apply()
{
    if (PluginLoader::instance())
    {
        d->pluginsList->slotApply();
        emit PluginLoader::instance()->replug();
    }
}

void ConfigWidget::slotCheckAll()
{
    d->pluginsList->slotCheckAll();
    d->updateInfo();
}

void ConfigWidget::slotClearList()
{
    d->pluginsList->slotClear();
    d->updateInfo();
}

void ConfigWidget::slotItemClicked()
{
    d->updateInfo();
}

void ConfigWidget::slotSetFilter(const QString& filter)
{
    d->pluginsList->setFilter(filter);
    d->updateInfo();
}

} // namespace KIPI
