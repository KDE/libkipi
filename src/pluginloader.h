/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2009 Andi Clemens <andi dot clemens at googlemail dot com>
    SPDX-FileCopyrightText: 2009 Aleix Pol Gonzalez <aleixpol at kde dot org>
    SPDX-FileCopyrightText: 2012 Victor Dodon <dodonvictor at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPI_PLUGINLOADER_H
#define KIPI_PLUGINLOADER_H

// Std includes

#include <memory>

// Qt includes

#include <QList>

// KF includes

#include <KService>
#include <KXmlGuiWindow>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

class Plugin;
class Interface;
class ConfigWidget;

/**
    \author Gilles Caulier
    \par Maintainer: Victor Dodon
    \class PluginLoader pluginloader.h <KIPI/PluginLoader>
    This is the class that will help host applications to load plugins.
    
    This is a singleton and host applications should create the PluginLoader
    just once, and then use the instance() static method to access it.

    The host application must create an instance of the plugin loader, and
    call the method loadPlugins() to get the plugins loaded. To ensure that
    plugins are correctly removed from menus and toolbars when loaded and
    unloaded after constructions, the application must connect to either the
    signals plug() / unplug() or the signal replug(). These signals are
    emitted when a plugin is to be inserted into the menus.

    If your application is using KDE XMLGUI, the easiest(nicest) way to get the
    plugins inserted into the menus is by adding an item in your application XML
    ui.rc file looking like this:

    \code

    <!DOCTYPE kpartgui SYSTEM "kpartgui.dtd">
    <gui version="1" name="MyKipiApplication" >

        <MenuBar>

            <Menu name="Image" ><text>&amp;Image</text>
                <DefineGroup name="kipi_image_group" append="kipi_image_group" />
            </Menu>

            <Menu name="Tools"><text>&amp;Tools</text>
                <DefineGroup name="kipi_album_group" append="kipi_album_group" />
                <Separator/>
                <DefineGroup name="kipi_tool_group" append="kipi_tool_group" />
                <Separator/>
                <DefineGroup name="kipi_batch_group" append="kipi_batch_group" />
            </Menu>

            <Merge/>

        </MenuBar>

        <ToolBar name="mainToolBar">
            <text>Main Toolbar</text>
        </Toolbar>

        <ActionProperties/>

    </gui>

    \endcode

    Then loading plugins into menus could be done with code similar to this implementation:

    \code

    class MyKipiApplication : public KXmlGuiWindow
    {
        Q_OBJECT

    public:

        MyKipiApplication();

    private Q_SLOTS:

        void slotKipiPluginPlug();

    private:

        KIPI::Interface*    m_iface;
        KIPI::PluginLoader* m_loader;
    };

    // -------------------------------------------------------------------------------

    MyKipiApplication::MyKipiApplication() : KXmlGuiWindow(0)
    {
        m_iface  = new KIPI::Interface(this, "MyKipiApplication_KIPI_interface");
        m_loader = new KIPI::PluginLoader(this);
        m_loader->setInterface(m_iface);
        m_loader->init();

        connect(m_loader, SIGNAL(replug()),
                this, SLOT(slotKipiPluginPlug()));

        m_loader->loadPlugins();
        // Do not delete m_loader as long as any of the plugins are in use
    }

    void MyKipiApplication::slotKipiPluginPlug()
    {
        QList<QAction*> kipiImageActions, kipiExportActions, kipiToolsActions;
        PluginLoader::PluginList list = m_loader->pluginList();

        // We need to remove loaded plugins from the gui factory
        for (PluginLoader::PluginList::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it )
        {
            Plugin* plugin = (*it)->plugin();
            if ( !plugin || !(*it)->shouldLoad() )
                continue;

            guiFactory()->removeClient(plugin);
        }

        for (PluginLoader::PluginList::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it )
        {
            Plugin* plugin = (*it)->plugin();
            if ( !plugin || !(*it)->shouldLoad() )
                continue;

            plugin->setup(this);
        }

        // We add plugins to the factory
        for (PluginLoader::PluginList::ConstIterator it = list.constBegin(); it != list.constEnd(); ++it )
        {
            Plugin* plugin = (*it)->plugin();
            if ( !plugin || !(*it)->shouldLoad() )
                continue;

            guiFactory()->addClient(plugin);
        }
    }

    \endcode

    For a implementation sample used to manage Kipi-plugins in host application,
    look the code of the kxmlkipicmd test application in the "test" folder from libkipi.

    To configure which plugins should be loaded, simply call
    PluginLoader::configWidget(), and insert the widget into your normal
    configuration dialog.
*/
class LIBKIPI_EXPORT PluginLoader : public QObject
{
    Q_OBJECT

public:

