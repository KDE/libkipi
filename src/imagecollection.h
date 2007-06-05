/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : image collection
 *
 * Copyright (C) 2004-2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

/** @file imagecollection.h */

#ifndef KIPI_IMAGECOLLECTION_H
#define KIPI_IMAGECOLLECTION_H

// Qt includes

#include <qstring.h>
#include <qdatetime.h>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "libkipi/libkipi_export.h"

/** @namespace KIPI */
namespace KIPI
{
    class ImageCollectionShared;

    /** @class ImageCollection 
        Holds info about the collection
      */
    class LIBKIPI_EXPORT ImageCollection
    {
    public:
        QString name() const;
        QString comment() const;
        QString category() const;
        QDate date() const;
        KURL::List images() const;
        KURL path() const;
        KURL uploadPath() const;
        KURL uploadRoot() const;
        QString uploadRootName() const;
        bool isDirectory() const;
        bool isValid() const;

        // Interface for host application + general stuff
        ImageCollection( ImageCollectionShared* );
        ImageCollection( const ImageCollection& other );
        ~ImageCollection();
        ImageCollection();
        ImageCollection& operator=( const ImageCollection& );

        bool operator==(const ImageCollection&) const;
    private:
        mutable KIPI::ImageCollectionShared* _data;
        void printNullError() const;
    };
}

#endif /* IMAGECOLLECTION_H */

