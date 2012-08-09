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

// Qt includes

#include <QTreeWidgetItem>
#include <QTreeWidget>

// Local includes

#include "plugin.h"
#include "pluginloader.h"

namespace KIPI
{

class PluginCheckBox : public QTreeWidgetItem
{
public:

    PluginCheckBox(PluginLoader::Info* const info, QTreeWidget* const parent);
    ~PluginCheckBox();

    bool contains(const QString& txt, Qt::CaseSensitivity cs) const;

public:

    PluginLoader::Info* m_info;
};

// ----------------------------------------------------------------------------------

class PluginListView : public QTreeWidget
{
    Q_OBJECT

public:

    PluginListView(QWidget* const parent);
    ~PluginListView();

    int actived() const;
    int count()   const;
    int visible() const;

    void    setFilter(const QString& filter, Qt::CaseSensitivity cs);
    QString filter() const;

Q_SIGNALS:

    void signalSearchResult(bool);

public Q_SLOTS:

    void slotApply();
    void slotCheckAll();
    void slotClear();

private:

    class Private;
    Private* const d;
};

} // namespace KIPI
