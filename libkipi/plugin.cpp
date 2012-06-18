/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin interface
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
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

// Qt includes

#include <QWidget>

// KDE includes

#include <kaction.h>
#include <kactioncollection.h>
#include <kcomponentdata.h>
#include <kdebug.h>

// Local includes

#include "version.h"
#include "interface.h"

namespace KIPI
{

class Plugin::PluginPrivate
{
public:

    PluginPrivate()
    {
        defaultWidget = 0;
    };

    QMap<QWidget*, KActionCollection*> actionCollection;
    KComponentData                     instance;
    QMap<QWidget*, QList<KAction*> >   actions;
    QWidget*                           defaultWidget;
};

Plugin::Plugin(const KComponentData& instance, QObject* const parent, const char* name)
      : QObject(parent), d(new PluginPrivate)
{
    d->instance = instance;
    setObjectName(name);

    setProperty("KipiBinaryVersion", kipi_binary_version);
}

Plugin::~Plugin()
{
    delete d;
}

KActionCollection* Plugin::widgetActionCollection(QWidget* const widget) const
{
    QWidget* w = !widget ? d->defaultWidget : widget;

    if (!d->actions.contains(w))
    {
        kWarning() << "Error in plugin. It needs to call Plugin::setup(QWidget*) "
                   << "as the very first line when overriding the setup method.";
    }

    return d->actionCollection[w];
}

QList<KAction*> Plugin::actions(QWidget* const widget) const
{
    QWidget* w = !widget ? d->defaultWidget : widget;

    if (!d->actions.contains(w))
    {
        kWarning() << "Error in plugin. It needs to call Plugin::setup(QWidget*) "
                   << "as the very first line when overriding the setup method.";
    }

    return d->actions[w];
}

void Plugin::addAction(KAction* const action)
{
    d->actions[d->defaultWidget].append(action);
}

void Plugin::setup(QWidget* const widget)
{
    d->defaultWidget = widget;
    d->actions.insert(widget, QList<KAction*>());
    d->actionCollection.insert(widget, new KActionCollection(widget, d->instance));
}

Interface* Plugin::interface() const
{
    return (dynamic_cast<Interface*>(parent()));
}

} // namespace KIPI
