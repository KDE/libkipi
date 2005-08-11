/* ============================================================
 * File  : imageinfo.h
 * Author: KIPI team developers (see AUTHORS files for details)
 * Date  : 2004-07-22
 * Description : 
 * 
 * Copyright 2004 by the KIPI team
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * ============================================================ */

/** @file imageinfo.h */

#ifndef KIPI_IMAGEINFO_H
#define KIPI_IMAGEINFO_H
#include <kurl.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qmap.h>
#include <qvariant.h>
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

