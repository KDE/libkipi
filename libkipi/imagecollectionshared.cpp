/* ============================================================
 * File   : imagecollectionshared.cpp
 * Authors: KIPI team developers (see AUTHORS files for details)
 *
 * Date   : 2004-02
 * Description :
 *
 * Copyright 2004 by the KIPI team
 *
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
    kdWarning(51000) << "This method should only be invoked if this imagecollection is a directory.\n"
                     <<  "See KIPI::ImageCollectionShared::isDirectory()"
                     << endl;
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

bool KIPI::ImageCollectionShared::isDirectory()
{
    return false;
}

QString KIPI::ImageCollectionShared::comment()
{
    kdWarning(51000) << "KIPI::ImageCollectionShared::comment should only be invoked if the host application supports\n"
        "the KIPI::Features AlbumsHaveComments - if the host application do support that, then this function should\n"
        "have been overriden in the host application.\n";
    return QString::null;
}

QString KIPI::ImageCollectionShared::category()
{
    kdWarning(51000) << "KIPI::ImageCollectionShared::category should only be invoked if the host application supports\n"
        "the KIPI::Features AlbumsHaveCategory - if the host application do support that, then this function should\n"
        "have been overriden in the host application.\n";
    return QString::null;
}

QDate KIPI::ImageCollectionShared::date()
{
    kdWarning(51000) << "KIPI::ImageCollectionShared::date should only be invoked if the host application supports\n"
        "the KIPI::Features AlbumsHaveCreationDate - if the host application do support that, then this function should\n"
        "have been overriden in the host application.\n";
    return QDate();
}


bool KIPI::ImageCollectionShared::operator==(ImageCollectionShared& ics) {
    return images()==ics.images();
}
