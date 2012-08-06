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
#include <QPalette>
#include <QFont>

// KDE includes

#include <kapplication.h>
#include <ksharedconfig.h>
#include <kconfiggroup.h>
#include <kconfig.h>

namespace KIPI
{

PluginCategoryItem::PluginCategoryItem(Category cat, QTreeWidget* const parent)
    : QTreeWidgetItem(parent),
      m_cat(cat)
{
    // Reset all item flags: item is not selectable.
    setFlags(Qt::ItemIsEnabled);
    setDisabled(false);
    setExpanded(true);
    setFirstColumnSpanned(true);

    setTextAlignment(0, Qt::AlignCenter);
    QFont fn0(font(0));
    fn0.setBold(true);
    fn0.setItalic(false);
    setFont(0, fn0);

    setBackground(0, QBrush(kapp->palette().color(QPalette::Highlight)));
    setForeground(0, QBrush(kapp->palette().color(QPalette::HighlightedText)));

    switch(m_cat)
    {
        case ToolsPlugin:
            setText(0, i18n("Tool Plugins"));
            break;
        case ImportPlugin:
            setText(0, i18n("Import Plugins"));
            break;
        case ExportPlugin:
            setText(0, i18n("Export Plugins"));
            break;
        case BatchPlugin:
            setText(0, i18n("Batch Plugins"));
            break;
        case CollectionsPlugin:
            setText(0, i18n("Collection Plugins"));
            break;
        default:   // ImagesPlugin
            setText(0, i18n("Image Plugins"));
            break;
    }
}

PluginCategoryItem::~PluginCategoryItem()
{
}

// ---------------------------------------------------------------------
    
PluginCheckBox::PluginCheckBox(PluginLoader::Info* const info, PluginCategoryItem* const parent)
    : QTreeWidgetItem(parent),
      m_info(info)
{
    setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    setDisabled(false);

    setText(0, m_info->name());
    setText(1, m_info->comment());
    setIcon(0, m_info->icon());
    setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    setCheckState(0, m_info->shouldLoad() ? Qt::Checked : Qt::Unchecked);
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

    QList<PluginCategoryItem*> categories;
    QList<PluginCheckBox*>     boxes;
};

PluginListView::PluginListView(QWidget* const parent)
    : QTreeWidget(parent),
      d(new Private)
{
    setRootIsDecorated(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAllColumnsShowFocus(true);
    setColumnCount(2);

    QStringList labels;
    labels.append( i18n("Name") );
    labels.append( i18n("Description") );
    setHeaderLabels(labels);
    header()->setResizeMode(0, QHeaderView::ResizeToContents);
    header()->setResizeMode(1, QHeaderView::Stretch);
    
    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);

    foreach(PluginLoader::Info* const info, PluginLoader::instance()->pluginList())
    {
        Category cat = info->plugin()->category(0);

        PluginCategoryItem* parent = 0;
        
        foreach(PluginCategoryItem* const item, d->categories)
        {
            if (item->m_cat == cat)
            {
                parent = item;
                break;
            }
        }

        if (!parent)
        {
            parent = new PluginCategoryItem(cat, this);
            d->categories.append(parent);
        }
        
        PluginCheckBox* cb = new PluginCheckBox(info, parent);
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
