/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  image collection shared
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Jesper K. Pedersen
 *         <a href="mailto:blackie at kde dot org">blackie at kde dot org</a>
 * @author Copyright (C) 2004-2005 by Aurelien Gateau
 *         <a href="mailto:aurelien dot gateau at free dot fr">aurelien dot gateau at free dot fr</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "imagecollectionshared.h"

// KDE Includes

#include <kdebug.h>

// Local includes

#include "imagecollection.h"

// Macros

#define PrintWarningMessageFeature(feature)                                           \
        kWarning() << "This should only be invoked if the host application supports " \
                      "KIPI::Features (" << feature << "). If host application do "   \
                      "support that, then this function should have been overridden " \
                      "in the KIPI host interface."

namespace KIPI
{

ImageCollectionShared::ImageCollectionShared()
    : m_count(1)
{
}

ImageCollectionShared::~ImageCollectionShared()
{
}

void ImageCollectionShared::addRef()
{
    m_count++;
}

void ImageCollectionShared::removeRef()
{
    m_count--;

    if ( m_count == 0 )
    {
        // kDebug() << "Deleting!" << endl;
        delete this;
    }
}

KUrl ImageCollectionShared::path()
{
    kWarning() << "This method should only be invoked if this imagecollection is a directory. "
               << "See KIPI::ImageCollectionShared::isDirectory()";
    return KUrl();
}

KUrl ImageCollectionShared::uploadPath()
{
    PrintWarningMessageFeature("AcceptNewImages");
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
    {
        return KUrl( "file:/" );
    }
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
    PrintWarningMessageFeature("AlbumsHaveComments");
    return QString();
}

QString ImageCollectionShared::category()
{
    PrintWarningMessageFeature("AlbumsHaveCategory");
    return QString();
}

QDate ImageCollectionShared::date()
{
    PrintWarningMessageFeature("AlbumsHaveCreationDate");
    return QDate();
}

bool ImageCollectionShared::operator==(ImageCollectionShared& ics)
{
    return images() == ics.images();
}

} // namespace KIPI
