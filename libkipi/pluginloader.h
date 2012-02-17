/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin loader
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2009 by Andi Clemens
 *         <a href="mailto:andi dot clemens at googlemail dot com">andi dot clemens at googlemail dot com</a>
 * @author Copyright (C) 2009 by Aleix Pol Gonzalez
 *         <a href="mailto:aleixpol at kde dot org">aleixpol at kde dot org</a>
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

#ifndef KIPI_PLUGINLOADER_H
#define KIPI_PLUGINLOADER_H

// Qt includes

#include <QtCore/QList>
#include <QtGui/QListWidget>

// KDE includes

#include <kservice.h>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

class Plugin;
class Interface;
class ConfigWidget;

/**
    \author Gilles Caulier
    \class KIPI::PluginLoader
    This is the class that will help host applications to load plugins.

    The host application must create an instance of the plugin loader, and
    call the method loadPlugins() to get the plugins loaded. To ensure that
    plugins are correctly removed from menus and toolbars when loaded and
    unloaded after constructions, the application must connect to either the
    signals plug() / unplug() or the signal replug(). These signals are
    emitted when a plugin is to be inserted into the menus.

    If your application is using XMLGUI, the easiest way to get the plugins
    inserted into the menus is by adding an item in the ui.rc file looking
    list this:

        &lt;ActionList name="image_actions"/&gt;

    Then plugin plugins into menus could be done with code similar to this from digiKam:

    \code
    void DigikamApp::slotKipiPluginPlug()
    {
        unplugActionList(QString::fromLatin1("file_actions_export"));
        unplugActionList(QString::fromLatin1("image_actions"));
        unplugActionList(QString::fromLatin1("tool_actions"));

        d->kipiImageActions.clear();
        d->kipiFileActionsExport.clear();
        d->kipiToolsActions.clear();

        d->kipipluginsActionCollection->clear();

        KIPI::PluginLoader::PluginList list = d->kipiPluginLoader->pluginList();
        int cpt                             = 0;

        for ( KIPI::PluginLoader::PluginList::ConstIterator it = list.constBegin() ;
            it != list.constEnd() ; ++it )
        {
            KIPI::Plugin* plugin = (*it)->plugin();

            if ( !plugin || !(*it)->shouldLoad() )
                continue;

            ++cpt;

            plugin->setup( this );

            // Add actions to kipipluginsActionCollection
            QList<QAction*> allPluginActions = plugin->actionCollection()->actions();

            foreach(QAction* action, allPluginActions)
            {
                QString actionName(action->objectName());

                if (!pluginActionsDisabled.contains(actionName))
                    d->kipipluginsActionCollection->addAction(actionName, action);
            }

            // Plugin category identification using KAction method based.

            QList<KAction*> actions = plugin->actions();
            foreach(KAction* action, actions)
            {
                QString actionName(action->objectName());

                switch (plugin->category(action))
                {
                    case KIPI::ExportPlugin:
                    {
                        d->kipiFileActionsExport.append(action);
                        break;
                    }
                    case KIPI::ImagesPlugin:
                    {
                        d->kipiImageActions.append(action);
                        break;
                    }
                    case KIPI::ToolsPlugin:
                    {
                        d->kipiToolsActions.append(action);
                        break;
                    }
                    default:
                    {
                        kDebug() << "No menu found for a plugin!";
                        break;
                    }
                }
            }
        }

        // load KIPI actions settings
        d->kipipluginsActionCollection->readSettings();

        // Create GUI menu in according with plugins.
        plugActionList(QString::fromLatin1("file_actions_export"),        d->kipiFileActionsExport);
        plugActionList(QString::fromLatin1("image_actions"),              d->kipiImageActions);
        plugActionList(QString::fromLatin1("tool_actions"),               d->kipiToolsActions);
    }
    \endcode

    For a complete implementation used to manage Kipi-plugins in digiKam, look <a href="https://projects.kde.org/projects/extragear/graphics/digikam/repository/revisions/master/entry/digikam/main/digikamapp.cpp">
    into this class</a>.

    To configure which plugins should be loaded, simply call
    PluginLoader::configWidget(), and insert the widget into your normal
    configuration dialog.
*/
class LIBKIPI_EXPORT PluginLoader : public QObject
{
    Q_OBJECT

public:

    class LIBKIPI_EXPORT Info
    {

    public:

        Info(const KService::Ptr& service, bool shouldLoad);
        ~Info();

        QString       name() const;
        QString       comment() const;
        QIcon         icon() const;
        QString       library() const;
        KService::Ptr service() const;
        Plugin*       plugin() const;

        /** @p deprecated. You do not want to change the plugin instance. */
        KDE_DEPRECATED void setPlugin(Plugin*);

        void reload();
        bool shouldLoad() const;
        void setShouldLoad(bool);

    private:

        class InfoPrivate;
        InfoPrivate* const d;
    };

public:

    typedef QList<Info*> PluginList;

public:

    PluginLoader(const QStringList& ignores, KIPI::Interface* const interface);
    PluginLoader(const QStringList& ignores, KIPI::Interface* const interface, const QString& constraint);
    virtual ~PluginLoader();

    ConfigWidget* configWidget(QWidget* parent) const;

    const PluginList& pluginList();

    /** @p deprecated now plugins will be loaded when requested to the Info item */
    void loadPlugins();

    /** @p deprecated now plugins will be loaded when requested to the Info item */
    KDE_DEPRECATED void loadPlugin(Info* const);

    void construct(const QStringList& ignores, KIPI::Interface* const interface, const QString& constraint);
    KIPI::Interface* interface() const;

    static PluginLoader* instance();

Q_SIGNALS:

    void plug( KIPI::PluginLoader::Info* );
    void unplug( KIPI::PluginLoader::Info* );

    /** @deprecated */
    void replug();

private:

    class PluginLoaderPrivate;
    PluginLoaderPrivate* const d;

private:

    friend class ConfigWidget;
    friend class PluginCheckBox;
};

// ----------------------------------------------------------------------

class LIBKIPI_EXPORT ConfigWidget : public QListWidget
{
    Q_OBJECT

public:

    ConfigWidget( QWidget* parent );
    ~ConfigWidget();

public Q_SLOTS:

    void apply();

    void slotCheckAll();
    void slotClear();

private:

    class ConfigWidgetPrivate;
    ConfigWidgetPrivate* const d;
};

} // namespace KIPI

#endif /* PLUGINLOADER_H */
