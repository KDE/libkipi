/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  image collection
 *
 * @author Copyright (C) 2004-2016 by Gilles Caulier
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

// Local includes

#include "imagecollection.h"
#include "imagecollectionshared.h"
#include "libkipi_debug.h"

// Macros

#define PrintWarningMessage()                                           \
        qCWarning(LIBKIPI_LOG) << "Image collection is invalid - this might be the case if you asked for an album, " \
                               << "and not album existed. You should check using .isValid() first. "                 \
                               << "Note: Plugins should never create an instance of ImageCollection, only the "      \
                               << "host application should do that."

namespace KIPI
{

ImageCollection::ImageCollection(ImageCollectionShared* const data)
    : d(data)
{
}

ImageCollection::ImageCollection(const ImageCollection& rhs)
{
    if ( rhs.d )
    {
        d = rhs.d;
        d->addRef();
    }
    else
    {
        d = 0;
    }
}

ImageCollection::ImageCollection()
{
    d = 0;
}

ImageCollection::~ImageCollection()
{
    if ( d )
        d->removeRef();
}

ImageCollection& ImageCollection::operator=(const ImageCollection& rhs)
{
    if ( rhs.d == d )
        return *this;

    if ( d )
        d->removeRef();

    if ( !rhs.d )
    {
        PrintWarningMessage();
        d = 0;
    }
    else
    {
        d = rhs.d;
        d->addRef();
    }
    return *this;
}

bool ImageCollection::operator==(const ImageCollection& ic) const
{
    if (!d || !(ic.d))
    {
        PrintWarningMessage();
        return false;
    }
    return *d == *(ic.d);
}

QString ImageCollection::comment() const
{
    if ( d )
    {
        return d->comment();
    }
    else
    {
        PrintWarningMessage();
        return QString::null;
    }
}

QString ImageCollection::name() const
{
    if ( d )
    {
        return d->name();
    }
    else
    {
        PrintWarningMessage();
        return QString();
    }
}

QString ImageCollection::category() const
{
    if ( d )
    {
        return d->category();
    }
    else
    {
        PrintWarningMessage();
        return QString::null;
    }
}

QDate ImageCollection::date() const
{
    if ( d )
    {
        return d->date();
    }
    else
    {
        PrintWarningMessage();
        return QDate();
    }
}

QList<QUrl> ImageCollection::images() const
{
    if ( d )
    {
        return d->images();
    }
    else
    {
        PrintWarningMessage();
        return QList<QUrl>();
    }
}

QUrl ImageCollection::url() const
{
    if ( d )
    {
        return d->url();
    }
    else
    {
        PrintWarningMessage();
        return QUrl();
    }
}

QUrl ImageCollection::uploadUrl() const
{
    if ( d )
    {
        return d->uploadUrl();
    }
    else
    {
        PrintWarningMessage();
        return QUrl();
    }
}

QUrl ImageCollection::uploadRootUrl() const
{
    if ( d )
    {
        return d->uploadRootUrl();
    }
    else
    {
        PrintWarningMessage();
        return QUrl();
    }
}

QString ImageCollection::uploadRootName() const
{
    if ( d )
    {
        return d->uploadRootName();
    }
    else
    {
        PrintWarningMessage();
        return QString();
    }
}

bool ImageCollection::isDirectory() const
{
    if ( d )
    {
        return d->isDirectory();
    }
    else
    {
        PrintWarningMessage();
        return false;
    }
}

bool ImageCollection::isValid() const
{
    return (d != 0);
}

} // namespace KIPI
