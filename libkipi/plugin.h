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

class KActionCollection;

namespace KIPI
{

    class Plugin : public QObject
    {
    public:

        enum Category {
            IMAGESPLUGIN = 0,
            EFFECTSPLUGIN,
            TOOLSPLUGIN,
            IMPORTPLUGIN,
            EXPORTPLUGIN
        };

        Plugin( QObject *parent, const char* name);
        virtual ~Plugin();

        KActionCollection* actionCollection();

        virtual Category category() const = 0;
        virtual bool     mergeContextMenu() const;
        virtual bool     mergeToolBar() const;


    private:

        KActionCollection *m_actions;
    };

}

#endif /* KIPI_PLUGIN_H */
