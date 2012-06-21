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

// LibKIPI includes

#include <libkipi/pluginloader.h>

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
        app                         = KipiTestMainWindow::instance();
        kipipluginsActionCollection = 0;
        kipiPluginLoader            = 0;
        kipiInterface               = 0;
    }

    static const QString        imagesActionName;
    static const QString        toolsActionName;
    static const QString        importActionName;
    static const QString        exportActionName;
    static const QString        batchActionName;
    static const QString        albumsActionName;

    KIPI::PluginLoader*         kipiPluginLoader;
    KipiInterface*              kipiInterface;
    KipiTestMainWindow*         app;

    KActionCollection*          kipipluginsActionCollection;
    QMap<int, KActionCategory*> kipiCategoryMap;

};

// -- Static values -------------------------------

const QString KipiTestPluginLoader::KipiTestPluginLoaderPriv::imagesActionName(QString::fromLatin1("image_kipi_actions"));
const QString KipiTestPluginLoader::KipiTestPluginLoaderPriv::toolsActionName(QString::fromLatin1("tool_kipi_actions"));
const QString KipiTestPluginLoader::KipiTestPluginLoaderPriv::importActionName(QString::fromLatin1("import_kipi_actions"));
const QString KipiTestPluginLoader::KipiTestPluginLoaderPriv::exportActionName(QString::fromLatin1("export_kipi_actions"));
const QString KipiTestPluginLoader::KipiTestPluginLoaderPriv::batchActionName(QString::fromLatin1("batch_kipi_actions"));
const QString KipiTestPluginLoader::KipiTestPluginLoaderPriv::albumsActionName(QString::fromLatin1("album_kipi_actions"));

KipiTestPluginLoader* KipiTestPluginLoader::m_instance = 0;

// -----------------------------------------------

KipiTestPluginLoader::KipiTestPluginLoader(QObject* const parent)
    : QObject(parent), d(new KipiTestPluginLoaderPriv)
{
    m_instance = this;

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

QList<QAction*> KipiTestPluginLoader::kipiActionsByCategory(KIPI::Category cat) const
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
    d->kipiInterface = new KipiInterface(d->app, "kxmlkipicmd_KIPI_interface");

    // List of obsoletes plugins to not load

    // Comment the following two lines if you want to load the HelloWorld and
    // the KXMLHelloWorld plugins for testing purposes
    ignores.append("HelloWorld");
    //ignores.append("KXMLHelloWorld");

    ignores.append("KameraKlient");

    // These plugins have been renamed with 0.2.0-rc1
    ignores.append("Facebook Exporter");
    ignores.append("SmugMug Exporter");
    ignores.append("SlideShow");
    ignores.append("PrintWizard");
    ignores.append("SimpleViewer");
    ignores.append("KioExport");

    // These plugins have been replaced by digiKam core solution with 2.6.0
    ignores.append("JPEGLossless");

    d->kipiPluginLoader = new KIPI::PluginLoader(ignores, d->kipiInterface);

    connect( d->kipiPluginLoader, SIGNAL(replug()),
             this, SLOT(slotKipiPluginsPlug()) );

    d->kipiPluginLoader->loadPlugins();
}

