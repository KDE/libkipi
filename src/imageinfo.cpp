/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-07-22
 * Description : image info.
 *
 * Copyright (C) 2004-2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

// Local includes.

#include "imageinfo.h"
#include "imageinfoshared.h"

/** @file imageinfo.cpp
*/
QString KIPI::ImageInfo::toString( const QVariant& /*data*/ ) const
{
    return QString();
}

/**
   PENDING(blackie) document
*/
QString KIPI::ImageInfo::title() const
{
    return _data->title();
}

/**
   PENDING(blackie) document
*/
KUrl KIPI::ImageInfo::path() const
{
    return _data->path();
}

/**
   PENDING(blackie) document
*/
QString KIPI::ImageInfo::description() const
{
    return _data->description();
}

/**
   Returns the time of the image.
   In case the host application supports time range, the spec argument
   specifies if it is the start or end time that should be returned.
*/
QDateTime KIPI::ImageInfo::time( TimeSpec spec ) const
{
    return _data->time( spec );
}

/**
   Returns a Map of attributes of the image
   In case the host application supports some special attributes of the image
   this function can be used to return them.
   Tags are supported by this feature: "tags" key contains QStringList() 
   encapsulated in a QVariant
   
*/
QMap<QString,QVariant> KIPI::ImageInfo::attributes() const
{
    return _data->attributes();
}

/**
   PENDING(blackie) document
*/
int KIPI::ImageInfo::size() const
{
    return _data->size();
}

KIPI::ImageInfo::ImageInfo( ImageInfoShared* shared )
    : _data( shared )
{
}

KIPI::ImageInfo::ImageInfo( const KIPI::ImageInfo& rhs )
{
    _data = rhs._data;
    _data->addRef();
}

KIPI::ImageInfo::~ImageInfo()
{
    _data->removeRef();
}

void KIPI::ImageInfo::setTitle( const QString& name )
{
    _data->setTitle( name );
}

void KIPI::ImageInfo::setDescription( const QString& description )
{
    _data->setDescription( description );
}

void KIPI::ImageInfo::clearAttributes()
{
    _data->clearAttributes();
}

void KIPI::ImageInfo::addAttributes( const QMap<QString,QVariant>& attributes )
{
    _data->addAttributes( attributes );
}

/**
   Returns the angle the application rotates the image with when displaying it.
   Certain host applications may choose to rotate the image on disk, and will always return 0,
   while other host application will rotate the image when displaying it, and will thus not rotate
   the image on disk.
*/
int KIPI::ImageInfo::angle() const
{
    return _data->angle();
}

/**
   See \ref angle
*/
void KIPI::ImageInfo::setAngle( int angle )
{
    _data->setAngle( angle );
}

/**
   In the case the application supports time ranges (like this image is
   from 1998-2000), this method will return true if the time is an exact
   specification, and thus not a range.
*/
bool KIPI::ImageInfo::isTimeExact() const
{
    return _data->isTimeExact();
}

void KIPI::ImageInfo::setTime( const QDateTime& time, TimeSpec spec )
{
    _data->setTime( time, spec );
}

/**
   Copies all the attibutes, description etc from the other imageinfo
*/
void KIPI::ImageInfo::cloneData( const ImageInfo& other )
{
    _data->cloneData( other._data );
}
