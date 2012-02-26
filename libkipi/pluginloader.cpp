/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin loader
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2009 by Andi Clemens
 *         <a href="mailto:andi dot clemens at googlemail dot com">andi dot clemens at googlemail dot com</a>
 * @author Copyright (C) 2009 by Aleix Pol Gonzalez
 *         <a href="mailto:aleixpol at kde dot org">aleixpol at kde dot org</a>
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

/** @file pluginloader.cpp */

#include "pluginloader.moc"

// Qt include.

#include <QStringList>
#include <QLayout>
#include <QList>
#include <QVariantList>

// KDE includes

#include <kservicetypetrader.h>
#include <kdebug.h>
#include <kdialog.h>
#include <ksharedconfig.h>
#include <kconfig.h>
#include <kglobal.h>
#include <klibloader.h>
#include <kaction.h>

// Local includes

#include "plugin.h"
#include "interface.h"
#include "version.h"

namespace KIPI
{

class PluginLoader::Info::InfoPrivate
{
public:

    InfoPrivate()
    {
        shouldLoad = false;
        plugin     = 0;
    };

    bool          shouldLoad;
    KService::Ptr service;
    Plugin*       plugin;
};

PluginLoader::Info::Info(const KService::Ptr& service, bool shouldLoad)
    : d(new InfoPrivate)
{
    d->service    = service;
    d->shouldLoad = shouldLoad;
}

PluginLoader::Info::~Info()
{
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
        Plugin* plugin = d->service->createInstance<Plugin>(PluginLoader::instance()->interface(), QVariantList(), &error);

        if (plugin)
        {
            kDebug(51001) << "Loaded plugin " << plugin->objectName();

            QVariant binaryVersion = plugin->property("KipiBinaryVersion");

            if (!binaryVersion.isValid() || binaryVersion.toInt() != kipi_binary_version)
            {
                kDebug(51001) << "Plugin " << plugin->objectName()
                              << "is compiled for a different libkipi ABI version."
                              << "Refusing to load.";
                delete plugin;
                plugin = 0;
            }
        }
        else
        {
            kWarning(51001) << "CreateInstance returned 0 for "
                            << name()
                            << " (" << library() << ")"
                            << " with error: "
                            << error;
        }

        d->plugin = plugin;

        if (d->plugin)   // Do not emit if we had trouble loading the plugin.
        {
            emit (PluginLoader::instance()->plug(const_cast<Info*>(this)));
        }
    }

    return d->plugin;
}

void PluginLoader::Info::reload()
{
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

class PluginLoader::PluginLoaderPrivate
{
public:

    PluginLoaderPrivate()
    {
        interface = 0;
    };

    PluginLoader::PluginList pluginList;
    Interface*               interface;
};

PluginLoader::PluginLoader(const QStringList& ignores, Interface* const interface)
    : d(new PluginLoaderPrivate)
{
    construct(ignores, interface, QString());
}

PluginLoader::PluginLoader(const QStringList& ignores, Interface* const interface, const QString& constraint)
    : d(new PluginLoaderPrivate)
{
    construct(ignores, interface, constraint);
}

void PluginLoader::construct(const QStringList& ignores, Interface* const interface, const QString& constraint)
{
    Q_ASSERT(s_instance == 0);
    s_instance                  = this;
    d->interface                = interface;
    const KService::List offers = KServiceTypeTrader::self()->query("KIPI/Plugin", constraint);
    KSharedConfigPtr config     = KGlobal::config();
    KConfigGroup group          = config->group(QString::fromLatin1("KIPI/EnabledPlugin"));

    for (KService::List::ConstIterator iter = offers.begin(); iter != offers.end(); ++iter)
    {
        KService::Ptr service   = *iter;
        QString name            = service->name();
        QString library         = service->library();
        QStringList reqFeatures = service->property(QString::fromLatin1("X-KIPI-ReqFeatures")).toStringList();

        if (library.isEmpty() || name.isEmpty())
        {
            kWarning(51001) << "Plugin had an empty name or library file - this should not happen.";
            continue;
        }

        if (ignores.contains(name))
        {
            kDebug(51001) << "Plugin " << name << " is in the ignore list from host application";
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

        bool load = group.readEntry(name, true);

        if (!appHasAllReqFeatures)
        {
            continue;
        }

        Info* info = new Info(service, load);
        d->pluginList.append(info);
    }
}

PluginLoader::~PluginLoader()
{
    qDeleteAll(d->pluginList);
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

// ---------------------------------------------------------------------

class PluginCheckBox : public QListWidgetItem
{
public:

    PluginCheckBox(PluginLoader::Info* const info, QListWidget* const parent)
        : QListWidgetItem(parent, QListWidgetItem::UserType),
          info(info)
    {
        setText(QString("%1  (%2)").arg(info->name(), info->comment()));
        setIcon(info->icon());
        setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        setCheckState(info->shouldLoad() ? Qt::Checked : Qt::Unchecked);
    }

    PluginLoader::Info* info;
};

// ---------------------------------------------------------------------

class ConfigWidget::ConfigWidgetPrivate
{
public:

    ConfigWidgetPrivate()
    {
    };

    QList<PluginCheckBox*> boxes;
};

ConfigWidget::ConfigWidget(QWidget* const parent)
    : QListWidget(parent),
      d(new ConfigWidgetPrivate)
{
    setAutoFillBackground(false);
    setSortingEnabled(true);
    viewport()->setAutoFillBackground(false);

    foreach(PluginLoader::Info* info, PluginLoader::instance()->pluginList())
    {
        PluginCheckBox* cb = new PluginCheckBox(info, this);
        d->boxes.append(cb);
    }
}

ConfigWidget::~ConfigWidget()
{
    delete d;
}

void ConfigWidget::apply()
{
    KSharedConfigPtr config = KGlobal::config();
    KConfigGroup group      = config->group(QString::fromLatin1("KIPI/EnabledPlugin"));

    for (QList<PluginCheckBox*>::Iterator it = d->boxes.begin(); it != d->boxes.end(); ++it)
    {
        bool orig = (*it)->info->shouldLoad();
        bool load = ((*it)->checkState() == Qt::Checked);

        if (orig != load)
        {
            group.writeEntry((*it)->info->name(), load);
            (*it)->info->setShouldLoad(load);

            // Bugfix #289779 - Plugins are not really freed / unplugged when disabled in the kipi setup dialog, always call reload()
            // to reload plugins properly when the replug() signal is send.
            /*
            if (load)
            {
                (*it)->info->reload();
            }
            else if ((*it)->info->plugin())   // Do not emit if we had trouble loading plugin.
            {
                emit PluginLoader::instance()->unplug((*it)->info);
            }
            */
            (*it)->info->reload();
        }
    }

    emit PluginLoader::instance()->replug();
}

void ConfigWidget::slotCheckAll()
{
    for (int i = 0; i < count(); ++i)
    {
        item(i)->setCheckState(Qt::Checked);
    }
}

void ConfigWidget::slotClear()
{
    for (int i = 0; i < count(); ++i)
    {
        item(i)->setCheckState(Qt::Unchecked);
    }
}

} // namespace KIPI
