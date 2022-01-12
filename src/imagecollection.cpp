/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
        d = nullptr;
    }
}

ImageCollection::ImageCollection()
{
    d = nullptr;
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
        d = nullptr;
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
        return QString();
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
        return QString();
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
    return (d != nullptr);
}

} // namespace KIPI
