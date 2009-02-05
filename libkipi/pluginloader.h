/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : plugin loader
 *
 * Copyright (C) 2004-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2009 by Andi Clemens <andi dot clemens at gmx dot net>
 * Copyright (C) 2004-2005 by Renchi Raju <renchi.raju at kdemail.net>
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

#ifndef KIPI_PLUGINLOADER_H
#define KIPI_PLUGINLOADER_H

// Qt includes.

#include <QtCore/QList>
#include <QtGui/QListWidget>

// KDE includes

#include <kservice.h>

// Local includes.

#include "libkipi_export.h"

namespace KIPI
{

class Plugin;
class Interface;
class PluginLoaderPrivate;
class ConfigWidget;
class ConfigWidgetPrivate;

class LIBKIPI_EXPORT PluginLoader : public QObject
{
    Q_OBJECT

public:

    class LIBKIPI_EXPORT Info
    {

    public:

        Info( const KService::Ptr& service, bool shouldLoad );
        ~Info();

        KService::Ptr service() const;

        QString name() const;

        QString comment() const;

        QString library() const;

        Plugin* plugin() const;
        void setPlugin(Plugin*);

        bool shouldLoad() const;
        void setShouldLoad(bool);

    private:

        struct Private;
        Private* d;
    };

public:

    typedef QList<Info*> PluginList;

    PluginLoader( const QStringList& ignores, Interface* interface );
    virtual ~PluginLoader();

    ConfigWidget* configWidget( QWidget* parent );

    const PluginList& pluginList();

    void loadPlugins();
    void loadPlugin(Info*);

    static PluginLoader* instance();

Q_SIGNALS:

    void plug( KIPI::PluginLoader::Info* );
    void unplug( KIPI::PluginLoader::Info* );
    void replug();

private:

    friend class ConfigWidget;
    friend class PluginCheckBox;

    PluginLoaderPrivate* const d;
};

// ----------------------------------------------------------------------

class LIBKIPI_EXPORT ConfigWidget : public QListWidget
{
    Q_OBJECT

public:

    ConfigWidget( QWidget* parent );
    ~ConfigWidget();

public Q_SLOTS:

    void apply();

private:

    ConfigWidgetPrivate* const d;
};

} // namespace KIPI

#endif /* PLUGINLOADER_H */
