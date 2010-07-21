/** ===========================================================
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-07-22
 * @brief  image info
 *
 * @author Copyright (C) 2004-2010 by Gilles Caulier
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

// Local includes.

#include "imageinfo.h"
#include "imageinfoshared.h"

namespace KIPI
{

QString ImageInfo::toString( const QVariant& data ) const
{
    QString string = data.toString();
    return string;
}

/**
   PENDING(blackie) document
*/
QString ImageInfo::title() const
{
    return d->title();
}

/**
   PENDING(blackie) document
*/
KUrl ImageInfo::path() const
{
    return d->path();
}

/**
   PENDING(blackie) document
*/
QString ImageInfo::description() const
{
    return d->description();
}

/**
   Returns the time of the image.
   In case the host application supports time range, the spec argument
   specifies if it is the start or end time that should be returned.
*/
QDateTime ImageInfo::time( TimeSpec spec ) const
{
    return d->time( spec );
}

/**
    Returns a Map of attributes of the image
    In case the host application supports some special attributes of the image
    this function can be used to return them. following attribes are supported by this feature:

    QString("tagspath")  :: QStringList() with tags path list formated as "Country/France/City/Paris" for ex.
    QString("tags")      :: QStringList() with tags name list.
    QString("rating")    :: integer value (ususally 0 <= rate <= 5).
    QString("latitude")  :: double value in degrees (-90.0 >= lat <=90.0).
    QString("longitude") :: double value in degrees (-180.0 >= lat <=180.0).
    QString("altitude")  :: double value in meters.
*/
QMap<QString,QVariant> ImageInfo::attributes() const
{
    return d->attributes();
}

/**
   PENDING(blackie) document
*/
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

void ImageInfo::setTitle( const QString& name )
{
    d->setTitle( name );
}

void ImageInfo::setDescription( const QString& description )
{
    d->setDescription( description );
}

/** Remove all attribute from the image. See delAttributes() for list of all attributes removed.
*/
void ImageInfo::clearAttributes()
{
    d->clearAttributes();
}

/** Set the attributes defined from the map to the image. Following keys/values can be used:
    QString("tagspath")  :: QStringList() with tags path list formated as "Country/France/City/Paris" for ex.
    QString("rating")    :: integer value (ususally 0 <= rate <= 5).
    QString("latitude")  :: double value in degrees (-90.0 >= lat <=90.0).
    QString("longitude") :: double value in degrees (-180.0 >= lat <=180.0).
    QString("altitude")  :: double value in meters.
*/
void ImageInfo::addAttributes( const QMap<QString,QVariant>& attributes )
{
    d->addAttributes( attributes );
}

/** Remove attributes listed from the image. Following values can be used:
    "tags"        : remove all tags.
    "rating"      : remove rating.
    "gpslocation" : remove latitude, longitude, and altitude values.
*/
void ImageInfo::delAttributes( const QStringList& attributes )
{
    d->delAttributes( attributes );
}

/**
   Returns the angle the application rotates the image with when displaying it.
   Certain host applications may choose to rotate the image on disk, and will always return 0,
   while other host application will rotate the image when displaying it, and will thus not rotate
   the image on disk.
*/
int ImageInfo::angle() const
{
    return d->angle();
}

/**
   See \ref angle
*/
void ImageInfo::setAngle( int angle )
{
    d->setAngle( angle );
}

/**
   In the case the application supports time ranges (like this image is
   from 1998-2000), this method will return true if the time is an exact
   specification, and thus not a range.
*/
bool ImageInfo::isTimeExact() const
{
    return d->isTimeExact();
}

void ImageInfo::setTime( const QDateTime& time, TimeSpec spec )
{
    d->setTime( time, spec );
}

/**
   Copies all the attibutes, description etc from the other imageinfo
*/
void ImageInfo::cloneData( const ImageInfo& other )
{
    d->cloneData( other.d );
}

} // namespace KIPI
