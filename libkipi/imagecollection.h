/* ============================================================
 * File   : imagecollection.h
 * Authors: KIPI team developers (see AUTHORS files for details)
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
 
#ifndef KIPI_IMAGECOLLECTION_H
#define KIPI_IMAGECOLLECTION_H

// Qt includes

#include <qstring.h>
#include <qdatetime.h>

// KDE includes.

#include <kurl.h>

namespace KIPI
{
    class ImageCollectionShared;

    class ImageCollection
    {
    public:
        // Interface for plugins.
        QString name() const;
        QString comment() const;
        QString category() const;
        QDate date() const;
        KURL::List images() const;
        KURL path() const;
        KURL uploadPath() const;
        KURL uploadRoot() const;
        QString uploadRootName() const;
        bool isValid() const;

        // Interface for host application + general stuff
        ImageCollection( ImageCollectionShared* );
        ImageCollection( const ImageCollection& other );
        ~ImageCollection();
        ImageCollection();
        ImageCollection& operator=( const ImageCollection& );

    private:
        mutable KIPI::ImageCollectionShared* _data;
        void printNullError() const;
    };
}

#endif /* IMAGECOLLECTION_H */

