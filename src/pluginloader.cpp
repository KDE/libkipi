/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2004-02-01
 * @brief  plugin loader
 *
 * @author Copyright (C) 2004-2018 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2009      by Andi Clemens
 *         <a href="mailto:andi dot clemens at googlemail dot com">andi dot clemens at googlemail dot com</a>
 * @author Copyright (C) 2009      by Aleix Pol Gonzalez
 *         <a href="mailto:aleixpol at kde dot org">aleixpol at kde dot org</a>
 * @author Copyright (C) 2012-2013 by Victor Dodon
 *         <a href="mailto:dodonvictor at gmail dot com">dodonvictor at gmail dot com</a>
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

#include "pluginloader.h"

// Qt includes

#include <QStringList>
#include <QVariantList>
#include <QVariant>
#include <QAction>
#include <QStandardPaths>

// KF includes

#include <KServiceTypeTrader>
#include <KSharedConfig>
#include <KDesktopFile>
#include <KConfigGroup>
#include <KXMLGUIFactory>
#include <KToolBar>
#include <KActionCollection>

// Local includes

#include "plugin.h"
#include "interface.h"
#include "configwidget.h"
#include "libkipi_version.h"
#include "libkipi_config.h"
#include "libkipi_debug.h"

namespace KIPI
{

class Q_DECL_HIDDEN PluginLoader::Info::Private
{
public:

    Private()
    {
        shouldLoad = false;
        plugin     = nullptr;
        parent     = nullptr;
    }

    bool           shouldLoad;
    KService::Ptr  service;
    Plugin*        plugin;
    KXmlGuiWindow* parent;
};

PluginLoader::Info::Info(KXmlGuiWindow* const parent, const KService::Ptr& service, bool shouldLoad)
    : d(new Private)
{
    d->service    = service;
    d->shouldLoad = shouldLoad;
    d->parent     = parent;
}

PluginLoader::Info::~Info()
{
    if (d->parent && d->plugin)
    {
        d->parent->guiFactory()->removeClient(d->plugin);

        const auto toolBars = d->parent->toolBars();
        for (KToolBar* const toolbar : toolBars)
        {
            toolbar->removeXMLGUIClient(d->plugin);
        }
    }

    delete d->plugin;
}

KService::Ptr PluginLoader::Info::service() const
{
    return d->service;
}

QString PluginLoader::Info::name() const
{
    return d->service->name();
}

QString PluginLoader::Info::uname() const
{
    return d->service->untranslatedGenericName();
}

QString PluginLoader::Info::author()  const
{
    return d->service->property(QString::fromLatin1("author"), QVariant::String).toString();
}

QString PluginLoader::Info::comment() const
{
    return d->service->comment();
}

QString PluginLoader::Info::library() const
{
    return d->service->library();
}

QIcon PluginLoader::Info::icon() const
{
    if (d->service->icon().isEmpty() && d->plugin)
    {
        if (!d->plugin->actions().isEmpty() && d->plugin->actions().first())
        {
            return d->plugin->actions().first()->icon();
        }
        else
        {
            return QIcon();
        }
    }
    else
    {
        return QIcon::fromTheme(d->service->icon());
    }
}

Plugin* PluginLoader::Info::plugin() const
{
    if (!d->plugin && shouldLoad())
    {
        QString error;

        d->plugin = d->service->createInstance<Plugin>(PluginLoader::instance()->interface(), QVariantList(), &error);

        if (d->plugin)
        {
            qCDebug(LIBKIPI_LOG) << "Loaded plugin " << d->plugin->objectName();

            Q_EMIT (PluginLoader::instance()->plug(const_cast<Info*>(this)));
        }
        else
        {
            qCWarning(LIBKIPI_LOG) << "Cannot create instance for plugin "
                                   << name()
                                   << " (" << library() << ")"
                                   << " with error: "
                                   << error;
        }
    }

    return d->plugin;
}

QStringList PluginLoader::Info::pluginCategories() const
{
    return d->service->property(QString::fromLatin1("X-KIPI-PluginCategories")).toStringList();
}

void PluginLoader::Info::reload()
{
    if (d->parent)
    {
        d->parent->guiFactory()->removeClient(d->plugin);

        const auto toolBars = d->parent->toolBars();
        for (KToolBar* const toolbar : toolBars)
        {
            toolbar->removeXMLGUIClient(d->plugin);
        }
    }

    delete d->plugin;
    d->plugin = nullptr;
}

bool PluginLoader::Info::shouldLoad() const
{
    return d->shouldLoad;
}

void PluginLoader::Info::setShouldLoad(bool value)
{
    d->shouldLoad = value;
}

//---------------------------------------------------------------------

static PluginLoader* s_instance = nullptr;
static bool          s_loaded   = false;

class Q_DECL_HIDDEN PluginLoader::Private
{
public:

    Private()
    {
        interface = nullptr;
        parent    = nullptr;
    }

