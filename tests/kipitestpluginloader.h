/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2012-06-20
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

#ifndef KIPITESTPLUGINLOADER_H
#define KIPITESTPLUGINLOADER_H

// Qt includes

#include <QObject>
#include <QList>
#include <QString>

// Libkipi includes

#include "plugin.h"

class QAction;

class KActionCollection;

namespace KXMLKipiCmd
{

class KipiTestPluginLoader : public QObject
{
    Q_OBJECT

public:

    KipiTestPluginLoader(QObject* const parent);

    QList<QAction*>    kipiActionsByCategory(KIPI::Category cat) const;

    KActionCollection* pluginsActionCollection() const;

    void kipiPlugActions(bool unplug = false);

    static KipiTestPluginLoader* instance();

private Q_SLOTS:

    /** Called by KIPI::PluginLoader when plugins list must be reloaded
     */
    void slotKipiPluginsPlug();

private:

    ~KipiTestPluginLoader();

    void loadPlugins();

    void checkEmptyCategory(KIPI::Category cat);

    QString categoryName(KIPI::Category cat) const;

private:

    static KipiTestPluginLoader* m_instance;

    class KipiTestPluginLoaderPriv;
    KipiTestPluginLoaderPriv* const d;
};

} // namespace KXMLKipiCmd

#endif // KIPITESTPLUGINLOADER_H
