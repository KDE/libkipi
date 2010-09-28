/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-19
 * @brief  image info shared
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

#include "imageinfoshared.h"

// Qt includes.

#include <QFileInfo>

// KDE includes.

#include <kdebug.h>

// Local includes.

#include "interface.h"

namespace KIPI
{

ImageInfoShared::ImageInfoShared( Interface* const interface, const KUrl& url )
               : _url( url ), m_count(1), m_interface( interface )
{
}

KUrl ImageInfoShared::path()
{
    return _url;
}

int ImageInfoShared::size()
{
    if ( ! _url.isLocalFile() )
    {
        kFatal() << "KIPI::ImageInfoShared::size does not yet support non local files, please fix\n";
        return 0;
    }
    else
    {
        return QFileInfo( _url.toLocalFile() ).size();
    }
}

QDateTime ImageInfoShared::time( TimeSpec )
{
    if ( ! _url.isLocalFile() )
    {
        kFatal() << "KIPI::ImageInfoShared::time does not yet support non local files, please fix\n";
        return QDateTime();
    }
    else
    {
        return QFileInfo( _url.toLocalFile() ).lastModified();
    }
}

void ImageInfoShared::addRef()
{
    m_count++;
}

void ImageInfoShared::removeRef()
{
    m_count--;
    if ( m_count == 0 )
    {
        delete this;
    }
}

int ImageInfoShared::angle()
{
    return 0;
}

void ImageInfoShared::setAngle( int )
{
}

bool ImageInfoShared::isTimeExact()
{
    return true;
}

void ImageInfoShared::setTime( const QDateTime& /*time*/, TimeSpec /*spec*/ )
{
}

void ImageInfoShared::setTitle( const QString& )
{
    kWarning() << "This method should only be invoked if the host application "
                  "supports the KIPI::ImagesHasTitlesWritable\n"
                  "If the host application do support that, then this function should\n"
                  "have been overridden in the host application.";
}

void ImageInfoShared::cloneData( ImageInfoShared* const other )
{
    if ( m_interface->hasFeature( ImagesHasTitlesWritable ) )
        setTitle( other->title() );

    if ( m_interface->hasFeature( ImagesHasComments ) )
        setDescription( other->description() );

    clearAttributes();
    addAttributes( other->attributes() );

    setTime( other->time( FromInfo ), FromInfo );
    if ( m_interface->hasFeature( HostSupportsDateRanges ) )
        setTime( other->time( ToInfo ), ToInfo );

    setAngle( other->angle() );
}

} // namespace KIPI