    QStringList              ignoredPlugins;
    QStringList              disabledActions;

    KXmlGuiWindow*           parent;

    PluginLoader::PluginList pluginList;
    Interface*               interface;
};

PluginLoader::PluginLoader()
    : d(new Private)
{
    Q_ASSERT((s_instance == nullptr) && (!s_loaded));
    s_instance = this;
}

PluginLoader::PluginLoader(KXmlGuiWindow* const parent)
    : d(new Private)
{
    Q_ASSERT((s_instance == nullptr) && (!s_loaded));
    s_instance = this;

    if (!parent)
    {
        qWarning(LIBKIPI_LOG) << "KDE XML application instance is null...";
    }

    d->parent = parent;
}

void PluginLoader::setInterface(Interface* const interface)
{
    d->interface = interface;
    setParent(interface);
}

void PluginLoader::setIgnoredPluginsList(const QStringList& ignores)
{
    d->ignoredPlugins = ignores;
}

void PluginLoader::setDisabledPluginActions(const QStringList& disabledActions)
{
    d->disabledActions = disabledActions;
}

QStringList PluginLoader::disabledPluginActions() const
{
    return d->disabledActions;
}

void PluginLoader::init()
{
    Q_ASSERT((s_instance != nullptr) && (!s_loaded));

    if (!d->interface)
    {
        qWarning(LIBKIPI_LOG) << "KIPI host interface instance is null. No plugin will be loaded...";
        return;
    }

    s_loaded                    = true;
    const KService::List offers = KServiceTypeTrader::self()->query(QString::fromLatin1("KIPI/Plugin"));
    KSharedConfigPtr config     = KSharedConfig::openConfig();
    KConfigGroup group          = config->group(QString::fromLatin1("KIPI/EnabledPlugin"));

    for (KService::List::ConstIterator iter = offers.begin(); iter != offers.end(); ++iter)
    {
        KService::Ptr service   = *iter;
        QString name            = service->name();
        QString uname           = service->untranslatedGenericName();
        QString library         = service->library();
        QStringList reqFeatures = service->property(QString::fromLatin1("X-KIPI-ReqFeatures")).toStringList();
        int binVersion          = service->property(QString::fromLatin1("X-KIPI-BinaryVersion")).toInt();

        if (library.isEmpty() || uname.isEmpty())
        {
            qCWarning(LIBKIPI_LOG) << "Plugin had an empty name or library file - this should not happen.";
            continue;
        }

        if (d->ignoredPlugins.contains(uname))
        {
            qCDebug(LIBKIPI_LOG) << "Plugin " << name << " (generic name: " << uname << ") is in the ignore list from host application";
            continue;
        }

        if (binVersion != kipi_binary_version)
        {
            qCDebug(LIBKIPI_LOG) << "Plugin " << name
                                 << "has a SO version (" << binVersion
                                 << ") which is different than libkipi ABI version (" << kipi_binary_version << "). "
                                 << "Refusing to load.";
            continue;
        }

        bool appHasAllReqFeatures = true;

        for (QStringList::const_iterator featureIt = reqFeatures.constBegin();
             featureIt != reqFeatures.constEnd(); ++featureIt)
        {
            if (!d->interface->hasFeature(*featureIt))
            {
                qCDebug(LIBKIPI_LOG) << "Plugin " << name << " was not loaded because the host application is missing\n"
                                     << "the feature " << *featureIt;
                appHasAllReqFeatures = false;
                break;
            }
        }

        bool load = group.readEntry(uname, true);

        if (!appHasAllReqFeatures)
        {
            continue;
        }

        Info* const info = new Info(d->parent, service, load);
        d->pluginList.append(info);
    }
}

PluginLoader::~PluginLoader() = default;

void PluginLoader::loadPlugins()
{
    Q_EMIT replug(); // added for convenience, now they can be loaded on demand
}

const PluginLoader::PluginList& PluginLoader::pluginList()
{
    return d->pluginList;
}

PluginLoader* PluginLoader::instance()
{
    if (!s_instance)
    {
        qCDebug(LIBKIPI_LOG) << "Instance is null...";
    }

    return s_instance;
}

Interface* PluginLoader::interface() const
{
    return d->interface;
}

ConfigWidget* PluginLoader::configWidget(QWidget* const parent) const
{
    return new ConfigWidget(parent);
}

QString PluginLoader::kipiPluginsVersion() const
{
    QString ver;
    QString path = QStandardPaths::locate(QStandardPaths::ApplicationsLocation, QString::fromLatin1("kipiplugins.desktop"));

    KDesktopFile desk(path);
    QMap<QString, QString> map = desk.entryMap(QString::fromLatin1("X-KipiPlugins Entry"));

    if (!map.isEmpty())
    {
        QString val = map[QString::fromLatin1("Version")];

        if (!val.isEmpty())
            ver = val;
    }

    return ver;
}

} // namespace KIPI
