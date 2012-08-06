/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-19
 * @brief  image info shared
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2006-2012 by Marcel Wiesweg
 *         <a href="mailto:marcel dot wiesweg at gmx dot de">marcel dot wiesweg at gmx dot de</a>
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

#include "imageinfoshared.h"

// Qt includes

#include <QFileInfo>

// KDE includes

#include <kdebug.h>

// Local includes

#include "interface.h"

namespace KIPI
{

class ImageInfoShared::Private
{
public:

    Private()
    {
        count     = 1;
        interface = 0;
    }

    int        count;
    Interface* interface;
};

ImageInfoShared::ImageInfoShared()
    : d(new Private)
{
}

ImageInfoShared::ImageInfoShared(Interface* const interface, const KUrl& url)
    : d(new Private)
{
    d->interface = interface;
    _url         = url;
}

ImageInfoShared::~ImageInfoShared()
{
    delete d;
}

void ImageInfoShared::addRef()
{
    d->count++;
}

void ImageInfoShared::removeRef()
{
    d->count--;
    if ( d->count == 0 )
    {
        delete this;
    }
}

KUrl ImageInfoShared::url() const
{
    return _url;
}

void ImageInfoShared::cloneData(ImageInfoShared* const other)
{
    clearAttributes();
    addAttributes(other->attributes());
}

bool ImageInfoShared::reserveForAction(QObject* const reservingObject, const QString& descriptionOfAction) const
{
    return d->interface->reserveForAction(_url, reservingObject, descriptionOfAction);
}

void ImageInfoShared::clearReservation(QObject* const reservingObject)
{
    return d->interface->clearReservation(_url, reservingObject);
}

bool ImageInfoShared::itemIsReserved(QString* const descriptionOfAction) const
{
    return d->interface->itemIsReserved(_url, descriptionOfAction);
}

FileReadWriteLock* ImageInfoShared::createReadWriteLock() const
{
    return d->interface->createReadWriteLock(_url);
}

} // namespace KIPI
