/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : image collection shared
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
    if ( _count == 0 ) 
    {
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
            "have been overridden in the host application.\n" << endl;
    return KURL();
}

KURL KIPI::ImageCollectionShared::uploadRoot()
{
    KURL path = uploadPath();
    if ( path.isValid() ) 
    {
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
        "have been overridden in the host application.\n" << endl;
    return QString::null;
}

QString KIPI::ImageCollectionShared::category()
{
    kdWarning(51000) << "KIPI::ImageCollectionShared::category should only be invoked if the host application supports\n"
        "the KIPI::Features AlbumsHaveCategory - if the host application do support that, then this function should\n"
        "have been overridden in the host application.\n" << endl;
    return QString::null;
}

QDate KIPI::ImageCollectionShared::date()
{
    kdWarning(51000) << "KIPI::ImageCollectionShared::date should only be invoked if the host application supports\n"
        "the KIPI::Features AlbumsHaveCreationDate - if the host application do support that, then this function should\n"
        "have been overridden in the host application.\n" << endl;
    return QDate();
}

bool KIPI::ImageCollectionShared::operator==(ImageCollectionShared& ics) 
{
    return images()==ics.images();
}
