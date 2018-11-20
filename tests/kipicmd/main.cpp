/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2009-11-21
 * @brief  kipi CLI host test application to run kipi tool as stand alone.
 *
 * @author Copyright (C) 2009-2010 by Michael G. Hansen
 *         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
 * @author Copyright (C) 2011-2018 by Gilles Caulier
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
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

// Libkipi includes

#include "libkipi_version.h"
#include "plugin.h"
#include "pluginloader.h"
#include "kipiinterface.h"

#ifdef HAVE_KEXIV2
#   include <kexiv2/kexiv2.h>
#endif

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
            categorystring = QLatin1String("Images");
            break;

        case ToolsPlugin:
            categorystring = QLatin1String("Tool");
            break;

        case ImportPlugin:
            categorystring = QLatin1String("Import");
            break;

        case ExportPlugin:
            categorystring = QLatin1String("Export");
            break;

        case BatchPlugin:
            categorystring = QLatin1String("Batch");
            break;

        case CollectionsPlugin:
            categorystring = QLatin1String("Collections");
            break;

        default:
            categorystring = QLatin1String("Unknown");
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
bool LoadPlugins(const QString& libraryName = QString::fromLatin1(""))
{
    if (!libraryName.isEmpty())
    {
        qDebug() << QString::fromLatin1("Will only load library \"%1\"").arg(libraryName);
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
                qDebug() << QString::fromLatin1("Can not load plugin \"%1\": Loader says it should not load.").arg(libraryName);
                return false;
            }

            if ( !(*it)->plugin() )
            {
                qDebug() << QString::fromLatin1("Plugin \"%1\" failed to load.").arg(libraryName);
                return false;
            }

            qDebug() << QString::fromLatin1("Plugin \"%1\" loaded.").arg(libraryName);
            return true;
        }
    }

    qDebug() << QString::fromLatin1("Plugin \"%1\" not found.").arg(libraryName);

    return false;
}

