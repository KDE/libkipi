/* ============================================================
 * File  : plugin.h
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Date  : 2004-02-18
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

#ifndef KIPI_PLUGIN_H
#define KIPI_PLUGIN_H

#include <qobject.h>
#include "libkipi/interface.h"
#include <kactioncollection.h>

class KInstance;

namespace KIPI
{
    enum Category {
        IMAGESPLUGIN = 0,
        EFFECTSPLUGIN,
        TOOLSPLUGIN,
        IMPORTPLUGIN,
        EXPORTPLUGIN
    };

    class Plugin : public QObject
    {
    public:
        Plugin( KInstance* instance, QObject *parent, const char* name);
        virtual ~Plugin();

        KActionPtrList actions();
        virtual Category category() const = 0;

    protected:
        KActionCollection* actionCollection();
        void addAction( KAction* action );

    private:
        KActionCollection *m_actionCollection;
        KInstance* m_instance;
        KActionPtrList m_actions;
    };

}

#endif /* KIPI_PLUGIN_H */
