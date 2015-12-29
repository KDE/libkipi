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
 * @author Copyright (C) 2012-2016 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
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

#include "kipitestpluginloader.h"

// Qt includes

#include <QMap>
#include <QAction>
#include <QDebug>

// KDE includes

#include <kactioncategory.h>
#include <kxmlguifactory.h>
#include <kxmlguiwindow.h>

// Local includes

#include "kipiinterface.h"
#include "kipitestmainwindow.h"

namespace KXMLKipiCmd
{

class KipiTestPluginLoader::Private
{

public:

    Private()
    {
        app                         = 0;
        kipipluginsActionCollection = 0;
        kipiPluginLoader            = 0;
        kipiInterface               = 0;
    }

    PluginLoader*               kipiPluginLoader;
    KipiInterface*              kipiInterface;
    KXmlGuiWindow*              app;

    KActionCollection*          kipipluginsActionCollection;
    QMap<int, KActionCategory*> kipiCategoryMap;
};

// -- Static values -------------------------------

KipiTestPluginLoader* KipiTestPluginLoader::m_instance = 0;

// -----------------------------------------------

KipiTestPluginLoader::KipiTestPluginLoader(KXmlGuiWindow* const parent, KipiInterface *const interface)
    : QObject(parent),
      d(new Private)
{
    m_instance       = this;
    d->kipiInterface = interface;
    d->app           = parent;

    loadPlugins();
}

KipiTestPluginLoader::~KipiTestPluginLoader()
{
    delete d;
    m_instance = 0;
}

KipiTestPluginLoader* KipiTestPluginLoader::instance()
{
    return m_instance;
}

KActionCollection* KipiTestPluginLoader::pluginsActionCollection() const
{
    return d->kipipluginsActionCollection;
}

QList<PluginLoader::Info*> KipiTestPluginLoader::pluginList()
{
    return d->kipiPluginLoader->pluginList();
}

QList<QAction*> KipiTestPluginLoader::kipiActionsByCategory(Category cat) const
{
    KActionCategory* const category = d->kipiCategoryMap[cat];

    if (category)
    {
        return category->actions();
    }

    return QList<QAction*>();
}

void KipiTestPluginLoader::loadPlugins()
{
    d->kipipluginsActionCollection = new KActionCollection(d->app, QLatin1String("kipitest"));

    QStringList ignores;

    // List of obsoletes plugins to not load

    // Comment the following two lines if you want to load the HelloWorld and
    // the KXMLHelloWorld plugins for testing purposes
    //ignores.append("HelloWorld");
    //ignores.append("KXMLHelloWorld");

    ignores.append(QString::fromLatin1("KameraKlient"));

    // These plugins have been renamed with 0.2.0-rc1
    ignores.append(QString::fromLatin1("Facebook Exporter"));
    ignores.append(QString::fromLatin1("SmugMug Exporter"));
    ignores.append(QString::fromLatin1("SlideShow"));
    ignores.append(QString::fromLatin1("PrintWizard"));
    ignores.append(QString::fromLatin1("SimpleViewer"));
    ignores.append(QString::fromLatin1("KioExport"));

    d->kipiPluginLoader = new PluginLoader(d->app);
    d->kipiPluginLoader->setInterface(d->kipiInterface);
    d->kipiPluginLoader->setIgnoredPluginsList(ignores);
    d->kipiPluginLoader->init();

    connect(d->kipiPluginLoader, &PluginLoader::replug,
            this, &KipiTestPluginLoader::slotKipiPluginsPlug);

    d->kipiPluginLoader->loadPlugins();
}

void KipiTestPluginLoader::slotKipiPluginsPlug()
{
    d->kipiCategoryMap.clear();
    d->kipipluginsActionCollection->clear();

    PluginLoader::PluginList list = d->kipiPluginLoader->pluginList();
    int cpt                       = 0;

    for (PluginLoader::PluginList::ConstIterator it = list.constBegin() ; it != list.constEnd() ; ++it)
    {
        Plugin* const plugin = (*it)->plugin();

        if (!plugin || !(*it)->shouldLoad())
        {
            continue;
        }

        d->app->guiFactory()->removeClient(plugin);
    }

    QStringList disabledActions = d->kipiPluginLoader->disabledPluginActions();

    for (PluginLoader::PluginList::ConstIterator it = list.constBegin() ; it != list.constEnd() ; ++it)
    {
        Plugin* const plugin = (*it)->plugin();

        if (!plugin || !(*it)->shouldLoad())
        {
            continue;
        }

        ++cpt;
        plugin->setup(d->app);
        plugin->rebuild();

        foreach(QAction* const action, plugin->actions())
        {
            QString actionName(action->objectName());
            Category cat = plugin->category(action);

            if (cat == InvalidCategory)
            {
                qWarning() << "Plugin action '" << actionName << "' has invalid category!";
                continue;
            }

            if (!disabledActions.contains(actionName))
            {
                KActionCategory* category = d->kipiCategoryMap[cat];

                if (!category)
                {
                    category = new KActionCategory(categoryName(cat), d->kipipluginsActionCollection);
                    d->kipiCategoryMap.insert(cat, category);
                }

                category->addAction(actionName, qobject_cast<QAction*>(action));
            }
            else
            {
                qDebug() << "Plugin '" << actionName << "' is disabled.";
            }
        }
    }

    for (PluginLoader::PluginList::ConstIterator it = list.constBegin() ; it != list.constEnd() ; ++it)
    {
        Plugin* const plugin = (*it)->plugin();

        if (!plugin || !(*it)->shouldLoad())
        {
            continue;
        }

        d->app->guiFactory()->addClient(plugin);
    }

    // load KIPI actions settings
    d->kipipluginsActionCollection->readSettings();
}

void KipiTestPluginLoader::checkEmptyCategory(Category cat)
{
    KActionCategory* const category = d->kipiCategoryMap[cat];

    if (!category)
    {
        QString actionName    = QString::fromLatin1("emptyCategory") + categoryShortName(cat);
        QAction* const action = d->app->actionCollection()->addAction(actionName);
        action->setEnabled(false);
        d->kipiCategoryMap.insert(cat, new KActionCategory(categoryName(cat), d->kipipluginsActionCollection));
    }
}

QString KipiTestPluginLoader::categoryName(Category cat) const
{
    QString res;

    switch (cat)
    {
        case ExportPlugin:
            res = QLatin1String("Export Tools");
            break;

        case ImportPlugin:
            res = QLatin1String("Import Tools");
            break;

        case ImagesPlugin:
            res = QLatin1String("Images Tools");
            break;

        case ToolsPlugin:
            res = QLatin1String("Miscellaneous Tools");
            break;

        case BatchPlugin:
            res = QLatin1String("Batch Tools");
            break;

        case CollectionsPlugin:
            res = QLatin1String("Albums Tools");
            break;

        default:
            res = QLatin1String("Unknown Tools");
            break;
    }

    return res;
}

QString KipiTestPluginLoader::categoryShortName(Category cat) const
{
    QString res;

    switch (cat)
    {
        case ExportPlugin:
            res = QLatin1String("Export");
            break;

        case ImportPlugin:
            res = QLatin1String("Import");
            break;

        case ToolsPlugin:
            res = QLatin1String("Tools");
            break;

        case BatchPlugin:
            res = QLatin1String("Batch");
            break;

        case CollectionsPlugin:
            res = QLatin1String("Collenctions");
            break;

        default:
            res = QLatin1String("Unknown");
            break;
    }

    return res;
}

} // namespace KXMLKipiCmd
