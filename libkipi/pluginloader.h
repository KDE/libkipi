/* ============================================================
 * File  : pluginloader.h
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2004-02-19
 * Description :
 *
 * Copyright 2004 by Renchi Raju

 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * ============================================================ */

#ifndef KIPI_PLUGINLOADER_H
#define KIPI_PLUGINLOADER_H

#include <libkipi/interface.h>
#include <qwidget.h>
#include <qscrollview.h>


namespace KIPI
{
    class Plugin;
    class Interface;
    class ConfigWidget;

    class PluginLoader :public QObject
    {
        Q_OBJECT
    public:
        class Info
        {
		public:
            Info( const QString& name, const QString& comment, const QString& library, bool shouldLoad );
            ~Info();
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

        PluginLoader( const QStringList& ignores, Interface* interface );
        virtual ~PluginLoader();
        void loadPlugins();
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

        void loadPlugin( Info* );

		struct Private;
		Private* d;
    };

    class ConfigWidget :public QScrollView
    {
        Q_OBJECT
    public:
        ConfigWidget( QWidget* parent );
		~ConfigWidget();
    public slots:
        void apply();
    private:
		struct Private;
		Private* d;
    };
}

#endif /* PLUGINLOADER_H */
