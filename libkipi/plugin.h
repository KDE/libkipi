/* ============================================================
 * File   : plugin.h
 * Authors: KIPI team developers
 *	    
 * Date   : 2004-02
 * Description :
 *
 * Copyright 2004 by the KIPI team
 * 
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

#ifndef KIPI_PLUGIN_H
#define KIPI_PLUGIN_H

// Qt includes.

#include <qobject.h>

// KDE includes.

#include <kaction.h>

// Lib KIPI includes.

#include "libkipi/interface.h"

class KInstance;

namespace KIPI
{
    enum Category 
       {
       IMAGESPLUGIN = 0,
       EFFECTSPLUGIN,
       TOOLSPLUGIN,
       IMPORTPLUGIN,
       EXPORTPLUGIN,
       BATCHPLUGIN,
       COLLECTIONSPLUGIN,
       UNDEFINEDPLUGIN
       };

    class Plugin : public QObject
    {
    public:
        Plugin( KInstance* instance, QObject *parent, const char* name);
        virtual ~Plugin();

        virtual void setup( QWidget* widget ) = 0;
        KActionPtrList actions( QWidget* parent = 0 );
        KActionCollection* actionCollection( QWidget* parent = 0 );
        virtual Category category() const;
        virtual Category category( KAction* action );

    protected:
        void addAction( KAction* action );

    private:
        QMap<QWidget*, KActionCollection*> m_actionCollection;
        KInstance* m_instance;
        QMap<QWidget*, KActionPtrList> m_actions;
        QWidget* m_defaultWidget;
    };
}

#endif  // KIPI_PLUGIN_H 
