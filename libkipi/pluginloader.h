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
    \class PluginLoader
    This is the class that will help host applications to load plugins.

    The host application must create an instance of the plugin loader, and
    call the method loadPlugins() to get the plugins loaded. To ensure that
    plugins are correctly removed from menus and toolbars when loaded and
    unloaded after constructions, the application must connect to either the
    signals plug() / unplug() or the signal replug(). These signals are
    emitted when a plugin is to be inserted into the menus.

    If your application is using XMLGUI, the easiest way to get the plugins
    inserted into the menus is by adding an item in the ui.rc file looking
    like this:

        &lt;ActionList name="export_kipi_actions"/&gt;
        &lt;ActionList name="image_kipi_actions"/&gt;
        &lt;ActionList name="tool_kipi_actions"/&gt;

    Then loading plugins into menus could be done with code similar to this slot implementation:

    \code

    KIPI::Interface*    m_iface  = 0;
    KIPI::PluginLoader* m_loader = 0;
    QList<QAction*>     m_kipiImageActions;
    QList<QAction*>     m_kipiExportActions;
    QList<QAction*>     m_kipiToolsActions;

    void MyApplication::loadPlugins()
    {
        m_iface  = new KIPI::Interface(this, "MyApplication_KIPI_interface");
        m_loader = new KIPI::PluginLoader(QStringList(), m_iface);

        connect(m_loader, SIGNAL(replug()),
                this, SLOT(slotKipiPluginPlug()));

        m_loader->loadPlugins();
    }
    
    void MyApplication::slotKipiPluginPlug()
    {
        unplugActionList("export_kipi_actions");
        unplugActionList("image_kipi_actions");
        unplugActionList("tool_kipi_actions");

        m_kipiImageActions.clear();
        m_kipiExportActions.clear();
        m_kipiToolsActions.clear();

        PluginLoader::PluginList list = m_loader->pluginList();

        for (PluginLoader::PluginList::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it )
        {
            Plugin* plugin = (*it)->plugin();
            if ( !plugin || !(*it)->shouldLoad() )
                continue;

            plugin->setup(this);

            // Plugin wrap based on category identification.

            foreach(KAction* const action, plugin->actions())
            {
                switch (plugin->category(action))
                {
                    case ExportPlugin:
                    {
                        m_kipiExportActions.append(action);
                        break;
                    }
                    case ImagesPlugin:
                    {
                        m_kipiImageActions.append(action);
                        break;
                    }
                    case ToolsPlugin:
                    {
                        m_kipiToolsActions.append(action);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        // Create GUI menu in according with plugins.
        plugActionList("export_kipi_actions", m_kipiExportActions);
        plugActionList("image_kipi_actions",  m_kipiImageActions);
        plugActionList("tool_kipi_actions",   m_kipiToolsActions);
    }

    \endcode

    For a complete implementation used to manage Kipi-plugins in digiKam, look <a href="https://projects.kde.org/projects/extragear/graphics/digikam/repository/revisions/master/entry/utilities/kipiiface/kipipluginloader.cpp">
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

    PluginLoader(const QStringList& ignores, Interface* const interface);
    PluginLoader(const QStringList& ignores, Interface* const interface, const QString& constraint);
    virtual ~PluginLoader();

    void construct(const QStringList& ignores, Interface* const interface, const QString& constraint);

    const PluginList& pluginList();

    // NOTE: plugin can be loaded through Info item.
    void loadPlugins();

    ConfigWidget*        configWidget(QWidget* const parent) const;
    Interface*           interface() const;
    static PluginLoader* instance();

Q_SIGNALS:

    void plug(KIPI::PluginLoader::Info*);
    void unplug(KIPI::PluginLoader::Info*);

    // NOTE: plugin can be pluged through Info item.
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

    ConfigWidget(QWidget* const parent);
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
