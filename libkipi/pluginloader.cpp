/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin loader
 *
 * @author Copyright (C) 2004-2010 by Gilles Caulier
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

/** @file pluginloader.cpp */

#include "pluginloader.moc"

// Qt include.

#include <QStringList>
#include <QLayout>
#include <QList>
#include <QVariantList>

// KDE includes.

#include <kservicetypetrader.h>
#include <kdebug.h>
#include <kdialog.h>
#include <ksharedconfig.h>
#include <kconfig.h>
#include <kglobal.h>
#include <klibloader.h>
#include <kaction.h>

// Local includes.

#include "plugin.h"
#include "interface.h"
#include "version.h"

/**
   \author Renchi Raju
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

   Then plugin plugins into menus could be done with code similar to this from KimDaBa:
   \code
    void slotReplug()
    {
        unplugActionList( QString::fromLatin1("file_actions") );
        unplugActionList( QString::fromLatin1("image_actions") );
        unplugActionList( QString::fromLatin1("tool_actions") );

        QPtrList<KAction> fileActions;
        QPtrList<KAction> imageActions;
        QPtrList<KAction> toolsActions;

        KIPI::PluginLoader::PluginList list = _pluginLoader->pluginList();
        for( KIPI::PluginLoader::PluginList::Iterator it = list.begin(); it != list.end(); ++it )
        {
            KIPI::Plugin* plugin = (*it)->plugin;
            if ( !plugin || !(*it)->shouldLoad )
                continue;

            plugin->setup( this );
            QPtrList<KAction>* popup = 0;
            if ( plugin->category() == KIPI::IMAGESPLUGIN )
                popup = &imageActions;

            else if ( plugin->category() == KIPI::EXPORTPLUGIN  ||
                        plugin->category() == KIPI::IMPORTPLUGIN )
                popup = &fileActions;

            else if ( plugin->category() == KIPI::TOOLSPLUGIN )
                popup = &toolsActions;

            if ( popup )
            {
                KActionPtrList actions = plugin->actions();
                for( KActionPtrList::Iterator it = actions.begin(); it != actions.end(); ++it )
                {
                    popup->append( *it );
                }
            }
            else
            {
                kDebug(51001) << "No menu found\n";
            }
            plugin->actionCollection()->readShortcutSettings();
        }

        // For this to work I need to pass false as second arg for createGUI
        plugActionList( QString::fromLatin1("file_actions"), fileActions );
        plugActionList( QString::fromLatin1("image_actions"), imageActions );
        plugActionList( QString::fromLatin1("tool_actions"), toolsActions );
    }
    \endcode

  To configure which plugins should be loaded, simply call
  PluginLoader::configWidget(), and insert the widget into your normal
  configuration dialog.
*/
namespace KIPI
{

//---------------------------------------------------------------------

class PluginLoader::Info::InfoPrivate
{
public:

    InfoPrivate()
    {
        shouldLoad = false;
        plugin     = 0;
    };

    bool          shouldLoad;
    KService::Ptr service;
    Plugin*       plugin;
};

PluginLoader::Info::Info(const KService::Ptr& service, bool shouldLoad)
                  : d(new InfoPrivate)
{
    d->service    = service;
    d->shouldLoad = shouldLoad;
}

PluginLoader::Info::~Info()
{
    delete d->plugin;
    delete d;
}

KService::Ptr PluginLoader::Info::service() const
{
    return d->service;
}

QString PluginLoader::Info::name() const
{
    return d->service->name();
}

QString PluginLoader::Info::comment() const
{
    return d->service->comment();
}

QString PluginLoader::Info::library() const
{
    return d->service->library();
}

QIcon PluginLoader::Info::icon() const
{
    if(d->service->icon().isEmpty() && d->plugin)
    {
        if (!d->plugin->actions().isEmpty() && d->plugin->actions().first())
            return d->plugin->actions().first()->icon();
        else
            return QIcon();
    }
    else
    {
        return KIcon(d->service->icon());
    }
}

Plugin* PluginLoader::Info::plugin() const
{
    if ( !d->plugin && shouldLoad() )
    {
        QString error;
        Plugin* plugin = d->service->createInstance<Plugin>(PluginLoader::instance()->interface(), QVariantList(), &error);
        if (plugin)
        {
            kDebug( 51001 ) << "KIPI::PluginLoader: Loaded plugin " << plugin->objectName();

            QVariant binaryVersion = plugin->property("KipiBinaryVersion");
            if (!binaryVersion.isValid() || binaryVersion.toInt() != kipi_binary_version)
            {
                kDebug( 51001 ) << "KIPI::PluginLoader: Plugin " << plugin->objectName()
                                << "is compiled for a different libkipi ABI version."
                                << "Refusing to load.";
                delete plugin;
                plugin = 0;
            }
        }
        else
        {
            kWarning( 51001 ) << "KIPI::PluginLoader:: createInstance returned 0 for "
                              << name()
                              << " (" << library() << ")"
                              << " with error: "
                              << error;
        }

        d->plugin = plugin;

        if ( d->plugin ) // Do not emit if we had trouble loading the plugin.
            emit PluginLoader::instance()->plug( const_cast<Info*>(this) );
    }

    return d->plugin;
}

void PluginLoader::Info::reload()
{
    delete d->plugin;
    d->plugin = 0;
}

void PluginLoader::Info::setPlugin(Plugin* const plugin)
{
    delete d->plugin;
    d->plugin = plugin;
}

bool PluginLoader::Info::shouldLoad() const
{
    return d->shouldLoad;
}

void PluginLoader::Info::setShouldLoad(bool value)
{
    d->shouldLoad = value;
}

//---------------------------------------------------------------------

static PluginLoader* s_instance = 0;

class PluginLoader::PluginLoaderPrivate
{
public:

