/* ============================================================
 * File   : imagecollectionshared.cpp
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
 
// KDE Includes.

#include <klocale.h>
#include <kdebug.h>

// Local Includes.

#include "imagecollectionshared.h"


KIPI::ImageCollectionShared::ImageCollectionShared()
     : _count(1)
{
}

void KIPI::ImageCollectionShared::addRef()
{
    _count++;
}

void KIPI::ImageCollectionShared::removeRef()
{
    _count--;
    if ( _count == 0 ) {
        // qDebug("Deleting!");
        delete this;
    }
}

KURL KIPI::ImageCollectionShared::path()
{
    kdWarning(51000) << "This method should only be invoked if the host application supports the KIPI::Features\n"
            "AlbumEQDir - if the host application do support that, then this function should\n"
            "have been overriden in the host application.\n";
    return KURL();
}

KURL KIPI::ImageCollectionShared::uploadPath()
{
    kdWarning(51000) << "This method should only be invoked if the host application supports the KIPI::Features\n"
            "AcceptNewImages - if the host application do support that, then this function should\n"
            "have been overriden in the host application.\n";
    return KURL();
}

KURL KIPI::ImageCollectionShared::uploadRoot()
{
    KURL path = uploadPath();
    if ( path.isValid() ) {
        path.setPath("/");
        return path;
    }
    else
        return KURL( "file:/" );
}

QString KIPI::ImageCollectionShared::uploadRootName()
{
   return (i18n("Images"));
}

QString KIPI::ImageCollectionShared::comment()
{
    kdWarning(51000) << "This method should only be invoked if the host application supports the KIPI::Features\n"
            "AlbumsHaveComments - if the host application do support that, then this function should\n"
            "have been overriden in the host application.\n";
    return QString::null;
}
