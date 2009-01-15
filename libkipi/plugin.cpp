/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : plugin interface
 *
 * Copyright (C) 2004-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include <QWidget>

// KDE includes.

#include <kaction.h>
#include <kactioncollection.h>
#include <kcomponentdata.h>
#include <kdebug.h>

// Local includes.

#include "plugin.h"
#include "plugin.moc"

namespace KIPI
{

class PluginPrivate
{
public:

    PluginPrivate()
    {
        m_defaultWidget = 0;
    };

    QMap<QWidget*, KActionCollection*> m_actionCollection;
    KComponentData                     m_instance;
    QMap< QWidget*, QList<KAction*> >  m_actions;
    QWidget*                           m_defaultWidget;
};

Plugin::Plugin(const KComponentData& instance, QObject *parent, const char* name)
      : QObject(parent), d(new PluginPrivate)
{
    d->m_instance = instance;
    setObjectName(name);
}

Plugin::~Plugin()
{
    delete d;
}

KActionCollection* Plugin::actionCollection( QWidget* widget )
{
    if ( widget == 0 )
        widget = d->m_defaultWidget;

    if (!d->m_actionCollection.contains( widget ))
        kWarning( 51000 ) << "Error in the plugin. The plugin needs to call Plugin::setup( QWidget* ) "
                          << "as the very first line when overriding the setup method." << endl;
    return d->m_actionCollection[widget];
}

void Plugin::addAction( KAction* action )
{
    d->m_actions[d->m_defaultWidget].append( action );
}

QList<KAction*> Plugin::actions( QWidget* widget )
{
    if ( widget == 0 )
        widget = d->m_defaultWidget;

    return d->m_actions[widget];
}

void Plugin::setup( QWidget* widget )
{
    d->m_defaultWidget = widget;
    d->m_actions.insert( widget, QList<KAction*>() );
    d->m_actionCollection.insert( widget, new KActionCollection( widget, d->m_instance ) );
}

} // namespace KIPI
