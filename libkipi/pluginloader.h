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


namespace KIPI
{
    class Plugin;
    class Interface;

    class PluginLoader
    {
    public:
        PluginLoader( Interface* interface );
        static PluginLoader* instance();

        typedef QPtrList<KIPI::Plugin> List;

        const List& pluginList();

        QStringList availablePlugins() const;
        QStringList loadedPlugins() const;

        void    loadPlugins();
        void    loadPlugins(const QStringList& names);
        Plugin* pluginIsLoaded(const QString& name);

    private:
        static PluginLoader* m_instance;
        void init();

        List         m_pluginList;
        QStringList  m_pluginNames;
        QStringList  m_libraryNames;
        Interface* m_interface;

    };
}

#endif /* PLUGINLOADER_H */
