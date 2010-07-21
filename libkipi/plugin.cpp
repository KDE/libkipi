/** ===========================================================
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin interface
 *
 * @author Copyright (C) 2004-2010 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Jesper K. Pedersen
 *         <a href="mailto:blackie at kde dot org">blackie at kde dot org</a>
 * @author Copyright (C) 2004-2005 by Aurelien Gateau
 *         <a href="mailto:aurelien dot gateau at free dot fr">aurelien dot gateau at free dot fr</a>
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

#include "plugin.moc"

// Qt includes.

#include <QWidget>

// KDE includes.

#include <kaction.h>
#include <kactioncollection.h>
#include <kcomponentdata.h>
#include <kdebug.h>

namespace KIPI
{

class Plugin::PluginPrivate
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

Plugin::Plugin(const KComponentData& instance, QObject* parent, const char* name)
      : QObject(parent), d(new PluginPrivate)
{
    d->m_instance = instance;
    setObjectName(name);
}

Plugin::~Plugin()
{
    delete d;
}

KActionCollection* Plugin::actionCollection( QWidget* widget ) const
{
    if ( widget == 0 )
        widget = d->m_defaultWidget;

    if (!d->m_actionCollection.contains( widget ))
        kWarning() << "Error in the plugin. The plugin needs to call Plugin::setup( QWidget* ) "
                   << "as the very first line when overriding the setup method.";

    return d->m_actionCollection[widget];
}

void Plugin::addAction( KAction* const action )
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
