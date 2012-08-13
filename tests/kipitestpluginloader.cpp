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

#include "kipitestpluginloader.moc"

// Qt includes

#include <QMap>
#include <QAction>

// KDE includes

#include <kapplication.h>
#include <kactioncategory.h>
#include <kactioncollection.h>
#include <kaction.h>
#include <kdebug.h>
#include <kxmlguiclient.h>
#include <kxmlguifactory.h>

// Local includes

#include "kipiinterface.h"
#include "kipitestmainwindow.h"

namespace KXMLKipiCmd
{

class KipiTestPluginLoader::KipiTestPluginLoaderPriv
{

public:

    KipiTestPluginLoaderPriv()
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
    : QObject(parent), d(new KipiTestPluginLoaderPriv)
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
    KActionCategory* category = d->kipiCategoryMap[cat];

    if (category)
    {
        return category->actions();
    }

    return QList<QAction*>();
}

void KipiTestPluginLoader::loadPlugins()
{
    d->kipipluginsActionCollection = new KActionCollection(d->app, KGlobal::mainComponent());

    QStringList ignores;

    // List of obsoletes plugins to not load

    // Comment the following two lines if you want to load the HelloWorld and
    // the KXMLHelloWorld plugins for testing purposes
    //ignores.append("HelloWorld");
    //ignores.append("KXMLHelloWorld");

    ignores.append("KameraKlient");

    // These plugins have been renamed with 0.2.0-rc1
    ignores.append("Facebook Exporter");
    ignores.append("SmugMug Exporter");
    ignores.append("SlideShow");
    ignores.append("PrintWizard");
    ignores.append("SimpleViewer");
    ignores.append("KioExport");

    d->kipiPluginLoader = new PluginLoader(d->app);
    d->kipiPluginLoader->setInterface(d->kipiInterface);
    d->kipiPluginLoader->setIgnoredPluginsList(ignores);
    d->kipiPluginLoader->init();

    connect(d->kipiPluginLoader, SIGNAL(replug()),
            this, SLOT(slotKipiPluginsPlug()));

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
        Plugin* plugin = (*it)->plugin();

        if (!plugin || !(*it)->shouldLoad())
        {
            continue;
        }

        d->app->guiFactory()->removeClient(plugin);
    }

    QStringList disabledActions = d->kipiPluginLoader->disabledPluginActions();

    for (PluginLoader::PluginList::ConstIterator it = list.constBegin() ; it != list.constEnd() ; ++it)
    {
        Plugin* plugin = (*it)->plugin();

        if (!plugin || !(*it)->shouldLoad())
        {
            continue;
        }

        ++cpt;
        plugin->setup(d->app);
        plugin->rebuild();

        foreach(KAction* const action, plugin->actions())
        {
            QString actionName(action->objectName());
            Category cat = plugin->category(action);

            if (cat == InvalidCategory)
            {
                kWarning() << "Plugin action '" << actionName << "' has invalid category!";
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
                kDebug() << "Plugin '" << actionName << "' is disabled.";
            }
        }
    }

    for (PluginLoader::PluginList::ConstIterator it = list.constBegin() ; it != list.constEnd() ; ++it)
    {
        Plugin* plugin = (*it)->plugin();

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
    KActionCategory* category = d->kipiCategoryMap[cat];

    if (!category)
    {
//        QAction* action = new QAction(i18n("No tool available"), d->app);
        QString actionName = "emptyCategory" + categoryShortName(cat);
        KAction* action = d->app->actionCollection()->addAction(actionName);
        action->setEnabled(false);
        category        = new KActionCategory(categoryName(cat), d->kipipluginsActionCollection);
        d->kipiCategoryMap.insert(cat, category);
    }
}

QString KipiTestPluginLoader::categoryName(Category cat) const
{
    QString res;

    switch (cat)
    {
        case ExportPlugin:
            res = i18n("Export Tools");
            break;

        case ImportPlugin:
            res = i18n("Import Tools");
            break;

        case ImagesPlugin:
            res = i18n("Images Tools");
            break;

        case ToolsPlugin:
            res = i18n("Miscellaneous Tools");
            break;

        case BatchPlugin:
            res = i18n("Batch Tools");
            break;

        case CollectionsPlugin:
            res = i18n("Albums Tools");
            break;

        default:
            res = i18n("Unknown Tools");
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
        res = i18n("Export");
        break;

    case ImportPlugin:
        res = i18n("Import");
        break;

    case ToolsPlugin:
        res = i18n("Tools");
        break;

    case BatchPlugin:
        res = i18n("Batch");
        break;

    case CollectionsPlugin:
        res = i18n("Collenctions");
        break;

    default:
        res = i18n("Unknown");
        break;
    }

    return res;
}

} // namespace KXMLKipiCmd
