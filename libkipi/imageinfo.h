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
    ImageInfo(ImageInfoShared* const);
    ImageInfo(const ImageInfo&);
    ~ImageInfo();

    /**
        Returns a Map of attributes of the image
        In case the host application supports some special attributes of the image
        this function can be used to return them. Following attributes are supported by these features:

        ATTRIBUTES    :: VALUE RETURNED  :: COMMENTS
        ----------------------------------------------------------------------------------------------------------------------------
        "name"        :: QString         :: name of item (usually file name).
        "comment"     :: QString         :: default item comment.
        "date"        :: QDateTime       :: date of item (usually the creation date).
                                            In the case the application supports date ranges (like this image is from 1998-2000),
                                            this attribute always return the start of date range.
        "dateto"      :: QDateTime       :: in the case the application supports date ranges (like this image is from 1998-2000),
                                            this attribute return the end of date range.
        "isexactdate" :: bool value      :: in the case the application supports date ranges (like this image is from 1998-2000),
                                            this attribute will be true if the date is an exact specification, and thus not a range.
        "orientation" :: integer value   :: Orientation information. See KExiv2::ImageOrientation value for details).
        "title"       :: QString         :: default item title.
        "rating"      :: integer value   :: item starts 0 <= rate <= 5).
        "colorlabel"  :: integer value   :: item color flag 0 <= colorlabel <= 10.
        "picklabel"   :: integer value   :: item workflow flag 0 <= picklabel <= 4.
        "latitude"    :: double value    :: latitude in degrees (-90.0 >= lat <=90.0).
        "longitude"   :: double value    :: longitude in degrees (-180.0 >= long <=180.0).
        "altitude"    :: double value    :: altitude in meters.
        "tagspath"    :: QStringList     :: all tags path list formated as "Country/France/City/Paris" for ex.
        "keywords"    :: QStringList     :: all tags name list (without path).
        "filesize"    :: qlonglong value :: file size in bytes.

        "tags"        :: QStringList     :: DEPRECATED: for compatibility. Deprecated and replaced by "keywords" attribute.
        "angle"       :: integer value   :: DEPRECATED: for compatibility. Deprecated and replaced by "orientation" attribute.
    */
    QMap<QString, QVariant> attributes() const;

    /** Set the attributes defined from the map to the image. Following keys/values are the same the attributes(),
     *  excepted "keywords", "filesize", and "isexactdate" properties which are read-only values.
    */
    void addAttributes(const QMap<QString, QVariant>&);

    /** Remove attributes listed from the image. Following values can be used:
        "comment"     :: Remove all comments.
        "date"        :: Remove date info.
        "orientation" :: Remove orientation info.
        "title"       :: Remove all titles.
        "tagspath"    :: Remove all tags path.
        "rating"      :: Remove rating info.
        "colorlabel"  :: Remove color labels info.
        "picklabel"   :: Remove pick labels info.
        "gpslocation" :: Remove latitude, longitude, and altitude values.

        "tags"        :: DEPRECATED: for compatibility. Deprecated and replaced by "tagspath" attribute.
        "angle"       :: DEPRECATED: for compatibility. Deprecated and replaced by "orientation" attribute.
    */
    void delAttributes(const QStringList&);

    /** Remove all attribute from the image. See delAttributes() for list of all attributes removed.
     */
    void clearAttributes();

    /** Copies all the attributes from the other imageinfo
    */
    void cloneData(const ImageInfo& other);

    /** Lock item for processing, to prevent parallel operations from KIPI host and plugins.
     *  Default implementation do nothing.
     */
    void lock();
    void unlock();
    bool isLocked();

    // -------------------------------------------------------------------------------------------------------
    // DEPRECATED METHODS. Do not use it, they will be removed in the future...
    //
    // More universal methods based on attributes must be used instead to extend more easily data exange between 
    // kipi-plugins and kipi host without to break binary compatibility.

    /** Managed by attribute "name".
     */
    KDE_DEPRECATED QString name() const;
    KDE_DEPRECATED void    setName(const QString& name);

    /** Managed by attribute "date" and "dateto".
     */
    KDE_DEPRECATED QDateTime time(TimeSpec spec = FromInfo) const;
    KDE_DEPRECATED void      setTime(const QDateTime& time, TimeSpec spec = FromInfo);

    /** Managed by attribute "isexactdate".
     */
    KDE_DEPRECATED bool isTimeExact() const;

    /** Managed by attributes "comment"
     */
    KDE_DEPRECATED QString description() const;
    KDE_DEPRECATED void    setDescription(const QString& description);

    /** Managed by attribute "angle"
    */
    KDE_DEPRECATED int  angle() const;
    KDE_DEPRECATED void setAngle(int);

    /** In plugins use KIPIPlugins::KPImageInfo::url() instead.
     */
    KDE_DEPRECATED KUrl path() const;

    /** Managed by attribute "filesize".
    */
    KDE_DEPRECATED int size() const;

private:

    ImageInfo() {}                                              // Disable
    ImageInfo& operator=( const ImageInfo& ) { return *this; }  // Disable

private:

    mutable KIPI::ImageInfoShared* d;
};

} // namespace KIPI

#endif /* KIPI_IMAGEINFO_H */
