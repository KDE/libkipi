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

/** @file imageinfo.h */

#ifndef KIPI_IMAGEINFO_H
#define KIPI_IMAGEINFO_H

// Qt includes.

#include <qstring.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qvariant.h>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "libkipi/libkipi_export.h"

namespace KIPI
{
    class ImageInfoShared;
    enum TimeSpec { FromInfo, ToInfo };

    /** @class ImageInfo 
        Holds info about an image 
      */
    class LIBKIPI_EXPORT ImageInfo
    {

    public:

        QString title() const;
        void setTitle( const QString& name );

        QString description() const;
        void setDescription( const QString& description);

        QMap<QString,QVariant> attributes() const;
        void clearAttributes();
        void addAttributes( const QMap<QString,QVariant>& );

        KURL path() const;
        QDateTime time( TimeSpec spec = FromInfo ) const;
        void setTime( const QDateTime& time, TimeSpec spec = FromInfo );
        bool isTimeExact() const;
        int size() const;

        int angle() const;
        void setAngle( int );

        QString toString( const QVariant& ) const;

        void cloneData( const ImageInfo& other );

        /** constructor
            Interface for host application + general stuff
          */
        ImageInfo( ImageInfoShared* );
        ImageInfo( const ImageInfo& );
        ~ImageInfo();

    private:

        ImageInfo() {} // Disabled
        ImageInfo& operator=( const ImageInfo& ) { return *this; } // Disabled
        mutable KIPI::ImageInfoShared* _data;
    };
}

#endif /* KIPI_IMAGEINFO_H */

