/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin loader
 *
 * @author Copyright (C) 2004-2014 by Gilles Caulier
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

#include "pluginloader.moc"

// Qt includes

#include <QStringList>
#include <QLayout>
#include <QList>
#include <QVariantList>
#include <QVariant>

// KDE includes

#include <kservicetypetrader.h>
#include <kdebug.h>
#include <kdialog.h>
#include <kdeversion.h>
#include <ksharedconfig.h>
#include <kdesktopfile.h>
#include <kconfig.h>
#include <kglobal.h>
#include <klibloader.h>
#include <kaction.h>
#include <kxmlguifactory.h>
#include <ktoolbar.h>
#include <kstandarddirs.h>
#include <kactioncollection.h>

// Local includes

#include "plugin.h"
#include "interface.h"
#include "version.h"
#include "configwidget.h"

static const KCatalogLoader loader("libkipi");

namespace KIPI
{

class PluginLoader::Info::Private
{
public:

    Private()
    {
        shouldLoad = false;
        plugin     = 0;
        parent     = 0;
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
#if KDE_IS_VERSION(4,8,5)
        foreach(KToolBar* const toolbar, d->parent->toolBars())
        {
            toolbar->removeXMLGUIClient(d->plugin);
        }
#endif
    }

    delete d->plugin;
    delete d;
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
    return d->service->property(QString("author"), QVariant::String).toString();
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
        return KIcon(d->service->icon());
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
            kDebug(51001) << "Loaded plugin " << d->plugin->objectName();

            emit (PluginLoader::instance()->plug(const_cast<Info*>(this)));
        }
        else
        {
            kWarning(51001) << "Cannot create instance for plugin "
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
#if KDE_IS_VERSION(4,8,5)
        foreach(KToolBar* const toolbar, d->parent->toolBars())
        {
            toolbar->removeXMLGUIClient(d->plugin);
        }
#endif
    }

    delete d->plugin;
    d->plugin = 0;
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

static PluginLoader* s_instance = 0;
static bool          s_loaded   = false;

class PluginLoader::Private
{
public:

    Private()
    {
        interface = 0;
        parent    = 0;
    };

    QStringList              ignoredPlugins;
    QStringList              disabledActions;

    KXmlGuiWindow*           parent;

    PluginLoader::PluginList pluginList;
    Interface*               interface;
};

PluginLoader::PluginLoader()
    : d(new Private)
{
    Q_ASSERT((s_instance == 0) && (!s_loaded));
    s_instance = this;

    KGlobal::dirs()->addResourceDir("data", KStandardDirs::installPath("data") + QString("kipi"));
}

PluginLoader::PluginLoader(KXmlGuiWindow* const parent)
    : d(new Private)
{
    Q_ASSERT((s_instance == 0) && (!s_loaded));
    s_instance = this;

    KGlobal::dirs()->addResourceDir("data", KStandardDirs::installPath("data") + QString("kipi"));

    if (!parent)
    {
        kWarning() << "KDE XML application instance is null...";
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
    Q_ASSERT((s_instance != 0) && (!s_loaded));

    if (!d->interface)
    {
        kWarning() << "KIPI host interface instance is null. No plugin will be loaded...";
        return;
    }

    s_loaded                    = true;
    const KService::List offers = KServiceTypeTrader::self()->query("KIPI/Plugin");
    KSharedConfigPtr config     = KGlobal::config();
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
            kWarning(51001) << "Plugin had an empty name or library file - this should not happen.";
            continue;
        }

        if (d->ignoredPlugins.contains(uname))
        {
            kDebug(51001) << "Plugin " << name << " (generic name: " << uname << ") is in the ignore list from host application";
            continue;
        }

        if (binVersion != kipi_binary_version)
        {
            kDebug(51001) << "Plugin " << name
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
                kDebug(51001) << "Plugin " << name << " was not loaded because the host application is missing\n"
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

PluginLoader::~PluginLoader()
{
    delete d;
}

void PluginLoader::loadPlugins()
{
    emit replug(); // added for convenience, now they can be loaded on demand
}

const PluginLoader::PluginList& PluginLoader::pluginList()
{
    return d->pluginList;
}

PluginLoader* PluginLoader::instance()
{
    if (!s_instance)
    {
        kDebug(51001) << "Instance is null...";
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
    QString ver                = i18nc("Version unavailable", "unavailable");
    QString path               = KGlobal::dirs()->installPath("xdgdata-apps") + QString("kipiplugins.desktop");
    KDesktopFile desk(path);
    QMap<QString, QString> map = desk.entryMap("X-KipiPlugins Entry");

    if (!map.isEmpty())
    {
        QString val = map["Version"];

        if (!val.isEmpty())
            ver = val;
    }

    return ver;
}

} // namespace KIPI
