/***************************************************************************
 *   Copyright (C) 2004 by Jesper K. Pedersen                              *
 *   blackie@kde.org                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mainwindow.h"

#include <qtextedit.h>
#include <kmainwindow.h>
#include <klocale.h>
#include <kstdaction.h>
#include <kstatusbar.h>
#include <qapplication.h>
#include <kparts/componentfactory.h>
#include <kdebug.h>
#include "demointerface.h"
#include <libkipi/pluginloader.h>
#include <qpopupmenu.h>
#include <libkipi/plugin.h>
#include <kactioncollection.h>

MainWindow::MainWindow()
    : KMainWindow( 0, "MainWindow" )
{
    // set the shell's ui resource file
//    setXMLFile("plugindemoappui.rc");

    // The editor instance
    _editor = new QTextEdit( this );
    setCentralWidget( _editor );

    KStdAction::quit( qApp, SLOT( quit() ), actionCollection() );

    statusBar()->message( "Wellcome to the KIPI demo", 3000 );
    createGUI(); // This must be called before plugins are loaded.

    // Sets up the plugin interface, and load the plugins
    _pluginInterface = new QObject( this, "_pluginInterface" );
    DemoInterface* interface = new DemoInterface( this, "demo interface" );
    PluginLoader* loader = new PluginLoader( interface );
    loader->loadPlugins();

    KIPI::PluginLoader::List list = loader->pluginList();
    for( QPtrListIterator<KIPI::Plugin> it( list ); *it; ++it ) {
        KIPI::Plugin* plugin = *it;
        if ( plugin->category() == KIPI::IMAGESPLUGIN ) {
            QPopupMenu *popup = static_cast<QPopupMenu*>(factory()->container("images",this));
            Q_ASSERT( popup );
            KActionCollection *actions = plugin->actionCollection();
            for (unsigned int i=0; i < actions->count(); i++) {
                actions->action(i)->plug( popup );
            }
        }
    }
}


void MainWindow::loadPlugins()
{
#ifdef TEMPORARILY_REMOVED
    KTrader::OfferList offers = KTrader::self()->query("PluginDemo/Plugin");

    KTrader::OfferList::ConstIterator iter;
    for(iter = offers.begin(); iter != offers.end(); ++iter) {
        KService::Ptr service = *iter;
        int errCode = 0;
        PluginDemo::Plugin* plugin =
            KParts::ComponentFactory::createInstanceFromService<PluginDemo::Plugin>
            ( service, _pluginInterface, 0, QStringList(), &errCode);
        // here we ought to check the error code.

        if (plugin) {
            guiFactory()->addClient(plugin);

            kdDebug() << "PluginDemo: Loaded plugin "
                      << plugin->name() << endl;
        }

    }
#endif
}

#include "mainwindow.moc"
