/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : plugin loader
 *
 * Copyright (C) 2004-2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2004-2005 by Renchi Raju <renchi.raju at kdemail.net>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

/** @file pluginloader.cpp */

// Qt include.

#include <QStringList>
#include <QCheckBox>
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

// Local includes.

#include "plugin.h"
#include "interface.h"
#include "pluginloader.h"
#include "pluginloader.moc"

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
    void slotReplug() {
      unplugActionList( QString::fromLatin1("file_actions") );
      unplugActionList( QString::fromLatin1("image_actions") );
      unplugActionList( QString::fromLatin1("tool_actions") );

      QPtrList<KAction> fileActions;
      QPtrList<KAction> imageActions;
      QPtrList<KAction> toolsActions;

      KIPI::PluginLoader::PluginList list = _pluginLoader->pluginList();
      for( KIPI::PluginLoader::PluginList::Iterator it = list.begin(); it != list.end(); ++it ) {
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

          if ( popup ) {
              KActionPtrList actions = plugin->actions();
              for( KActionPtrList::Iterator it = actions.begin(); it != actions.end(); ++it ) {
                  popup->append( *it );
              }
          }
          else {
              kDebug() << "No menu found\n";
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

struct PluginLoader::Info::Private 
{
    KService::Ptr m_service;
    Plugin* m_plugin;
    bool m_shouldLoad;
};

PluginLoader::Info::Info(KService::Ptr service, bool shouldLoad)
{
    d = new Private;
    d->m_service    = service;
    d->m_plugin     = 0;
    d->m_shouldLoad = shouldLoad;
}

PluginLoader::Info::~Info()
{
    delete d;
}

KService::Ptr PluginLoader::Info::service() const
{
    return d->m_service;
}

QString PluginLoader::Info::name() const
{
    return d->m_service->name();
}

QString PluginLoader::Info::comment() const
{
    return d->m_service->comment();
}

QString PluginLoader::Info::library() const
{
    return d->m_service->library();
}

Plugin* PluginLoader::Info::plugin() const
{
    return d->m_plugin;
}

void PluginLoader::Info::setPlugin(Plugin* plugin)
{
    d->m_plugin=plugin;
}

bool PluginLoader::Info::shouldLoad() const
{
    return d->m_shouldLoad;
}

void PluginLoader::Info::setShouldLoad(bool value)
{
    d->m_shouldLoad=value;
}

//---------------------------------------------------------------------

static PluginLoader* s_instance = 0;

struct PluginLoader::Private
{
    PluginList  m_pluginList;
    Interface*  m_interface;
    QStringList m_ignores;
};

PluginLoader::PluginLoader( const QStringList& ignores, Interface* interface )
{
    Q_ASSERT( s_instance == 0 );
    s_instance = this;

    d = new Private;
    d->m_interface = interface;
    d->m_ignores   = ignores;

    const KService::List offers = KServiceTypeTrader::self()->query("KIPI/Plugin");
    KSharedConfigPtr config     = KGlobal::config();
    KConfigGroup group          = config->group( QString::fromLatin1( "KIPI/EnabledPlugin" ) );

    KService::List::ConstIterator iter;
    for(iter = offers.begin(); iter != offers.end(); ++iter) 
    {
        KService::Ptr service = *iter;
        QString name    = service->name();
        QString comment = service->comment();
        QString library = service->library();
        QStringList reqFeatures = service->property( QString::fromLatin1( "X-KIPI-ReqFeatures" ) ).toStringList();

        if (library.isEmpty() || name.isEmpty() ) 
        {
            kWarning( 51001 ) << "KIPI::PluginLoader: Plugin had an empty name or library file - this should not happen.";
            continue;
        }

        if ( d->m_ignores.contains( name ) ) 
        {
            kDebug( 51001 ) << "KIPI::PluginLoader: plugin " << name << " is in the ignore list for host application";
            continue;
        }

        bool appHasAllReqFeatures=true;
        for( QStringList::Iterator featureIt = reqFeatures.begin(); featureIt != reqFeatures.end(); ++featureIt ) 
        {
            if ( !d->m_interface->hasFeature( *featureIt ) ) 
            {
                kDebug( 51001 ) << "Plugin " << name << " was not loaded because the host application is missing\n"
                                 << "the feature " << *featureIt << endl;
				appHasAllReqFeatures=false;
				break;
            }
        }

        bool load = group.readEntry( name, true );

        if (!appHasAllReqFeatures)
            continue;

        Info* info = new Info( service, load );
        d->m_pluginList.append( info );
    }
}

PluginLoader::~PluginLoader()
{
    delete d;
}

void PluginLoader::loadPlugins()
{
    for( PluginList::Iterator it = d->m_pluginList.begin(); it != d->m_pluginList.end(); ++it ) 
    {
        loadPlugin( *it );
    }
    emit replug();
}

void PluginLoader::loadPlugin( Info* info )
{
    if ( info->plugin() == 0 && info->shouldLoad() )
    {
        QString error;
        Plugin *plugin = info->service()->createInstance<Plugin>(d->m_interface, QVariantList(), &error);
        if (plugin)
        {
            kDebug( 51001 ) << "KIPI::PluginLoader: Loaded plugin " << plugin->objectName();
        }
        else
        {
            kWarning( 51001 ) << "KIPI::PluginLoader:: createInstance returned 0 for "
                               << info->name()
                               << " (" << info->library() << ")"
                               << " with error: "
                               << error << endl;
        }
        info->setPlugin(plugin);
    }
    if ( info->plugin() ) // Do not emit if we had trouble loading the plugin.
        emit PluginLoader::instance()->plug( info );
}

const PluginLoader::PluginList& PluginLoader::pluginList()
{
    return d->m_pluginList;
}

PluginLoader* PluginLoader::instance()
{
    Q_ASSERT( s_instance != 0);
    return s_instance;
}

//---------------------------------------------------------------------

ConfigWidget* PluginLoader::configWidget( QWidget* parent )
{
    return new ConfigWidget( parent );
}

class PluginCheckBox : public QCheckBox
{
public:

    PluginCheckBox( PluginLoader::Info* info, QWidget* parent )
        : QCheckBox( info->comment(), parent ), info( info )
        {
            setChecked( info->shouldLoad() );
        }
    PluginLoader::Info* info;
};

struct ConfigWidget::Private
{
    QList< PluginCheckBox* > _boxes;
};

ConfigWidget::ConfigWidget(QWidget* parent)
            : QAbstractScrollArea(parent)
{
    d = new Private;
    QWidget* top = new QWidget( viewport() );
    setViewport( top );

    QVBoxLayout* lay = new QVBoxLayout(top);
    lay->setMargin(KDialog::marginHint());
    lay->setSpacing(KDialog::spacingHint());

    PluginLoader::PluginList list = PluginLoader::instance()->d->m_pluginList;
    for( PluginLoader::PluginList::Iterator it = list.begin(); it != list.end(); ++it ) 
    {
        PluginCheckBox* cb = new PluginCheckBox( *it, top );
        lay->addWidget( cb );
        d->_boxes.append( cb );
    }

    lay->addStretch(10);
}

ConfigWidget::~ConfigWidget()
{
    delete d;
}

void ConfigWidget::apply()
{
    KSharedConfigPtr config = KGlobal::config();
    KConfigGroup group = config->group( QString::fromLatin1( "KIPI/EnabledPlugin" ) );
    bool changes = false;

    for( QList<PluginCheckBox*>::Iterator it = d->_boxes.begin(); it != d->_boxes.end(); ++it ) 
    {
        bool orig = (*it)->info->shouldLoad();
        bool load = (*it)->isChecked();
        if ( orig != load ) 
        {
            changes = true;
            group.writeEntry( (*it)->info->name(), load );
            (*it)->info->setShouldLoad(load);
            if ( load ) 
            {
                PluginLoader::instance()->loadPlugin( (*it)->info);
            }
            else 
            {
                if ( (*it)->info->plugin() ) // Do not emit if we had trouble loading plugin.
                    emit PluginLoader::instance()->unplug( (*it)->info);
            }
        }
    }
    emit PluginLoader::instance()->replug();
}

} // namespace KIPI

