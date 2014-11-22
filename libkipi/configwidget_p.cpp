/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-08-06
 * @brief  private implementation of plugin config widget
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

// Qt include

#include <QList>
#include <QHeaderView>

// KDE includes

#include <ksharedconfig.h>
#include <kconfiggroup.h>
#include <kconfig.h>
#include <kaction.h>
#include <kdebug.h>

namespace KIPI
{

PluginCheckBox::PluginCheckBox(PluginLoader::Info* const info, QTreeWidget* const parent)
    : QTreeWidgetItem(parent),
      m_info(info)
{
    setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    setDisabled(false);

    // Name + Icon + Selector
    setText(0, m_info->name());
    setIcon(0, m_info->icon());
    setCheckState(0, m_info->shouldLoad() ? Qt::Checked : Qt::Unchecked);

    // Categories
    QStringList list = m_info->pluginCategories();
    list.removeDuplicates();
    list.sort();
    setText(1, list.join(", "));

    // Description
    setText(2, m_info->comment());

    // Author
    setText(3, m_info->author().section(',', 0, 0));
}

PluginCheckBox::~PluginCheckBox()
{
}

bool PluginCheckBox::contains(const QString& txt, Qt::CaseSensitivity cs) const
{
    return (text(0).contains(txt, cs) ||
            text(1).contains(txt, cs) ||
            text(2).contains(txt, cs) ||
            text(3).contains(txt, cs));
}

// ---------------------------------------------------------------------

class PluginListView::Private
{
public:

    Private()
    {
    };

    QString                filter;
    QList<PluginCheckBox*> boxes;
};

PluginListView::PluginListView(QWidget* const parent)
    : QTreeWidget(parent),
      d(new Private)
{
    setRootIsDecorated(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAllColumnsShowFocus(true);
    setSortingEnabled(true);
    setColumnCount(4);

    QStringList labels;
    labels.append(i18n("Name"));
    labels.append(i18n("Categories"));
    labels.append(i18n("Description"));
    labels.append(i18n("Author"));

    setHeaderLabels(labels);
    header()->setResizeMode(0, QHeaderView::ResizeToContents);
    header()->setResizeMode(1, QHeaderView::ResizeToContents);
    header()->setResizeMode(2, QHeaderView::Stretch);
    header()->setResizeMode(3, QHeaderView::Interactive);
    header()->setSortIndicatorShown(true);

    PluginLoader* const loader = PluginLoader::instance();

    if (loader)
    {
        foreach(PluginLoader::Info* const info, loader->pluginList())
        {
            if (info)
            {
                d->boxes.append(new PluginCheckBox(info, this));
            }
        }
    }

    // Sort items by plugin names.
    sortItems(0, Qt::AscendingOrder);
}

PluginListView::~PluginListView()
{
    delete d;
}

void PluginListView::slotApply()
{
    KSharedConfigPtr config = KGlobal::config();
    KConfigGroup group      = config->group(QString::fromLatin1("KIPI/EnabledPlugin"));

    foreach (PluginCheckBox* const item, d->boxes)
    {
        bool orig = item->m_info->shouldLoad();
        bool load = (item->checkState(0) == Qt::Checked);

        if (orig != load)
        {
            group.writeEntry(item->m_info->uname(), load);
            item->m_info->setShouldLoad(load);

            // Bugfix #289779 - Plugins are not really freed / unplugged when disabled in the kipi setup dialog, always call reload()
            // to reload plugins properly when the replug() signal is send.
            item->m_info->reload();
        }
    }

    config->sync();
}

void PluginListView::slotCheckAll()
{
    foreach (PluginCheckBox* const item, d->boxes)
    {
        item->setCheckState(0, Qt::Checked);
    }
}

void PluginListView::slotClear()
{
    foreach (PluginCheckBox* const item, d->boxes)
    {
        item->setCheckState(0, Qt::Unchecked);
    }
}

int PluginListView::count() const
{
    return d->boxes.count();
}

int PluginListView::actived() const
{
    int actived = 0;

    foreach (PluginCheckBox* const item, d->boxes)
    {
        if (item->checkState(0) == Qt::Checked)
            actived++;
    }

    return actived;
}

int PluginListView::visible() const
{
    int visible = 0;

    foreach (PluginCheckBox* const item, d->boxes)
    {
        if (!item->isHidden())
            visible++;
    }

    return visible;
}

void PluginListView::setFilter(const QString& filter, Qt::CaseSensitivity cs)
{
    d->filter  = filter;
    bool query = false;

    foreach (PluginCheckBox* const item, d->boxes)
    {
        if (item->contains(filter, cs))
        {
            query = true;
            item->setHidden(false);
        }
        else
        {
            item->setHidden(true);
        }
    }

    emit signalSearchResult(query);
}

QString PluginListView::filter() const
{
    return d->filter;
}

} // namespace KIPI
