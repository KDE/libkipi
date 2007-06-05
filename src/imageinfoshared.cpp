/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-19
 * Description : image info shared.
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

// Qt includes.

#include <QFileInfo>

// KDE includes.
  
#include <kdebug.h>

// Local includes.

#include "imageinfoshared.h"
#include "interface.h"

KIPI::ImageInfoShared::ImageInfoShared( Interface* interface, const KUrl& url )
    : _url( url ), _count(1), _interface( interface )
{
}

KUrl KIPI::ImageInfoShared::path()
{
    return _url;
}

int KIPI::ImageInfoShared::size()
{
    if ( ! _url.isLocalFile() ) 
    {
        kdFatal() << "KIPI::ImageInfoShared::size does not yet support non local files, please fix\n" << endl;
        return 0;
    }
    else
        return QFileInfo( _url.path() ).size();
}

QDateTime KIPI::ImageInfoShared::time( KIPI::TimeSpec )
{
    if ( ! _url.isLocalFile() ) 
    {
        kdFatal() << "KIPI::ImageInfoShared::time does not yet support non local files, please fix\n" << endl;
        return QDateTime();
    }
    else
        return QFileInfo( _url.path() ).lastModified();
}

void KIPI::ImageInfoShared::addRef()
{
    _count++;
}

void KIPI::ImageInfoShared::removeRef()
{
    _count--;
    if ( _count == 0 ) 
    {
        delete this;
    }
}

int KIPI::ImageInfoShared::angle()
{
    return 0;
}

void KIPI::ImageInfoShared::setAngle( int )
{
}

bool KIPI::ImageInfoShared::isTimeExact()
{
    return true;
}

void KIPI::ImageInfoShared::setTime( const QDateTime& /*time*/, TimeSpec /*spec*/ )
{
}

void KIPI::ImageInfoShared::setTitle( const QString& )
{
    kdWarning(51000) << "This method should only be invoked if the host application supports the KIPI::ImageTitlesWritable\n"
        "If the host application do support that, then this function should\n"
        "have been overridden in the host application.\n" << endl;
}

void KIPI::ImageInfoShared::cloneData( ImageInfoShared* other )
{
    if ( _interface->hasFeature( ImageTitlesWritable ) )
        setTitle( other->title() );

    if ( _interface->hasFeature( ImagesHasComments ) )
        setDescription( other->description() );

    clearAttributes();
    addAttributes( other->attributes() );

    setTime( other->time( FromInfo ), FromInfo );
    if ( _interface->hasFeature( SupportsDateRanges ) )
        setTime( other->time( ToInfo ), ToInfo );

    setAngle( other->angle() );
}