/**
* \brief Lists the available plugins and their actions
* \param libraryName Load only plugin found in library libraryName
* \returns False if plugin in library \a libraryName could not be loaded
*/
bool ListPlugins(const QString& libraryName = QString::fromLatin1(""))
{
    PluginLoader* const kipiPluginLoader = PluginLoader::instance();

    if ( !LoadPlugins( libraryName ) )
        return false;

    const PluginLoader::PluginList pluginList = kipiPluginLoader->pluginList();
    int pluginNumber                          = 1;
    const int nPlugins                        = pluginList.size();
    const int nDigits                         = QString::number(nPlugins).size();
    const QString preSpace                    = QString(nDigits+1+1, QChar::fromLatin1(' '));
    QWidget* const dummyWidget                = new QWidget();

    qDebug() << QString::fromLatin1("Found %1 plugin(s):").arg(nPlugins);

    for (PluginLoader::PluginList::ConstIterator it = pluginList.constBegin();
         it!= pluginList.constEnd(); ++ it)
    {
        const QString pluginNumberString = QString::fromLatin1("%1").arg(pluginNumber, nDigits);
        ++pluginNumber;

        qDebug() << QString::fromLatin1("%1: %2 - %3").arg(pluginNumberString).arg((*it)->name()).arg((*it)->comment());
        qDebug() << preSpace << QString::fromLatin1("Library: ") << (*it)->library();

        Plugin* const plugin = (*it)->plugin();

        if (plugin == 0)
        {
            qDebug() << preSpace << QString::fromLatin1( "Plugin not loaded.");
            continue;
        }

        plugin->setup(dummyWidget);
        const QList<QPair<int, QAction*> > actionsList = FlattenActionList(plugin->actions());

        qDebug() << preSpace << QString::fromLatin1("Actions:");

        const QString preSpaceActions = preSpace + QString::fromLatin1("  ");

        for (QList<QPair<int, QAction*> >::ConstIterator it = actionsList.constBegin();
             it!=actionsList.constEnd(); ++it)
        {
            const int level             = (*it).first;
            const QAction* const action = (*it).second;
            qDebug() << preSpaceActions << QString(level*2, QChar::fromLatin1(' ')) << '"' << action->text() << '"';
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
bool CallAction(const QString& actionText, const QString& libraryName = QString::fromLatin1(""))
{
    qDebug() << QString::fromLatin1("Looking for action \"%1\"...").arg(actionText);

    PluginLoader* const kipiPluginLoader = PluginLoader::instance();

    if ( !LoadPlugins( libraryName ) )
        return false;

    PluginLoader::PluginList pluginList = kipiPluginLoader->pluginList();

    QWidget* const dummyWidget = new QWidget();

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
            qDebug() << QString::fromLatin1("Plugin \"%1\" failed to load.").arg((*info)->library());
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

            qDebug() << QString::fromLatin1("Found action \"%1\" in library \"%2\", will now call it.").arg(actionText).arg((*info)->library());

            // call the action:
            pluginAction->trigger();
            qDebug() << QString::fromLatin1("Plugin is done.");
            foundAction = true;

            break;
        }
    }

    if (!foundAction)
        qDebug() << QString::fromLatin1("Could not find action \"%1\".").arg(actionText);

    return foundAction;
}

int main(int argc, char* argv[])
{
#ifdef HAVE_KEXIV2
    KExiv2Iface::KExiv2::initializeExiv2();
#endif

    QApplication app(argc, argv);
    app.setApplicationName(QLatin1String("kipicmd"));
    app.setApplicationVersion(QLatin1String(KIPI_VERSION_STRING));
    app.setOrganizationName(QLatin1String("http://www.kde.org"));
    app.setWindowIcon(QIcon(QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                   QLatin1String(":/images/kipi-icon.svg"))));

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.setApplicationDescription(QLatin1String("kipi CLI host test application to run kipi tool as stand alone"));

    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("list"),           QLatin1String("List the available plugins")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("w"),              QLatin1String("Wait until non-modal dialogs are closed")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("l"),              QLatin1String("Library name of plugin to use"),             QLatin1String("library")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("a"),              QLatin1String("Action to call"),                            QLatin1String("action")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("i"),              QLatin1String("Selected images"),                           QLatin1String("selectedimages")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("c"),              QLatin1String("Selected collections"),                       QLatin1String("selectedcollections")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("allc"),           QLatin1String("All collections"),                           QLatin1String("allcollections")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("+[images]"),      QLatin1String("List of images")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("+[collections]"), QLatin1String("List of collections")));
    parser.process(app);

    KipiInterface* const kipiInterface = new KipiInterface(&app);

    PluginLoader* const loader = new PluginLoader(0);
    loader->setInterface(kipiInterface);
    loader->init();

    QList<QUrl> listSelectedImages;
    QList<QUrl> listSelectedAlbums;
    QList<QUrl> listAllAlbums;

    // Determine which with list we start

    QList<QUrl>* startList = 0;

    if (parser.isSet(QString::fromLatin1("i")))
    {
        startList = &listSelectedImages;
        startList->append(QUrl::fromLocalFile(parser.value(QString::fromLatin1("i"))));
    }
    else if (parser.isSet(QString::fromLatin1("c")))
    {
        startList = &listSelectedAlbums;
        startList->append(QUrl::fromLocalFile(parser.value(QString::fromLatin1("c"))));
    }
    else if (parser.isSet(QString::fromLatin1("allc")))
    {
        startList = &listAllAlbums;
        startList->append(QUrl::fromLocalFile(parser.value(QString::fromLatin1("allc"))));
    }

    if (startList)
    {
        qDebug() << "startList: " << *startList;
    }
    else
    {
        qDebug() << "startList: is null";
    }

    qDebug() << "parser: "    << parser.optionNames();

    // Append the remaining arguments to the lists

    const QStringList args = parser.positionalArguments();

    for (int i = 0; i < args.count(); ++i)
    {
        const QString argValue = args.value(i);

        if (argValue == QString::fromLatin1("-i"))
        {
            startList = &listSelectedImages;
        }
        else if (argValue == QString::fromLatin1("-c"))
        {
            startList = &listSelectedAlbums;
        }
        else if (argValue == QString::fromLatin1("-allc"))
        {
            startList = &listAllAlbums;
        }
        else
        {
            if (startList == 0)
            {
                qCritical() << "StartList is null.\n"
                               "Please specify how the filenames you provided should be used.";
                return 0;
            }
            else
            {
                startList->append(QUrl::fromLocalFile(args.value(i)));
            }
        }
    }

    qDebug() << "listSelectedImages:" << listSelectedImages;
    qDebug() << "listSelectedAlbums:" << listSelectedAlbums;
    qDebug() << "listAllAlbums:"      << listAllAlbums;

    kipiInterface->addSelectedImages(listSelectedImages);
    kipiInterface->addSelectedAlbums(listSelectedAlbums);
    kipiInterface->addAlbums(listAllAlbums);

    // determine whether only one plugin should be loaded

    const QString nameOfOnlyOnePluginToLoad = parser.value(QString::fromLatin1("l"));

    // determine what to do

    int returnValue    = 0;
    bool startedPlugin = false;

    if ( parser.isSet(QString::fromLatin1("list")) )
    {
        if (!ListPlugins( nameOfOnlyOnePluginToLoad ))
        {
            returnValue = 1;
        }
    }
    else if ( parser.isSet(QString::fromLatin1("a")) )
    {
        const QString action = parser.value(QString::fromLatin1("a"));

        qDebug() << action;

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
        qCritical() << "No argument specified: either use --list,\n"
                       "or specify an action to be called.\n"
                       "Example : ./kipicmd -w -lkipiplugin_kxmlhelloworld -a\"KXML Hello World Image...\" -i ~/Images/*";
        return 0;
    }

    int ret = returnValue;

    if (startedPlugin && parser.isSet(QString::fromLatin1("w")))
    {
        ret = app.exec();
    }

#ifdef HAVE_KEXIV2
    KExiv2Iface::KExiv2::cleanupExiv2();
#endif

    return ret;
}
