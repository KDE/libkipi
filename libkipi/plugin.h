/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : plugin interface
 *
 * Copyright (C) 2004-2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2004-2005 by Renchi Raju <renchi.raju at kdemail.net>
 * Copyright (C) 2004-2005 by Jesper K. Pedersen <blackie at kde.org>
 * Copyright (C) 2004-2005 by Aurelien Gateau <aurelien dot gateau at free.fr>
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

#ifndef KIPI_PLUGIN_H
#define KIPI_PLUGIN_H

// Qt includes.

#include <QtCore/QObject>
#include <QtCore/QList>

// Local includes.

#include "libkipi_export.h"

class KAction;
class KActionCollection;
class KComponentData;

/** @namespace KIPI */
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
       COLLECTIONSPLUGIN
    };

    class LIBKIPI_EXPORT Plugin : public QObject
    {
        Q_OBJECT

    public:

        Plugin(const KComponentData& instance, QObject *parent, const char* name);
        virtual ~Plugin();

        virtual void setup( QWidget* widget ) = 0;
        QList<KAction*> actions( QWidget* parent = 0 );
        KActionCollection* actionCollection( QWidget* parent = 0 );
        virtual Category category( KAction* action ) const = 0;

    protected:

        void addAction( KAction* action );

    private:

        struct Private;
        Private* d;
    };

} // namespace KIPI

#endif  // KIPI_PLUGIN_H 
