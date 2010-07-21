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

/** @file imageinfo.h */

#ifndef KIPI_IMAGEINFO_H
#define KIPI_IMAGEINFO_H

// Qt includes.

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QVariant>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "libkipi_export.h"

namespace KIPI
{

class ImageInfoShared;

enum TimeSpec
{
    FromInfo,
    ToInfo
};

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
    void delAttributes( const QStringList& );

    KUrl path() const;
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
    ImageInfo( ImageInfoShared* const );
    ImageInfo( const ImageInfo& );
    ~ImageInfo();

private:

    ImageInfo() {} // Disabled
    ImageInfo& operator=( const ImageInfo& ) { return *this; } // Disabled

private:

    mutable KIPI::ImageInfoShared* d;
};

} // namespace KIPI

#endif /* KIPI_IMAGEINFO_H */

