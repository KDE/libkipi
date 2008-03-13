/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : image collection shared
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
 
#ifndef KIPI_IMAGECOLLECTIONSHARED_H
#define KIPI_IMAGECOLLECTIONSHARED_H

// Qt Includes.

#include <QtCore/QString>
#include <QtCore/QDateTime>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "imagecollection.h"
#include "libkipi_export.h"

namespace KIPI
{

    class LIBKIPI_EXPORT ImageCollectionShared
    {

    public:

        ImageCollectionShared();
        virtual ~ImageCollectionShared() {}
        virtual QString name() = 0;
        virtual QString comment();
        virtual QString category();
        virtual QDate date();
        virtual KUrl::List images() = 0;
        virtual KUrl path();
        virtual KUrl uploadPath();
        virtual KUrl uploadRoot();
        virtual QString uploadRootName();
        virtual bool isDirectory();
        virtual bool operator==(ImageCollectionShared&);

    private:

        friend class ImageCollection;
        void addRef();
        void removeRef();
        int _count;
    };

} // namespace KIPI

#endif /* IMAGECOLLECTIONSHARED_H */

