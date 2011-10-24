/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  plugin loader
 *
 * @author Copyright (C) 2004-2010 by Gilles Caulier
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
class ConfigWidget;

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

        QIcon icon() const;

        Plugin* plugin() const;

        void reload();

        /** @p deprecated. You do not want to change the plugin instance. */
        void KDE_DEPRECATED setPlugin(Plugin*);

        bool shouldLoad() const;
        void setShouldLoad(bool);

    private:

        class InfoPrivate;
        InfoPrivate* const d;
    };

public:

    typedef QList<Info*> PluginList;

    PluginLoader( const QStringList& ignores, KIPI::Interface* const interface );
    PluginLoader( const QStringList& ignores, KIPI::Interface* const interface, const QString& constraint );
    virtual ~PluginLoader();

    ConfigWidget* configWidget( QWidget* parent ) const;

    const PluginList& pluginList();

    /** @p deprecated now plugins will be loaded when requested to the Info item */
    void loadPlugins();

    /** @p deprecated now plugins will be loaded when requested to the Info item */
    void KDE_DEPRECATED loadPlugin(Info* const);

    void construct(const QStringList& ignores, KIPI::Interface* const interface, const QString& constraint);
    KIPI::Interface* interface() const;

    static PluginLoader* instance();

Q_SIGNALS:

    void plug( KIPI::PluginLoader::Info* );
    void unplug( KIPI::PluginLoader::Info* );

    /** @deprecated */
    void replug();

private:

    class PluginLoaderPrivate;
    PluginLoaderPrivate* const d;

private:

    friend class ConfigWidget;
    friend class PluginCheckBox;
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

    class ConfigWidgetPrivate;
    ConfigWidgetPrivate* const d;
};

} // namespace KIPI

#endif /* PLUGINLOADER_H */
