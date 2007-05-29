/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : plugin interface
 *
 * Copyright (C) 2004-2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2004-2005 by Renchi Raju <renchi.raju at kdemail.net>
 * Copyright (C) 2004-2005 by Jesper K. Pedersen <blackie at kde.org>
 * Copyright (C) 2004-2005 by Aurelien Gateau <aurelien dot gateau at free.fr>
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

// Qt includes.
 
#include <qwidget.h>

// KDE includes.

#include <kaction.h>
#include <kinstance.h>
#include <kdebug.h>

// Local includes.

#include "plugin.h"

struct KIPI::Plugin::Private 
{
    QMap<QWidget*, KActionCollection*> m_actionCollection;
    KInstance* m_instance;
    QMap<QWidget*, KActionPtrList> m_actions;
    QWidget* m_defaultWidget;
};

KIPI::Plugin::Plugin( KInstance* instance, QObject *parent, const char* name)
            : QObject( parent, name)
{
    d=new Private;
    d->m_instance=instance;
}

KIPI::Plugin::~Plugin()
{
    delete d;
}

KActionCollection* KIPI::Plugin::actionCollection( QWidget* widget )
{
    if ( widget == 0 )
        widget = d->m_defaultWidget;

    if (!d->m_actionCollection.contains( widget ))
        kdWarning( 51000 ) << "Error in the plugin. The plugin needs to call Plugin::setup( QWidget* ) "
                           << "as the very first line when overriding the setup method." << endl;
    return d->m_actionCollection[widget];
}

void KIPI::Plugin::addAction( KAction* action )
{
    d->m_actions[d->m_defaultWidget].append( action );
}

KActionPtrList KIPI::Plugin::actions( QWidget* widget )
{
    if ( widget == 0 )
        widget = d->m_defaultWidget;

    return d->m_actions[widget];
}

void KIPI::Plugin::setup( QWidget* widget )
{
    d->m_defaultWidget = widget;
    d->m_actions.insert( widget, KActionPtrList() );
    QString name = QString( "action collection for %1" ).arg( widget->name() );
    d->m_actionCollection.insert( widget, new KActionCollection( widget, widget, name.latin1(), d->m_instance ) );
}
