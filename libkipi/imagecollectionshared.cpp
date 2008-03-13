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

// KDE Includes.

#include <kdebug.h>

// Local Includes.

#include "imagecollectionshared.h"

namespace KIPI
{

ImageCollectionShared::ImageCollectionShared()
                     : _count(1)
{
}

void ImageCollectionShared::addRef()
{
    _count++;
}

void ImageCollectionShared::removeRef()
{
    _count--;
    if ( _count == 0 ) 
    {
        // qDebug("Deleting!");
        delete this;
    }
}

KUrl ImageCollectionShared::path()
{
    kWarning(51000) << "This method should only be invoked if this imagecollection is a directory.\n"
                     <<  "See KIPI::ImageCollectionShared::isDirectory()"
                     << endl;
    return KUrl();
}

KUrl ImageCollectionShared::uploadPath()
{
    kWarning(51000) << "This method should only be invoked if the host application supports the KIPI::Features\n"
            "AcceptNewImages - if the host application do support that, then this function should\n"
            "have been overridden in the host application.\n" << endl;
    return KUrl();
}

KUrl ImageCollectionShared::uploadRoot()
{
    KUrl path = uploadPath();
    if ( path.isValid() ) 
    {
        path.setPath("/");
        return path;
    }
    else
        return KUrl( "file:/" );
}

QString ImageCollectionShared::uploadRootName()
{
   return (QString("Images"));   // No i18n here. THis must be done on KIPI host interface.
}

bool ImageCollectionShared::isDirectory()
{
    return false;
}

QString ImageCollectionShared::comment()
{
    kWarning(51000) << "KIPI::ImageCollectionShared::comment should only be invoked if the host application supports\n"
        "the KIPI::Features AlbumsHaveComments - if the host application do support that, then this function should\n"
        "have been overridden in the host application.\n" << endl;
    return QString();
}

QString ImageCollectionShared::category()
{
    kWarning(51000) << "KIPI::ImageCollectionShared::category should only be invoked if the host application supports\n"
        "the KIPI::Features AlbumsHaveCategory - if the host application do support that, then this function should\n"
        "have been overridden in the host application.\n" << endl;
    return QString();
}

QDate ImageCollectionShared::date()
{
    kWarning(51000) << "KIPI::ImageCollectionShared::date should only be invoked if the host application supports\n"
        "the KIPI::Features AlbumsHaveCreationDate - if the host application do support that, then this function should\n"
        "have been overridden in the host application.\n" << endl;
    return QDate();
}

bool ImageCollectionShared::operator==(ImageCollectionShared& ics) 
{
    return images()==ics.images();
}

} // namespace KIPI
