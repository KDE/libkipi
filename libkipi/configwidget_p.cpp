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
#include <QHeaderView>

// KDE includes

#include <ksharedconfig.h>
#include <kconfiggroup.h>
#include <kconfig.h>
#include <kaction.h>

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
    QStringList list;

    foreach (KAction* const action, info->plugin()->actions())
    {
        Category cat = info->plugin()->category(action);

        switch(cat)
        {
            case ToolsPlugin:
                list << i18n("Tool");
                break;
            case ImportPlugin:
                list << i18n("Import");
                break;
            case ExportPlugin:
                list << i18n("Export");
                break;
            case BatchPlugin:
                list << i18n("Batch");
                break;
            case CollectionsPlugin:
                list << i18n("Collection");
                break;
            default:   // ImagesPlugin
                list << i18n("Image");
                break;
        }
    }

    list.removeDuplicates();
    list.sort();
    setText(1, list.join(", "));

    // Description
    setText(2, m_info->comment());
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
    : QTreeWidget(parent),
      d(new Private)
{
    setRootIsDecorated(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAllColumnsShowFocus(true);
    setSortingEnabled(true);
    setColumnCount(3);

    QStringList labels;
    labels.append( i18n("Name") );
    labels.append( i18n("Categories") );
    labels.append( i18n("Description") );

    setHeaderLabels(labels);
    header()->setResizeMode(0, QHeaderView::ResizeToContents);
    header()->setResizeMode(1, QHeaderView::ResizeToContents);
    header()->setResizeMode(2, QHeaderView::Stretch);
    header()->setSortIndicatorShown(true);

    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);

    foreach(PluginLoader::Info* const info, PluginLoader::instance()->pluginList())
    {
        PluginCheckBox* cb = new PluginCheckBox(info, this);
        d->boxes.append(cb);
    }

    // Sort items by plugin names.
    sortItems(0, Qt::AscendingOrder);
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
        bool load = ((*it)->checkState(0) == Qt::Checked);

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

} // namespace KIPI
