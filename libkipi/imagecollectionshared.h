/* ============================================================
 * File   : imagecollectionshared.h
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
 
#ifndef KIPI_IMAGECOLLECTIONSHARED_H
#define KIPI_IMAGECOLLECTIONSHARED_H

// Qt Includes.

#include <qstring.h>
#include <qdatetime.h>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "imagecollection.h"

namespace KIPI
{
    class ImageCollectionShared
    {
    public:
        ImageCollectionShared();
        virtual ~ImageCollectionShared() {}
        virtual QString name() = 0;
        virtual QString comment();
        virtual QString category();
        virtual QDate date();
        virtual KURL::List images() = 0;
        virtual KURL path();
        virtual KURL uploadPath();
        virtual KURL uploadRoot();
        virtual QString uploadRootName();

    private:
        friend class ImageCollection;
        void addRef();
        void removeRef();
        int _count;
    };
}

#endif /* IMAGECOLLECTIONSHARED_H */

