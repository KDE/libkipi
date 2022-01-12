/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2006-2012 Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "imageinfoshared.h"

// Local includes

#include "interface.h"

namespace KIPI
{

class Q_DECL_HIDDEN ImageInfoShared::Private
{
public:

    Private()
    {
        count     = 1;
        interface = nullptr;
    }

    int        count;
    Interface* interface;
};

ImageInfoShared::ImageInfoShared()
    : d(new Private)
{
}

ImageInfoShared::ImageInfoShared(Interface* const interface, const QUrl &url)
    : d(new Private)
{
    d->interface = interface;
    _url         = url;
}

ImageInfoShared::~ImageInfoShared() = default;

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

QUrl ImageInfoShared::url() const
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
