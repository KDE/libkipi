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
        kipiConfig(0)
    {
    }

    void updateInfo();

public:

    QLabel*         pluginsNumber;
    QLabel*         pluginsNumberActivated;

    QPushButton*    checkAllBtn;
    QPushButton*    clearBtn;

    PluginListView* kipiConfig;
};

void ConfigWidget::Private::updateInfo()
{
    pluginsNumber->setText(i18np("1 Kipi plugin found",
                                    "%1 Kipi plugins found",
                                    kipiConfig->count()));

    pluginsNumberActivated->setText(i18nc("%1: number of plugins activated",
                                          "(%1 activated)", kipiConfig->actived()));
}

ConfigWidget::ConfigWidget(QWidget* const parent)
    : QScrollArea(parent), d(new Private)
{
    QWidget* panel = new QWidget(viewport());
    setWidget(panel);
    setWidgetResizable(true);

    QGridLayout* mainLayout   = new QGridLayout(panel);
    d->pluginsNumber          = new QLabel(panel);
    d->pluginsNumberActivated = new QLabel(panel);
    d->checkAllBtn            = new QPushButton(i18n("Check all"), panel);
    d->clearBtn               = new QPushButton(i18n("Clear"), panel);
    d->kipiConfig             = new PluginListView(panel);
    d->kipiConfig->setWhatsThis(i18n("List of available Kipi plugins."));

    mainLayout->addWidget(d->pluginsNumber,             0, 0, 1, 1);
    mainLayout->addWidget(d->pluginsNumberActivated,    0, 1, 1, 1);
    mainLayout->addWidget(d->checkAllBtn,               0, 3, 1, 1);
    mainLayout->addWidget(d->clearBtn,                  0, 4, 1, 1);
    mainLayout->addWidget(d->kipiConfig,                1, 0, 1, -1);
    mainLayout->setColumnStretch(2, 10);
    mainLayout->setMargin(KDialog::spacingHint());
    mainLayout->setSpacing(KDialog::spacingHint());

    // --------------------------------------------------------

    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);
    panel->setAutoFillBackground(false);

    // --------------------------------------------------------

    connect(d->checkAllBtn, SIGNAL(clicked()),
            this, SLOT(slotCheckAll()));

    connect(d->clearBtn, SIGNAL(clicked()),
            this, SLOT(slotClearList()));

    connect(d->kipiConfig, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this, SLOT(slotItemClicked()));

    // --------------------------------------------------------

    d->updateInfo();
}

ConfigWidget::~ConfigWidget()
{
    delete d;
}

void ConfigWidget::apply()
{
    if (PluginLoader::instance())
    {
        d->kipiConfig->apply();
        emit PluginLoader::instance()->replug();
    }
}

void ConfigWidget::slotCheckAll()
{
    d->kipiConfig->slotCheckAll();
    d->updateInfo();
}

void ConfigWidget::slotClearList()
{
    d->kipiConfig->slotClear();
    d->updateInfo();
}

void ConfigWidget::slotItemClicked()
{
    d->updateInfo();
}

} // namespace KIPI
