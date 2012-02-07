/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-07-22
 * @brief  image info
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
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

// Qt includes

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QVariant>

// KDE includes

#include <kurl.h>
#include <kdemacros.h>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

class ImageInfoShared;

// Deprecated data used by time() and setTime()
enum KDE_DEPRECATED TimeSpec
{
    FromInfo,
    ToInfo
};

/** @class ImageInfo
    Holds info about an image
    NOTE: If you want to manage item properties from kipi-plugins, use wrapper class KIPIPlugins::KPImageInfo,
    not this class directly.
 */
class LIBKIPI_EXPORT ImageInfo
{

public:

    /** constructor
        Interface for host application + general stuff
     */
    ImageInfo( ImageInfoShared* const );
    ImageInfo( const ImageInfo& );
    ~ImageInfo();

    QString name() const;
    void setName( const QString& name );

    /**
        Returns a Map of attributes of the image
        In case the host application supports some special attributes of the image
        this function can be used to return them. Following attributes are supported by these features:

        QString("comment")     :: QString() with default comment [same than description()].
        QString("date")        :: QDateTime() with date  [same than time()].
        QString("orientation") :: integer value (orientation information. See KExiv2::ImageOrientation value for details).
        QString("title")       :: QString() with default title.
        QString("rating")      :: integer value (ususally 0 <= rate <= 5).
        QString("colorlabel")  :: integer value (ususally 0 <= colorlabel <= 9).
        QString("picklabel")   :: integer value (ususally 0 <= picklabel <= 4).
        QString("latitude")    :: double value in degrees (-90.0 >= lat <=90.0).
        QString("longitude")   :: double value in degrees (-180.0 >= long <=180.0).
        QString("altitude")    :: double value in meters.
        QString("tagspath")    :: QStringList() with tags path list formated as "Country/France/City/Paris" for ex.
        QString("keywords")    :: QStringList() with tags name list.
        QString("filesize")    :: qlonglong value in bytes.

        QString("tags")        :: DEPRECATED: for compatibility. Deprecated and replaced by "keywords" attribute.
        QString("angle")       :: DEPRECATED: for compatibility. Deprecated and replaced by "orientation" attribute.
    */
    QMap<QString, QVariant> attributes() const;

    /** Set the attributes defined from the map to the image. Following keys/values are the same the attributes(),
     *  excepted "tags" property which is read-only.
    */
    void addAttributes(const QMap<QString, QVariant>&);

    /** Remove attributes listed from the image. Following values can be used:
        QString("comment")     :: Remove all comments.
        QString("date")        :: Remove date info.
        QString("orientation") :: Remove orientation info.
        QString("title")       :: Remove all titles.
        QString("tagspath")    :: Remove all tags path.
        QString("rating")      :: Remove rating info.
        QString("colorlabel")  :: Remove color labels info.
        QString("picklabel")   :: Remove pick labels info.
        QString("gpslocation") :: Remove latitude, longitude, and altitude values.

        QString("tags")        :: DEPRECATED: for compatibility. Deprecated and replaced by "tagspath" attribute.
        QString("angle")       :: DEPRECATED: for compatibility. Deprecated and replaced by "orientation" attribute.
    */
    void delAttributes( const QStringList& );

    /** Remove all attribute from the image. See delAttributes() for list of all attributes removed.
     */
    void clearAttributes();

    /**
        Copies all the attributes from the other imageinfo
    */
    void cloneData( const ImageInfo& other );

    // -------------------------------------------------------------------------------------------------------
    // NOTE: Deprecated Methods. Do not use it, they will be removed in the future...
    //
    // More universal methods based on attributes must be used instead to extend more easily data exange between 
    // kipi-plugins and kipi host without to break binary compatibility.

    /**
        Replaced by attributes(QString("date"))
     */
    KDE_DEPRECATED QDateTime time( TimeSpec spec = FromInfo ) const;

    /**
        Replaced by addAttributes(QMap < QString("date"), QDateTime(...) >)
     */
    KDE_DEPRECATED void setTime( const QDateTime& time, TimeSpec spec = FromInfo );

    /**
        This method is deprecated because it have never used by kipi-plugins.
     */
    KDE_DEPRECATED bool isTimeExact() const;

    /** replaced by attributes(QString("comment"))
     */
    KDE_DEPRECATED QString description() const;

    /** replaced by addAttributes(QMap < QString("comment"), QString(...) >)
     */
    KDE_DEPRECATED void setDescription( const QString& description);

    /**
        replaced by attributes(QString("angle"))
    */
    KDE_DEPRECATED int angle() const;

    /**
        replaced by addAttributes(QMap < QString("angle"), int >)
    */
    KDE_DEPRECATED void setAngle(int);

    /** In plugins use KIPIPlugins::KPImageInfo::url() instead.
     */
    KDE_DEPRECATED KUrl path() const;

    /**
        replaced by attributes(QString("filesize"))
    */
    KDE_DEPRECATED int size() const;

private:

    // Disabled operator and constructor.
    ImageInfo() {}
    ImageInfo& operator=( const ImageInfo& ) { return *this; }

private:

    mutable KIPI::ImageInfoShared* d;
};

} // namespace KIPI

#endif /* KIPI_IMAGEINFO_H */
