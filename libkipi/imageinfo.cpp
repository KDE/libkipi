/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-07-22
 * @brief  image info
 *
 * @author Copyright (C) 2004-2013 by Gilles Caulier
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
    d = 0;
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

KUrl ImageInfo::url() const
{
    return d->url();
}

} // namespace KIPI
