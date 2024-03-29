/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "configwidget.h"

// Qt include

#include <QList>
#include <QHeaderView>

// KF includes

#include <KSharedConfig>
#include <KConfigGroup>

namespace KIPI
{

class PluginCheckBox : public QTreeWidgetItem
{
public:

    PluginCheckBox(PluginLoader::Info* const info, QTreeWidget* const parent)
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
        setText(1, list.join(QString::fromLatin1(", ")));

        // Description
        setText(2, m_info->comment());

        // Author
        setText(3, m_info->author().section(QString::fromLatin1(","), 0, 0));
    };

    ~PluginCheckBox() override
    {
    };

    bool contains(const QString& txt, Qt::CaseSensitivity cs) const
    {
        return (text(0).contains(txt, cs) ||
                text(1).contains(txt, cs) ||
                text(2).contains(txt, cs) ||
                text(3).contains(txt, cs));
    };

public:

    PluginLoader::Info* m_info;
};

// ---------------------------------------------------------------------

class Q_DECL_HIDDEN ConfigWidget::Private
{
public:

    Private()
    {
    };

    QString                filter;
    QList<PluginCheckBox*> boxes;
};

ConfigWidget::ConfigWidget(QWidget* const parent)
    : QTreeWidget(parent),
      d(new Private)
{
    setRootIsDecorated(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAllColumnsShowFocus(true);
    setSortingEnabled(true);
    setColumnCount(4);

    header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(2, QHeaderView::Stretch);
    header()->setSectionResizeMode(3, QHeaderView::Interactive);
    header()->setSortIndicatorShown(true);

    setAutoFillBackground(false);
    viewport()->setAutoFillBackground(false);

    PluginLoader* const loader = PluginLoader::instance();

    if (loader)
    {
        const auto infos = loader->pluginList();
        for (PluginLoader::Info* const info : infos)
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

ConfigWidget::~ConfigWidget() = default;

void ConfigWidget::apply()
{
    if (PluginLoader::instance())
    {
        KSharedConfigPtr config = KSharedConfig::openConfig();
        KConfigGroup group      = config->group(QString::fromLatin1("KIPI/EnabledPlugin"));

        for (PluginCheckBox* const item : std::as_const(d->boxes))
        {
            bool orig = item->m_info->shouldLoad();
            bool load = (item->checkState(0) == Qt::Checked);

            if (orig != load)
            {
                group.writeEntry(item->m_info->uname(), load);
                item->m_info->setShouldLoad(load);

                // See Bug #289779 - Plugins are not really freed / unplugged when disabled in the kipi setup dialog, always call reload()
                // to reload plugins properly when the replug() signal is send.
                item->m_info->reload();
            }
        }

        config->sync();

        Q_EMIT PluginLoader::instance()->replug();
    }
}

void ConfigWidget::selectAll()
{
    for (PluginCheckBox* const item : std::as_const(d->boxes))
    {
        item->setCheckState(0, Qt::Checked);
    }
}

void ConfigWidget::clearAll()
{
    for (PluginCheckBox* const item : std::as_const(d->boxes))
    {
        item->setCheckState(0, Qt::Unchecked);
    }
}

int ConfigWidget::count() const
{
    return d->boxes.count();
}

int ConfigWidget::actived() const
{
    int actived = 0;

    for (PluginCheckBox* const item : std::as_const(d->boxes))
    {
        if (item->checkState(0) == Qt::Checked)
            actived++;
    }

    return actived;
}

int ConfigWidget::visible() const
{
    int visible = 0;

    for (PluginCheckBox* const item : std::as_const(d->boxes))
    {
        if (!item->isHidden())
            visible++;
    }

    return visible;
}

void ConfigWidget::setFilter(const QString& filter, Qt::CaseSensitivity cs)
{
    d->filter  = filter;
    bool query = false;

    for (PluginCheckBox* const item : std::as_const(d->boxes))
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

    Q_EMIT signalSearchResult(query);
}

QString ConfigWidget::filter() const
{
    return d->filter;
}

} // namespace KIPI

#include "moc_configwidget.cpp"
