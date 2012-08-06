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

#include "configwidget_p.moc"

// Qt include.

#include <QList>

// KDE includes

#include <ksharedconfig.h>
#include <kconfiggroup.h>
#include <kconfig.h>

namespace KIPI
{

PluginCheckBox::PluginCheckBox(PluginLoader::Info* const info, QListWidget* const parent)
    : QListWidgetItem(parent, QListWidgetItem::UserType),
        m_info(info)
{
    setText(QString("%1  (%2)").arg(m_info->name(), m_info->comment()));
    setIcon(m_info->icon());
    setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    setCheckState(m_info->shouldLoad() ? Qt::Checked : Qt::Unchecked);
}

PluginCheckBox::~PluginCheckBox()
{
}


// ---------------------------------------------------------------------

class PluginListView::Private
{
public:

    Private()
    {
    };

    QList<PluginCheckBox*> boxes;
};

PluginListView::PluginListView(QWidget* const parent)
    : QListWidget(parent),
      d(new Private)
{
    setAutoFillBackground(false);
    setSortingEnabled(true);
    viewport()->setAutoFillBackground(false);

    foreach(PluginLoader::Info* const info, PluginLoader::instance()->pluginList())
    {
        PluginCheckBox* cb = new PluginCheckBox(info, this);
        d->boxes.append(cb);
    }
}

PluginListView::~PluginListView()
{
    delete d;
}

void PluginListView::apply()
{
    KSharedConfigPtr config = KGlobal::config();
    KConfigGroup group      = config->group(QString::fromLatin1("KIPI/EnabledPlugin"));

    for (QList<PluginCheckBox*>::Iterator it = d->boxes.begin(); it != d->boxes.end(); ++it)
    {
        bool orig = (*it)->m_info->shouldLoad();
        bool load = ((*it)->checkState() == Qt::Checked);

        if (orig != load)
        {
            group.writeEntry((*it)->m_info->name(), load);
            (*it)->m_info->setShouldLoad(load);

            // Bugfix #289779 - Plugins are not really freed / unplugged when disabled in the kipi setup dialog, always call reload()
            // to reload plugins properly when the replug() signal is send.
            /*
            if (load)
            {
                (*it)->info->reload();
            }
            else if ((*it)->info->plugin())   // Do not emit if we had trouble loading plugin.
            {
                emit PluginLoader::instance()->unplug((*it)->info);
            }
            */
            (*it)->m_info->reload();
        }
    }
}

void PluginListView::slotCheckAll()
{
    for (int i = 0; i < count(); ++i)
    {
        item(i)->setCheckState(Qt::Checked);
    }
}

void PluginListView::slotClear()
{
    for (int i = 0; i < count(); ++i)
    {
        item(i)->setCheckState(Qt::Unchecked);
    }
}

int PluginListView::actived() const
{
    int actived = 0;

    for (int i = 0; i < count(); ++i)
    {
        if (item(i)->checkState() == Qt::Checked)
            actived++;
    }

    return actived;
}

} // namespace KIPI
