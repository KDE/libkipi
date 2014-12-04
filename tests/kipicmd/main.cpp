/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2009-11-21
 * @brief  kipi host test application
 *
 * @author Copyright (C) 2009-2010 by Michael G. Hansen
 *         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
 * @author Copyright (C) 2011-2014 by Gilles Caulier
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

// C++ include

#include <memory>

// Qt includes

#include <QTextStream>
#include <QMenu>
#include <QDebug>
#include <QUrl>
#include <QStandardPaths>

// KDE includes

#include <kapplication.h>
#include <k4aboutdata.h>
#include <kcmdlineargs.h>
#include <klocalizedstring.h>

// Libkipi includes

#include "libkipi_version.h"
#include "plugin.h"
#include "pluginloader.h"
#include "kipiinterface.h"

// local includes

#include "kipiinterface.h"

using namespace KXMLKipiCmd;
using namespace KIPI;

/**
* \brief Returns the name of a KIPI::Category
* \param category Category which should be returned as a string
* \returns String version of the category
*/
QString PluginCategoriesToString(const Category& category)
{
    QString categorystring;

    switch(category)
    {
        case ImagesPlugin:
            categorystring = i18n("Images");
            break;

        case ToolsPlugin:
            categorystring = i18n("Tool");
            break;

        case ImportPlugin:
            categorystring = i18n("Import");
            break;

        case ExportPlugin:
            categorystring = i18n("Export");
            break;

        case BatchPlugin:
            categorystring = i18n("Batch");
            break;

        case CollectionsPlugin:
            categorystring = i18n("Collections");
            break;

        default:
            categorystring = i18n("Unknown");
            break;
    };

    return categorystring;
}

/**
* \brief Takes a list of QActions with submenus and converts them to a flat list
* \param actions List of QActions with submenus
* \param level Initial level of indentation (used internally for recursion)
* \returns A list of QPairs containing the indentation level and the QAction*
*/
QList<QPair<int, QAction*> > FlattenActionList(const QList<QAction*>& actions, const int level = 0)
{
    QList<QPair<int, QAction*> > results;

    for (QList<QAction*>::ConstIterator it = actions.constBegin(); it!=actions.constEnd(); ++it)
    {
        const QString text = (*it)->text();

        if (!text.isEmpty())
        {
            results.append( QPair<int, QAction*>(level, *it) );
        }

        QMenu* const menu = (*it)->menu();

        if (menu)
        {
            results.append( FlattenActionList(menu->actions(), level+1) );
        }
    }
    return results;
}

/**
* \brief Loads the kipi-plugins
* \param libraryName If not empty, only the plugin in library libraryName is loaded and setup
* \returns False if plugin library libraryName could not be loaded
*/
bool LoadPlugins(const QString& libraryName = "")
{
    if (!libraryName.isEmpty())
    {
        qDebug() << QString("Will only load library \"%1\"").arg(libraryName);
    }
    else
    {
        qDebug() << "Will load all libraries, if possible.";
    }

    PluginLoader* const kipiPluginLoader = PluginLoader::instance();

    if (libraryName.isEmpty())
    {
        kipiPluginLoader->loadPlugins();
        return true;
    }

    const PluginLoader::PluginList pluginList = kipiPluginLoader->pluginList();

    for (PluginLoader::PluginList::ConstIterator it = pluginList.constBegin();
         it!=pluginList.constEnd(); ++it)
    {
        if ( (*it)->library() == libraryName )
        {
            if ( !(*it)->shouldLoad() )
            {
                qDebug() << i18n("Can not load plugin \"%1\": Loader says it should not load.", libraryName);
                return false;
            }

            (*it)->shouldLoad();

            if ( !(*it)->plugin() )
            {
                qDebug() << i18n("Plugin \"%1\" failed to load.", libraryName);
                return false;
            }

            qDebug() << i18n("Plugin \"%1\" loaded.", libraryName);
            return true;
        }
    }

    qDebug() << i18n("Plugin \"%1\" not found.", libraryName);

    return false;
}

