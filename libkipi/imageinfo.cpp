/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-07-22
 * @brief  image info
 *
 * @author Copyright (C) 2004-2011 by Gilles Caulier
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

/** @file imageinfo.cpp
*/

// Local includes

#include "imageinfo.h"
#include "imageinfoshared.h"

namespace KIPI
{

QString ImageInfo::toString( const QVariant& data ) const
{
    QString string = data.toString();
    return string;
}

QString ImageInfo::name() const
{
    return d->name();
}

void ImageInfo::setName( const QString& name )
{
    d->setName( name );
}

KUrl ImageInfo::path() const
{
    return d->path();
}

QString ImageInfo::description() const
{
    return d->description();
}

QDateTime ImageInfo::time( TimeSpec spec ) const
{
    return d->time( spec );
}

int ImageInfo::size() const
{
    return d->size();
}

ImageInfo::ImageInfo( ImageInfoShared* const shared )
         : d( shared )
{
}

ImageInfo::ImageInfo( const ImageInfo& rhs )
{
    d = rhs.d;
    d->addRef();
}

ImageInfo::~ImageInfo()
{
    d->removeRef();
}

void ImageInfo::setDescription( const QString& description )
{
    d->setDescription( description );
}

QMap<QString,QVariant> ImageInfo::attributes() const
{
    return d->attributes();
}

void ImageInfo::addAttributes( const QMap<QString,QVariant>& attributes )
{
    d->addAttributes( attributes );
}

void ImageInfo::delAttributes( const QStringList& attributes )
{
    d->delAttributes( attributes );
}

void ImageInfo::clearAttributes()
{
    d->clearAttributes();
}

int ImageInfo::angle() const
{
    return d->angle();
}

void ImageInfo::setAngle( int angle )
{
    d->setAngle( angle );
}

bool ImageInfo::isTimeExact() const
{
    return d->isTimeExact();
}

void ImageInfo::setTime( const QDateTime& time, TimeSpec spec )
{
    d->setTime( time, spec );
}

void ImageInfo::cloneData( const ImageInfo& other )
{
    d->cloneData( other.d );
}

// Deprecated methods.

QString ImageInfo::title() const
{
    return d->name();
}

void ImageInfo::setTitle( const QString& name )
{
    d->setName( name );
}

} // namespace KIPI