    PluginLoaderPrivate()
    {
        interface = 0;
    };

    PluginLoader::PluginList pluginList;
    Interface*               interface;
};

PluginLoader::PluginLoader( const QStringList& ignores, Interface* const interface)
            : d(new PluginLoaderPrivate)
{
    construct(ignores, interface, QString());
}

PluginLoader::PluginLoader( const QStringList& ignores, Interface* const interface, const QString& constraint )
            : d(new PluginLoaderPrivate)
{
    construct(ignores, interface, constraint);
}

void PluginLoader::construct( const QStringList& ignores, Interface* const interface, const QString& constraint )
{
    Q_ASSERT( s_instance == 0 );
    s_instance                  = this;
    d->interface                = interface;
    const KService::List offers = KServiceTypeTrader::self()->query("KIPI/Plugin", constraint);
    KSharedConfigPtr config     = KGlobal::config();
    KConfigGroup group          = config->group( QString::fromLatin1( "KIPI/EnabledPlugin" ) );

    KService::List::ConstIterator iter;
    for(iter = offers.begin(); iter != offers.end(); ++iter)
    {
        KService::Ptr service   = *iter;
        QString name            = service->name();
        QString library         = service->library();
        QStringList reqFeatures = service->property( QString::fromLatin1( "X-KIPI-ReqFeatures" ) ).toStringList();

        if (library.isEmpty() || name.isEmpty() )
        {
            kWarning( 51001 ) << "KIPI::PluginLoader: Plugin had an empty name or library file - this should not happen.";
            continue;
        }

        if ( ignores.contains( name ) )
        {
            kDebug( 51001 ) << "KIPI::PluginLoader: plugin " << name
                            << " is in the ignore list for host application";
            continue;
        }

        bool appHasAllReqFeatures = true;
        for( QStringList::const_iterator featureIt = reqFeatures.constBegin();
             featureIt != reqFeatures.constEnd(); ++featureIt )
        {
            if ( !d->interface->hasFeature( *featureIt ) )
            {
                kDebug( 51001 ) << "Plugin " << name << " was not loaded because the host application is missing\n"
                                << "the feature " << *featureIt;
                appHasAllReqFeatures = false;
                break;
            }
        }

        bool load = group.readEntry( name, true );

        if (!appHasAllReqFeatures)
            continue;

        Info* info = new Info( service, load );
        d->pluginList.append( info );
    }
}

PluginLoader::~PluginLoader()
{
    qDeleteAll(d->pluginList);
    delete d;
}

void PluginLoader::loadPlugins()
{
    emit replug(); //added for convenience, now they will be loaded on demand
}

///deprecated
void PluginLoader::loadPlugin( Info* const )
{
}

const PluginLoader::PluginList& PluginLoader::pluginList()
{
    return d->pluginList;
}

PluginLoader* PluginLoader::instance()
{
    if(!s_instance)
        kDebug( 51001 ) << "KIPI::PluginLoader::instance is null...";

    return s_instance;
}

Interface* PluginLoader::interface() const
{
    return d->interface;
}

ConfigWidget* PluginLoader::configWidget(QWidget* parent) const
{
    return new ConfigWidget(parent);
}

// ---------------------------------------------------------------------

class PluginCheckBox : public QListWidgetItem
{
public:

    PluginCheckBox(PluginLoader::Info* const info, QListWidget* parent)
                               : QListWidgetItem(parent, QListWidgetItem::UserType), info(info)
    {
        setText(QString("%1  (%2)").arg(info->name(), info->comment()));
        setIcon(info->icon());
        setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        setCheckState(info->shouldLoad() ? Qt::Checked : Qt::Unchecked);
    }

    PluginLoader::Info* info;
};

class ConfigWidget::ConfigWidgetPrivate
{
public:

    ConfigWidgetPrivate()
    {
    };

    QList<PluginCheckBox*> boxes;
};

ConfigWidget::ConfigWidget(QWidget* parent)
            : QListWidget(parent), d(new ConfigWidgetPrivate)
{
    setAutoFillBackground(false);
    setSortingEnabled(true);
    viewport()->setAutoFillBackground(false);

    foreach(PluginLoader::Info *info, PluginLoader::instance()->pluginList())
    {
        PluginCheckBox* cb = new PluginCheckBox(info, this);
        d->boxes.append(cb);
    }
}

ConfigWidget::~ConfigWidget()
{
    delete d;
}

void ConfigWidget::apply()
{
    KSharedConfigPtr config = KGlobal::config();
    KConfigGroup group      = config->group( QString::fromLatin1( "KIPI/EnabledPlugin" ) );
//    bool changes            = false;

    for( QList<PluginCheckBox*>::Iterator it = d->boxes.begin(); it != d->boxes.end(); ++it )
    {
        bool orig = (*it)->info->shouldLoad();
        bool load = ((*it)->checkState() == Qt::Checked) ? true : false;

        if ( orig != load )
        {
//            changes = true;
            group.writeEntry( (*it)->info->name(), load );
            (*it)->info->setShouldLoad(load);

            if ( load )
            {
                (*it)->info->reload();
            }
            else if ( (*it)->info->plugin() ) // Do not emit if we had trouble loading plugin.
            {
                emit PluginLoader::instance()->unplug( (*it)->info);
            }
        }
    }

    emit PluginLoader::instance()->replug();
}

} // namespace KIPI