/**
* \brief Lists the available plugins and their actions
* \param libraryName Load only plugin found in library libraryName
* \returns False if plugin in library \a libraryName could not be loaded
*/
bool ListPlugins(const QString& libraryName = "")
{
    PluginLoader* const kipiPluginLoader = PluginLoader::instance();

    if ( !LoadPlugins( libraryName ) )
        return false;

    const PluginLoader::PluginList pluginList = kipiPluginLoader->pluginList();
    int pluginNumber                          = 1;
    const int nPlugins                        = pluginList.size();
    const int nDigits                         = QString::number(nPlugins).size();
    const QString preSpace                    = QString(nDigits+1+1, ' ');

    std::auto_ptr<QWidget> dummyWidget( new QWidget() );

    qDebug() << i18np("Found 1 plugin:", "Found %1 plugins:", nPlugins);

    for (PluginLoader::PluginList::ConstIterator it = pluginList.constBegin();
         it!= pluginList.constEnd(); ++ it)
    {
        const QString pluginNumberString = QString("%1").arg(pluginNumber, nDigits); ++pluginNumber;

        qDebug() << QString("%1: %2 - %3").arg(pluginNumberString).arg((*it)->name()).arg((*it)->comment());
        qDebug() << preSpace << i18n("Library: ")<< (*it)->library();

        Plugin* const plugin = (*it)->plugin();

        if (plugin == 0)
        {
            qDebug() << preSpace << i18n( "Plugin not loaded." );
            continue;
        }

        plugin->setup(dummyWidget.get());
        const QList<QPair<int, QAction*> > actionsList = FlattenActionList(plugin->actions());
        qDebug() << preSpace<<i18n("Actions:");
        const QString preSpaceActions = preSpace + "  ";

        for (QList<QPair<int, QAction*> >::ConstIterator it = actionsList.constBegin();
             it!=actionsList.constEnd(); ++it)
        {
            const int level             = (*it).first;
            const QAction* const action = (*it).second;
            qDebug() << preSpaceActions << QString(level*2, ' ') << '"' << action->text() << '"';
        }
    }

    return true;
}

/**
* \brief Calls an action of a plugin
* \param actionText Text of the action to call
* \param libraryName Load only the plugin in this library
* \returns False if the action could not be called
*/
bool CallAction(const QString& actionText, const QString& libraryName = "")
{
    qDebug() << QString("Looking for action \"%1\"...").arg(actionText);

    PluginLoader* const kipiPluginLoader = PluginLoader::instance();

    if ( !LoadPlugins( libraryName ) )
        return false;

    PluginLoader::PluginList pluginList = kipiPluginLoader->pluginList();

    /*std::auto_ptr<*/QWidget* dummyWidget( new QWidget() );

    bool foundAction = false;

    for (PluginLoader::PluginList::ConstIterator info = pluginList.constBegin();
         (info!=pluginList.constEnd()) && !foundAction; ++info)
    {
        if ( !libraryName.isEmpty() && ( (*info)->library() != libraryName ) )
            continue;

        // scan for the desired action:
        Plugin* const plugin = (*info)->plugin();

        if (plugin == 0)
        {
            qDebug() << i18n("Plugin \"%1\" failed to load.", (*info)->library());
            continue;
        }

        plugin->setup(dummyWidget/*.get()*/);
        const QList<QPair<int, QAction*> > actionsList = FlattenActionList(plugin->actions());

        for (QList<QPair<int, QAction*> >::ConstIterator it = actionsList.constBegin();
             it!=actionsList.constEnd(); ++it)
        {
            QAction* const pluginAction = (*it).second;
            qDebug()<<pluginAction->text();

            if ( pluginAction->text() != actionText )
                continue;

            qDebug() << i18n("Found action \"%1\" in library \"%2\", will now call it.", actionText, (*info)->library());

            // call the action:
            pluginAction->trigger();
            qDebug() << i18n("Plugin is done.");
            foundAction = true;

            break;
        }
    }

    if (!foundAction)
        qDebug() << i18n("Could not find action \"%1\".", actionText);

    return foundAction;
}

