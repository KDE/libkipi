/* ============================================================
 * File  : pluginloader.h
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2004-02-19
 * Description :
 *
 * Copyright 2004 by Renchi Raju

 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef KIPI_PLUGINLOADER_H
#define KIPI_PLUGINLOADER_H

#include <qstringlist.h>
#include <qptrlist.h>
#include <libkipi/interface.h>
#include <qwidget.h>
#include <qscrollview.h>


class PluginCheckBox;
namespace KIPI
{
    class Plugin;
    class Interface;
    class ConfigWidget;

    class PluginLoader :public QObject
    {
        Q_OBJECT
    public:
        struct Info
        {
            Info( const QString& name, const QString& comment, const QString& library, bool shouldLoad )
                : name(name), comment(comment), library(library), plugin( 0 ),  shouldLoad( shouldLoad ) {}
            Info() {}
            QString name;
            QString comment;
            QString library;
            Plugin* plugin;
            bool shouldLoad;
        };

        PluginLoader( const QStringList& ignores, Interface* interface );
        void loadPlugins();
        virtual ~PluginLoader() {}
        static PluginLoader* instance();
        ConfigWidget* configWidget( QWidget* parent );

        typedef QValueList<Info*> PluginList;

        const PluginList& pluginList();

    signals:
        void plug( KIPI::PluginLoader::Info* );
        void unplug( KIPI::PluginLoader::Info* );
        void replug();

    private:
        friend class ConfigWidget;
        friend class PluginCheckBox;

        static PluginLoader* m_instance;
        void loadPlugin( Info* );

        PluginList m_pluginList;
        Interface* m_interface;
        QStringList m_ignores;
    };

    class ConfigWidget :public QScrollView
    {
        Q_OBJECT
    public:
        ConfigWidget( QWidget* parent );
    public slots:
        void apply();
    private:
        QValueList< PluginCheckBox* > _boxes;
    };
}

#endif /* PLUGINLOADER_H */
