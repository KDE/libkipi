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

using namespace KIPI;

KIPI::PluginLoader* KIPI::PluginLoader::m_instance = 0;

KIPI::PluginLoader::PluginLoader( const QStringList& ignores, Interface* interface )
    : m_interface( interface ), m_ignores( ignores )
{
    Q_ASSERT( m_instance == 0 );
    m_pluginList.setAutoDelete(true);
    init();
}

void KIPI::PluginLoader::init()
{
    m_libraryNames.clear();
    m_pluginNames.clear();

    KTrader::OfferList offers = KTrader::self()->query("KIPI/Plugin");

    KTrader::OfferList::ConstIterator iter;
    for(iter = offers.begin(); iter != offers.end(); ++iter) {

        KService::Ptr service = *iter;
        QString name    = service->name();
        QString library = service->library();
        QStringList reqFeatures = service->property( QString::fromLatin1( "X-KIPI-ReqFeatures" ) ).toStringList();

        if (library.isEmpty() || name.isEmpty() || m_ignores.contains( name ) )
            continue;

		bool appHasAllReqFeatures=true;
        for( QStringList::Iterator featureIt = reqFeatures.begin(); featureIt != reqFeatures.end(); ++featureIt ) {
            if ( !m_interface->hasFeature( *featureIt ) ) {
                kdDebug( 51001 ) << "Plugin " << name << " was not loaded due to the host application was missing\n"
                                 << "the feature " << *featureIt << endl;
				appHasAllReqFeatures=false;
				break;
            }
        }

		if (appHasAllReqFeatures) {
	        m_pluginNames.append(name);
    	    m_libraryNames.append(library);
		}
    }
}

const KIPI::PluginLoader::List& KIPI::PluginLoader::pluginList()
{
    return m_pluginList;
}

QStringList KIPI::PluginLoader::availablePlugins() const
{
    return m_pluginNames;
}

QStringList KIPI::PluginLoader::loadedPlugins() const
{
    QStringList plist;

    QPtrListIterator<KIPI::Plugin> it(m_pluginList);

    while (Plugin *plugin = it.current()) {
        ++it;
        plist.append(plugin->name());
    }

    return plist;
}

void KIPI::PluginLoader::loadPlugins()
{
    m_pluginList.clear();

    for (QStringList::Iterator it = m_libraryNames.begin();
         it != m_libraryNames.end(); ++it) {

        Plugin *plugin =
            KParts::ComponentFactory
            ::createInstanceFromLibrary<KIPI::Plugin>((*it).local8Bit().data(), m_interface );

        if (plugin) {
            m_pluginList.append(plugin);
            kdDebug( 51001 ) << "KIPI::PluginLoader: Loaded plugin " << plugin->name() << endl;
        }
    }
}

void KIPI::PluginLoader::loadPlugins(const QStringList& names)
{
    QStringList::Iterator itLib;
    QStringList::Iterator itName = m_pluginNames.begin();

    for (itLib = m_libraryNames.begin(), itName = m_pluginNames.begin() ;
         itLib != m_libraryNames.end() && itName != m_pluginNames.end();
         ++itName, ++itLib) {

        if (!names.contains(*itName)) {

            Plugin* plugin = pluginIsLoaded(*itName);
            if (plugin)
                m_pluginList.remove(plugin);
            continue;
        }

        if (!pluginIsLoaded(*itName)) {

            Plugin *plugin =
                KParts::ComponentFactory
                ::createInstanceFromLibrary<KIPI::Plugin>(
                    (*itLib).local8Bit().data());

            if (plugin) {
                m_pluginList.append(plugin);
                kdDebug( 51001 ) << "KIPI::PluginLoader: Loaded plugin " << plugin->name() << endl;
            }
        }

    }
}

Plugin* KIPI::PluginLoader::pluginIsLoaded(const QString& name)
{
    for (Plugin* plugin = m_pluginList.first(); plugin;
         plugin = m_pluginList.next()) {

        if (plugin->name() == name) {
            return plugin;
        }
    }

    return 0;
}


PluginLoader* KIPI::PluginLoader::instance()
{
    Q_ASSERT( m_instance != 0);
    return m_instance;
}

