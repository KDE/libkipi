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
        struct Info
        {
            Info( const QString& name, const QString& comment, const QString& library, Plugin* plugin )
                : name(name), comment(comment), library(library), plugin( plugin ) {}
            Info() {}
            QString name;
            QString comment;
            QString library;
            Plugin* plugin;
        };

    public:
        PluginLoader( const QStringList& ignores, Interface* interface );
        static PluginLoader* instance();

        typedef QValueList<Info> List;

        const List& pluginList();

    private:
        static PluginLoader* m_instance;
        void init();

        List         m_pluginList;
        Interface* m_interface;
        QStringList m_ignores;
    };
}

#endif /* PLUGINLOADER_H */
