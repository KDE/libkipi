#include "helloplugin.h"
#include <kgenericfactory.h>
#include <qwidget.h>
#include <kaction.h>
#include <kshortcut.h>
#include <kdebug.h>
#include <qiconset.h>



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

K_EXPORT_COMPONENT_FACTORY( libkipi_hello,
                            KGenericFactory<HelloPlugin>( "libkipi_hello" ) )

HelloPlugin::HelloPlugin( QObject* parent, const char* name, const QStringList& )
    : KIPI::Plugin( parent, name )
{
    kdDebug() << "Loaded hello plugin\n";

    m_interface = dynamic_cast< KIPI::Interface* >( parent );
    Q_ASSERT( m_interface );

    new KAction( i18n("Say hello"), QIconSet(), KShortcut(), this, SLOT(go()), actionCollection(), "hello_plugin" );
}

void HelloPlugin::go()
{
    kdDebug() << "Hello world!\n";
    (void) m_interface->currentAlbum();
}

KIPI::Plugin::Category HelloPlugin::category() const
{
    return KIPI::Plugin::IMAGESPLUGIN;
}

#include "helloplugin.moc"
