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
    init();
}

void KIPI::PluginLoader::init()
{
    KTrader::OfferList offers = KTrader::self()->query("KIPI/Plugin");

    KTrader::OfferList::ConstIterator iter;
    for(iter = offers.begin(); iter != offers.end(); ++iter) {

        KService::Ptr service = *iter;
        QString name    = service->name();
        QString comment = service->comment();
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
            Plugin *plugin =
                KParts::ComponentFactory
                ::createInstanceFromLibrary<KIPI::Plugin>(library.local8Bit().data(), m_interface );

            if (plugin) {
                kdDebug( 51001 ) << "KIPI::PluginLoader: Loaded plugin " << plugin->name() << endl;
                m_pluginList.append( Info( name, comment, library, plugin ) );
            }
            else {
                kdWarning( 51001 ) << "KIPI::PluginLoader: failed to load plugin " << library << endl;
            }
        }
    }
}

const KIPI::PluginLoader::List& KIPI::PluginLoader::pluginList()
{
    return m_pluginList;
}

PluginLoader* KIPI::PluginLoader::instance()
{
    Q_ASSERT( m_instance != 0);
    return m_instance;
}