int main(int argc, char* argv[])
{
    const K4AboutData aboutData("kipiplugins",
                               "kipi",
                               ki18n("kipicmd"),
                               KIPI_VERSION_STRING,            // version
                               ki18n("Kipi host test application"),
                               K4AboutData::License_GPL,
                               ki18n("(c) 2009-2010 Michael G. Hansen\n"
                                     "(c) 2011-2014 Gilles Caulier "),
                               KLocalizedString(),             // optional text
                               "http://www.digikam.org",       // URI of homepage
                               "kde-imaging@kde.org"           // bugs e-mail address
                              );

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineOptions options;
    options.add( "listplugins", ki18n("List the available plugins") );
    options.add( "l" ).add("library <libraryname>", ki18n("Library name of plugin to use") );
    options.add( "a" ).add( "action <action>", ki18n("Action to call") );
    options.add( "w" ).add( "wait", ki18n("Wait until non-modal dialogs are closed.") );
    options.add( "!selectedalbums <album>", ki18n("Selected albums") );
    options.add( "!selectedimages <images>", ki18n("Selected images") );
    options.add( "!allalbums <albums>", ki18n("All albums") );
    options.add( "+[images]", ki18n("List of images") );
    options.add( "+[albums]", ki18n("List of albums") );
    options.add( "", ki18n("Exemple : ./kipicmd -w -lkipiplugin_rawconverter -a\"Batch RAW Converter...\"") );
    KCmdLineArgs::addCmdLineOptions( options );

    KApplication app;
    app.setWindowIcon(QIcon(QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kipi/data/kipi-icon.svg")));

    KipiInterface* const kipiInterface = new KipiInterface(&app);

    // create an instance of the plugin loader:
    PluginLoader* const loader = new PluginLoader(0);
    loader->setInterface(kipiInterface);
    loader->init();

    KCmdLineArgs* const args = KCmdLineArgs::parsedArgs();

    // handling of selectedimages, selectalbums and otheralbums is complicated
    // create new lists of these parameters:
    QList<QUrl> listSelectedImages;
    QList<QUrl> listSelectedAlbums;
    QList<QUrl> listAllAlbums;

    // determine which with list we start:
    QList<QUrl>* startList = 0;

    if (args->isSet("selectedimages"))
    {
        startList = &listSelectedImages;
        startList->append(KCmdLineArgs::makeURL(args->getOption("selectedimages").toUtf8()));
    }
    else if (args->isSet("selectedalbums"))
    {
        startList = &listSelectedAlbums;
        startList->append(KCmdLineArgs::makeURL(args->getOption("selectedalbums").toUtf8()));
    }
    else if (args->isSet("allalbums"))
    {
        startList = &listAllAlbums;
        startList->append(KCmdLineArgs::makeURL(args->getOption("allalbums").toUtf8()));
    }

    // append the remaining arguments to the lists:
    for (int i = 0; i < args->count(); ++i)
    {
        const QString argValue = args->arg(i);

        if (argValue == "--selectedimages")
        {
            startList = &listSelectedImages;
        }
        else if (argValue == "--selectedalbums")
        {
            startList = &listSelectedAlbums;
        }
        else if (argValue == "--allalbums")
        {
            startList = &listAllAlbums;
        }
        else
        {
            if (startList == 0)
            {
                qCritical() << "startList==0";
                args->usageError(i18n("Please specify how the filenames you provided should be used."));
            }
            else
            {
                startList->append(args->url(i));
            }
        }
    }

//     qDebug() << "listSelectedImages:" << listSelectedImages;
//     qDebug() << "listSelectedAlbums:" << listSelectedAlbums;
//     qDebug() << "listAllAlbums:"      << listAllAlbums;

    kipiInterface->addSelectedImages(listSelectedImages);
    kipiInterface->addSelectedAlbums(listSelectedAlbums);
    kipiInterface->addAlbums(listAllAlbums);

    // determine whether only one plugin should be loaded:
    const QString nameOfOnlyOnePluginToLoad = args->getOption("library");

    // determine what to do:
    int returnValue                         = 0;
    bool startedPlugin                      = false;

    if ( args->isSet("listplugins") )
    {
        if (!ListPlugins( nameOfOnlyOnePluginToLoad ))
            returnValue = 1;
    }
    else if ( args->isSet("a") )
    {
        const QString action = args->getOption("action");

        if ( !CallAction( action, nameOfOnlyOnePluginToLoad ) )
        {
            returnValue = 1;
        }
        else
        {
            startedPlugin = true;
        }
    }
    else
    {
        KCmdLineArgs::usageError( i18n("No argument specified: either use --listplugins, "
                                       "or specify an action to be called.") );
    }

    if (startedPlugin&&args->isSet("wait"))
    {
        return app.exec();
    }
    else
    {
        return returnValue;
    }
}
