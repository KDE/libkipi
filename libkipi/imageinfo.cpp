/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-07-22
 * Description : image info.
 *
 * Copyright (C) 2004-2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
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
    return _data->title();
}

/**
   PENDING(blackie) document
*/
KUrl ImageInfo::path() const
{
    return _data->path();
}

/**
   PENDING(blackie) document
*/
QString ImageInfo::description() const
{
    return _data->description();
}

/**
   Returns the time of the image.
   In case the host application supports time range, the spec argument
   specifies if it is the start or end time that should be returned.
*/
QDateTime ImageInfo::time( TimeSpec spec ) const
{
    return _data->time( spec );
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
    return _data->attributes();
}

/**
   PENDING(blackie) document
*/
int ImageInfo::size() const
{
    return _data->size();
}

ImageInfo::ImageInfo( ImageInfoShared* shared )
         : _data( shared )
{
}

ImageInfo::ImageInfo( const ImageInfo& rhs )
{
    _data = rhs._data;
    _data->addRef();
}

ImageInfo::~ImageInfo()
{
    _data->removeRef();
}

void ImageInfo::setTitle( const QString& name )
{
    _data->setTitle( name );
}

void ImageInfo::setDescription( const QString& description )
{
    _data->setDescription( description );
}

/** Remove all attribute from the image. See delAttributes() for list of all attributes removed.
*/
void ImageInfo::clearAttributes()
{
    _data->clearAttributes();
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
    _data->addAttributes( attributes );
}

/** Remove attributes listed from the image. Following values can be used:
    "tags"        : remove all tags.
    "rating"      : remove rating.
    "gpslocation" : remove latitude, longitude, and altitude values.
*/
void ImageInfo::delAttributes( const QStringList& attributes )
{
    _data->delAttributes( attributes );
}

/**
   Returns the angle the application rotates the image with when displaying it.
   Certain host applications may choose to rotate the image on disk, and will always return 0,
   while other host application will rotate the image when displaying it, and will thus not rotate
   the image on disk.
*/
int ImageInfo::angle() const
{
    return _data->angle();
}

/**
   See \ref angle
*/
void ImageInfo::setAngle( int angle )
{
    _data->setAngle( angle );
}

/**
   In the case the application supports time ranges (like this image is
   from 1998-2000), this method will return true if the time is an exact
   specification, and thus not a range.
*/
bool ImageInfo::isTimeExact() const
{
    return _data->isTimeExact();
}

void ImageInfo::setTime( const QDateTime& time, TimeSpec spec )
{
    _data->setTime( time, spec );
}

/**
   Copies all the attibutes, description etc from the other imageinfo
*/
void ImageInfo::cloneData( const ImageInfo& other )
{
    _data->cloneData( other._data );
}

} // namespace KIPI