    /**
     * \class Info pluginloader.h <KIPI/PluginLoader>
     *
     * Info about a plugin.
     */
    class LIBKIPI_EXPORT Info
    {

    public:

        Info(KXmlGuiWindow* const parent, const KService::Ptr& service, bool shouldLoad);
        ~Info();

        QString       name()             const;
        QString       uname()            const;
        QString       author()           const;
        QString       comment()          const;
        QIcon         icon()             const;
        QString       library()          const;
        KService::Ptr service()          const;
        Plugin*       plugin()           const;
        QStringList   pluginCategories() const;

        void reload();
        bool shouldLoad() const;
        void setShouldLoad(bool);

    private:

        class Private;
        std::unique_ptr<Private> const d;
    };

public:

    typedef QList<Info*> PluginList;

public:

    /**
     * Use this constructor if your application does not use KDE XML GUI technology.
     * 
     * Note that the PluginLoader is intended to be a singleton, so you
     * should create only one and then use instance().
     */
    PluginLoader();

    /**
     * Standard constructor. You must pass the instance of KDE XML GUI application as argument.
     * 
     * Note that the PluginLoader is intended to be a singleton, so you
     * should create only one and then use instance().
     * 
     * @param parent the pointer to the KXmlGuiWindow of your application
     */
    PluginLoader(KXmlGuiWindow* const parent);

    /**
     * Standard destructor
     * 
     * Since PluginLoader is a singleton, you should not call this
     * in client code.
     */
    ~PluginLoader() override;

    /**
     * Set KIPI interface instance from host application.
     */
    void setInterface(Interface* const interface);

    /**
     * Return KIPI host interface instance.
     */
    Interface* interface() const;

    /**
     * Set Plugins ignore list, with name of obsoletes plugins to not load through init().
     */
    void setIgnoredPluginsList(const QStringList& ignores);

    /**
     * Set disabled plugin actions that will not be plugged into the gui,
     */
    void setDisabledPluginActions(const QStringList& disabledActions);

    /**
     * Return the list of disabled plugin actions
     */
    QStringList disabledPluginActions() const;

    /**
     * Init plugin loader. Call this method to parse relevant plugins installed on your system.
     * Before to call this method, you must setup KIPI interface instance.
     * Optionally, setup list of plugins to ignore, the constraint list, and
     * the disabled plugin actions
     */
    void init();

    /**
     * Call this method to load relevant plugins installed on your system to your KIPI host application
     * @note Plugins can be loaded through Info item.
     */
    void loadPlugins();

    /**
     * Returns the list of loaded plugins
     */
    const PluginList& pluginList();

    /**
     * Return the kipi-plugins version installed on your computer if it's found through kipiplugins.desktop file.
     */
    QString kipiPluginsVersion() const;

    /**
     * Return the config widget with list of plugins to manage.
     */
    ConfigWidget* configWidget(QWidget* const parent) const;

    /**
     * Returns plugin loader instance.
     */
    static PluginLoader* instance();

Q_SIGNALS:

    void plug(KIPI::PluginLoader::Info*);
    void unplug(KIPI::PluginLoader::Info*);

    /// @note Plugin can be plugged through Info item.
    void replug();

private:

    class Private;
    std::unique_ptr<Private> const d;

private:

    friend class ConfigWidget;
};

} // namespace KIPI

#endif /* KIPI_PLUGINLOADER_H */