void KipiTestPluginLoader::slotKipiPluginsPlug()
{
    // Remove plugin GUI menus in application.
    kipiPlugActions(true);

    d->kipiCategoryMap.clear();
    d->kipipluginsActionCollection->clear();

    KIPI::PluginLoader::PluginList list = d->kipiPluginLoader->pluginList();
    int cpt                             = 0;

    // List of obsolete tool actions to not load

    QStringList pluginActionsDisabled;
    pluginActionsDisabled << QString("gpssync2");                       // Experimental plugin renamed gpssync during GoSC2010.
    pluginActionsDisabled << QString("raw_converter_single");           // Obsolete since 0.9.5 and new Raw Import tool.
    pluginActionsDisabled << QString("batch_rename_images");            // Obsolete since 1.0.0, replaced by AdvancedRename.
    pluginActionsDisabled << QString("batch_border_images");            // Obsolete since 1.2.0, replaced by BQM border tool.
    pluginActionsDisabled << QString("batch_convert_images");           // Obsolete since 1.2.0, replaced by BQM convert tool.
    pluginActionsDisabled << QString("batch_color_images");             // Obsolete since 1.2.0, replaced by BQM color tool.
    pluginActionsDisabled << QString("batch_filter_images");            // Obsolete since 1.2.0, replaced by BQM enhance tool.

    for (KIPI::PluginLoader::PluginList::ConstIterator it = list.constBegin() ; it != list.constEnd() ; ++it)
    {
        KIPI::Plugin* plugin = (*it)->plugin();

        if (!plugin || !(*it)->shouldLoad())
        {
            continue;
        }

        ++cpt;
        plugin->setup(d->app);

        foreach(KAction* const action, plugin->actions())
        {
            QString actionName(action->objectName());
            KIPI::Category cat = plugin->category(action);

            if (!pluginActionsDisabled.contains(actionName))
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

    // load KIPI actions settings
    d->kipipluginsActionCollection->readSettings();

    // Check if the Export/Import/tools Plugin lists are empty, if so, add an empty action which tells the user that no
    // Export/Import/tools plugins are available. It is more user-friendly to present some menu entry,
    // instead of leaving it completely empty.
    checkEmptyCategory(KIPI::ExportPlugin);
    checkEmptyCategory(KIPI::ImportPlugin);
    checkEmptyCategory(KIPI::ToolsPlugin);

    // Create plugin GUI menus in application.
    kipiPlugActions();
}

void KipiTestPluginLoader::checkEmptyCategory(KIPI::Category cat)
{
    KActionCategory* category = d->kipiCategoryMap[cat];

    if (!category)
    {
        QAction* action = new QAction(i18n("No tool available"), d->app);
        action->setEnabled(false);
        category        = new KActionCategory(categoryName(cat), d->kipipluginsActionCollection);
        d->kipiCategoryMap.insert(cat, category);
    }
}

void KipiTestPluginLoader::kipiPlugActions(bool unplug)
{
    if (unplug)
    {
        d->app->unplugActionList(d->exportActionName);
        d->app->unplugActionList(d->importActionName);
        d->app->unplugActionList(d->imagesActionName);
        d->app->unplugActionList(d->toolsActionName);
        d->app->unplugActionList(d->batchActionName);
        d->app->unplugActionList(d->albumsActionName);
    }
    else
    {
        d->app->plugActionList(d->exportActionName, kipiActionsByCategory(KIPI::ExportPlugin));
        d->app->plugActionList(d->importActionName, kipiActionsByCategory(KIPI::ImportPlugin));
        d->app->plugActionList(d->imagesActionName, kipiActionsByCategory(KIPI::ImagesPlugin));
        d->app->plugActionList(d->toolsActionName,  kipiActionsByCategory(KIPI::ToolsPlugin));
        d->app->plugActionList(d->batchActionName,  kipiActionsByCategory(KIPI::BatchPlugin));
        d->app->plugActionList(d->albumsActionName, kipiActionsByCategory(KIPI::CollectionsPlugin));
    }
}

QString KipiTestPluginLoader::categoryName(KIPI::Category cat) const
{
    QString res;

    switch (cat)
    {
        case KIPI::ExportPlugin:
            res = i18n("Export Tools");
            break;

        case KIPI::ImportPlugin:
            res = i18n("Import Tools");
            break;

        case KIPI::ImagesPlugin:
            res = i18n("Images Tools");
            break;

        case KIPI::ToolsPlugin:
            res = i18n("Miscellaneous Tools");
            break;

        case KIPI::BatchPlugin:
            res = i18n("Batch Tools");
            break;

        case KIPI::CollectionsPlugin:
            res = i18n("Albums Tools");
            break;

        default:
            res = i18n("Unknown Tools");
            break;
    }

    return res;
}

} // namespace KXMLKipiCmd
