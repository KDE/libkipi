/* ============================================================
 * File   : plugin.cpp
 * Authors: KIPI team developers
 *	    
 * Date   : 2004-02
 * Description :
 *
 * Copyright 2004 by the KIPI team
 * 
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * ============================================================ */

// Qt includes.
 
#include <qwidget.h>

// KDE includes.

#include <kaction.h>
#include <kinstance.h>
#include <kdebug.h>

// Local includes.

#include "plugin.h"

KIPI::Plugin::Plugin( KInstance* instance, QObject *parent, const char* name)
            : QObject( parent, name), m_instance( instance )
{
}

KIPI::Plugin::~Plugin()
{
}

KActionCollection* KIPI::Plugin::actionCollection( QWidget* widget )
{
    if ( widget == 0 )
        widget = m_defaultWidget;

    if (!m_actionCollection.contains( widget ))
        kdWarning( 51000 ) << "Error in the plugin. The plugin needs to call Plugin::setup( QWidget* ) "
                           << "as the very first line when overriding the setup method." << endl;
    return m_actionCollection[widget];
}

void KIPI::Plugin::addAction( KAction* action )
{
    m_actions[m_defaultWidget].append( action );
}

KActionPtrList KIPI::Plugin::actions( QWidget* widget )
{
    if ( widget == 0 )
        widget = m_defaultWidget;

    return m_actions[widget];
}

void KIPI::Plugin::setup( QWidget* widget )
{
    m_defaultWidget = widget;
    m_actions.insert( widget, KActionPtrList() );
    QString name = QString( "action collection for %1" ).arg( widget->name() );
    m_actionCollection.insert( widget, new KActionCollection( widget, widget, name.latin1(), m_instance ) );
}



