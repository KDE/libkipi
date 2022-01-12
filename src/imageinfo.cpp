/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// Local includes

#include "imageinfo.h"
#include "imageinfoshared.h"

namespace KIPI
{

ImageInfo::ImageInfo(ImageInfoShared* const shared)
    : d(shared)
{
}

ImageInfo::ImageInfo(const ImageInfo& rhs)
{
    d = rhs.d;
    d->addRef();
}

ImageInfo::ImageInfo()
{
    d = nullptr;
}

ImageInfo::~ImageInfo()
{
    d->removeRef();
}

ImageInfo& ImageInfo::operator=(const ImageInfo&)
{
    return *this;
}

QMap<QString,QVariant> ImageInfo::attributes() const
{
    return d->attributes();
}

void ImageInfo::addAttributes(const QMap<QString,QVariant>& attributes)
{
    d->addAttributes( attributes );
}

void ImageInfo::delAttributes(const QStringList& attributes)
{
    d->delAttributes(attributes);
}

void ImageInfo::clearAttributes()
{
    d->clearAttributes();
}

void ImageInfo::cloneData(const ImageInfo& other)
{
    d->cloneData(other.d);
}

QUrl ImageInfo::url() const
{
    return d->url();
}

} // namespace KIPI
