/* ============================================================
 * File  : pluginloader.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2004-02-19
 * Description :
 *
 * Copyright 2004 by Renchi Raju

 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include <ktrader.h>
#include <kparts/componentfactory.h>
#include <kdebug.h>

#include "plugin.h"
#include "pluginloader.h"
#include "interface.h"
#include <kconfig.h>
#include <qcheckbox.h>
#include <qlayout.h>

/**
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

   Then plugin plugins into menus could be done with code similiar to this from KimDaBa:
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
              kdDebug() << "No menu found\n";
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
using namespace KIPI;

KIPI::PluginLoader* KIPI::PluginLoader::m_instance = 0;

KIPI::PluginLoader::PluginLoader( const QStringList& ignores, Interface* interface )
    : m_interface( interface ), m_ignores( ignores )
{
    Q_ASSERT( m_instance == 0 );
    m_instance = this;

    KTrader::OfferList offers = KTrader::self()->query("KIPI/Plugin");
    KConfig* config = KGlobal::config();
    config->setGroup( QString::fromLatin1( "KIPI/EnabledPlugin" ) );

    KTrader::OfferList::ConstIterator iter;
    for(iter = offers.begin(); iter != offers.end(); ++iter) {

        KService::Ptr service = *iter;
        QString name    = service->name();
        QString comment = service->comment();
        QString library = service->library();
        QStringList reqFeatures = service->property( QString::fromLatin1( "X-KIPI-ReqFeatures" ) ).toStringList();

        if (library.isEmpty() || name.isEmpty() ) {
            kdWarning( 51001 ) << "KIPI::PluginLoader: Plugin had an empty name or library file - this should not happen." << endl;
            continue;
        }

        bool loadDefault = true;
        if ( m_ignores.contains( name ) ) {
            kdDebug( 51001 ) << "KIPI::PluginLoader: plugin " << name << " was in the ignore for host application list" << endl;
            loadDefault = false;
        }

		bool appHasAllReqFeatures=true;
        for( QStringList::Iterator featureIt = reqFeatures.begin(); featureIt != reqFeatures.end(); ++featureIt ) {
            if ( !m_interface->hasFeature( *featureIt ) ) {
                kdDebug( 51001 ) << "Plugin " << name << " was not loaded due to the host application was missing\n"
                                 << "the feature " << *featureIt << endl;
				appHasAllReqFeatures=false;
				break;
            }
        }

        bool load = config->readBoolEntry( name, loadDefault );

		if (!appHasAllReqFeatures)
            continue;

        Info* info = new Info( name, comment, library, load );
        m_pluginList.append( info );
    }
}

void KIPI::PluginLoader::loadPlugins()
{
    for( PluginList::Iterator it = m_pluginList.begin(); it != m_pluginList.end(); ++it ) {
        loadPlugin( *it );
    }
    emit replug();
}

void KIPI::PluginLoader::loadPlugin( Info* info )
{
    if ( info->plugin == 0 && info->shouldLoad ) {
        Plugin *plugin = 0;
        plugin =  KParts::ComponentFactory
                  ::createInstanceFromLibrary<KIPI::Plugin>(info->library.local8Bit().data(), m_interface );

        if (plugin)
            kdDebug( 51001 ) << "KIPI::PluginLoader: Loaded plugin " << plugin->name() << endl;
        else
            kdWarning( 51001 ) << "KIPI::PluginLoader:: createInstanceFromLibrary returned 0";
        info->plugin = plugin;
    }
    if ( info->plugin ) // Do not emit if we had trouble loading the plugin.
        emit PluginLoader::instance()->plug( info );
}


const KIPI::PluginLoader::PluginList& KIPI::PluginLoader::pluginList()
{
    return m_pluginList;
}

PluginLoader* KIPI::PluginLoader::instance()
{
    Q_ASSERT( m_instance != 0);
    return m_instance;
}

KIPI::ConfigWidget* KIPI::PluginLoader::configWidget( QWidget* parent )
{
    return new KIPI::ConfigWidget( parent );
}


class PluginCheckBox :public QCheckBox
{
public:
    PluginCheckBox( KIPI::PluginLoader::Info* info, QWidget* parent )
        : QCheckBox( info->comment, parent ), info( info )
        {
            setChecked( info->shouldLoad );
        }
    KIPI::PluginLoader::Info* info;
};

KIPI::ConfigWidget::ConfigWidget( QWidget* parent )
    :QScrollView( parent, "KIPI::PluginLoader::ConfigWidget" )
{
    QWidget* top = new QWidget( viewport() );
    addChild( top );
    setResizePolicy( AutoOneFit );

    QVBoxLayout* lay = new QVBoxLayout( top, 0, 6 );

    KIPI::PluginLoader::PluginList list = PluginLoader::instance()->m_pluginList;
    for( KIPI::PluginLoader::PluginList::Iterator it = list.begin(); it != list.end(); ++it ) {
        PluginCheckBox* cb = new PluginCheckBox( *it, top );
        lay->addWidget( cb );
        _boxes.append( cb );
    }
}

void KIPI::ConfigWidget::apply()
{
    KConfig* config = KGlobal::config();
    config->setGroup( QString::fromLatin1( "KIPI/EnabledPlugin" ) );
    bool changes = false;

    for( QValueList<PluginCheckBox*>::Iterator it = _boxes.begin(); it != _boxes.end(); ++it ) {
        bool orig = (*it)->info->shouldLoad;
        bool load = (*it)->isChecked();
        if ( orig != load ) {
            changes = true;
            config->writeEntry( (*it)->info->name, load );
            (*it)->info->shouldLoad = load;
            if ( load ) {
                PluginLoader::instance()->loadPlugin( (*it)->info );
            }
            else {
                if ( (*it)->info->plugin ) // Do not emit if we had trouble loading plugin.
                    emit PluginLoader::instance()->unplug( (*it)->info );
            }
        }
    }
    emit PluginLoader::instance()->replug();
}




#include "pluginloader.moc"
