/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPI_IMAGEINFO_H
#define KIPI_IMAGEINFO_H

// Qt includes

#include <QString>
#include <QDateTime>
#include <QMap>
#include <QVariant>
#include <QUrl>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

class ImageInfoShared;

/** @class ImageInfo imageinfo.h <KIPI/ImageInfo>
    Holds info about an image from KIPI host application.

    @note If you want to manage item properties from kipi-plugins, use wrapper class KIPIPlugins::KPImageInfo,
    not this class directly. See kipi-plugins/common/libkipi-plugins API for details.

    @note This class must be re-implemented in KIPI host application through KIPI::ImageInfoShared. This implementation
    must be thread-safe, due to possible use into kipi plugins separated thread implementation in the future.
 */
class LIBKIPI_EXPORT ImageInfo
{

public:

    /** Constructor and interface for host application.
     */
    ImageInfo(ImageInfoShared* const);
    ImageInfo(const ImageInfo&);
    ~ImageInfo();

    /**
        Returns a Map of attributes of the image.
        In case the host application supports some special attributes of the image,
        this function can be used to return them. Following attributes are supported:

      <table><tr>
            <th>ATTRIBUTES</th>
            <th>VALUE RETURNED</th>
            <th>COMMENTS</th>
        </tr><tr>
            <th>"name"</th>
            <td>QString</td>
            <td>Returns name of item (usually file name).</td>
        </tr><tr>
            <th>"comment"</th>
            <td>QString</td>
            <td>Returns default item comment.</td>
        </tr><tr>
            <th>"date"</th>
            <td>QDateTime</td>
            <td>Returns date of item (usually the creation date). In the case the application supports date ranges (like this image is from 1998-2000), this attribute always return the start of date range.</td>
        </tr><tr>
            <th>"dateto"</th>
            <td>QDateTime</td>
            <td>In the case the application supports date ranges (like this image is from 1998-2000), this attribute returns the end of date range.</td>
        </tr><tr>
            <th>"isexactdate"</th>
            <td>bool value</td>
            <td>In the case the application supports date ranges (like this image is from 1998-2000), this attribute returns true if the date is an exact specification, and thus not a range.</td>
        </tr><tr>
            <th>"orientation"</th>
            <td>integer value</td>
            <td>Returns orientation information. See ExifOrientation values for details).</td>
        </tr><tr>
            <th>"title"</th>
            <td>QString</td>
            <td>Returns default item title.</td>
        </tr><tr>
            <th>"rating"</th>
            <td>integer value</td>
            <td>Returns item starts 0 <= rate <= 5).</td>
        </tr><tr>
            <th>"colorlabel"</th>
            <td>integer value</td>
            <td>Return item color flag 0 <= colorlabel <= 10.</td>
        </tr><tr>
            <th>"picklabel"</th>
            <td>integer value</td>
            <td>Return item workflow flag 0 <= picklabel <= 4.</td>
        </tr><tr>
            <th>"latitude"</th>
            <td>double value</td>
            <td>Return item latitude in degrees (-90.0 >= lat <=90.0).</td>
        </tr><tr>
            <th>"longitude"</th>
            <td>double value</td>
            <td>Return item longitude in degrees (-180.0 >= long <=180.0).</td>
        </tr><tr>
            <th>"altitude"</th>
            <td>double value</td>
            <td>Return item altitude in meters.</td>
        </tr><tr>
            <th>"tagspath"</th>
            <td>QStringList</td>
            <td>Return all tags path list of item, formatted as "Country/France/City/Paris" for ex.</td>
        </tr><tr>
            <th>"keywords"</th>
            <td>QStringList</td>
            <td>Return all tags name list (without path).</td>
        </tr><tr>
            <th>"filesize"</th>
            <td>qlonglong value</td>
            <td>Return item file size in bytes.</td>
        </tr><tr>
            <th>"creators"</th>
            <td>QStringList</td>
            <td>Return creators list of item.</td>
        </tr><tr>
            <th>"credit"</th>
            <td>QString</td>
            <td>Return credit of item.</td>
        </tr><tr>
            <th>"rights"</th>
            <td>QString</td>
            <td>Return rights of item.</td>
        </tr><tr>
            <th>"source"</th>
            <td>QString</td>
            <td>Return source of item.</td>
        </tr><tr>
            <th>"tags"</th>
            <td>QStringList</td>
            <td>For compatibility. DEPRECATED and replaced by "keywords" attribute.</td>
        </tr><tr>
            <th>"angle"</th>
            <td>integer value</td>
            <td>For compatibility. DEPRECATED and replaced by "orientation" attribute.</td>
      </tr></table>
    */
    QMap<QString, QVariant> attributes() const;

    /** Set the attributes defined from the map to the image. Following keys/values are the same the attributes(),
     *  excepted "keywords", "filesize", and "isexactdate" properties which are read-only values.
    */
    void addAttributes(const QMap<QString, QVariant>&);

    /** Remove attributes listed from the image. Following values can be used:

      <table><tr>
            <th>ATTRIBUTES</th>
            <th>COMMENTS</th>
        </tr><tr>
            <th>"comment"</th>
            <td>Remove all comments.</td>
        </tr><tr>
            <th>"date"</th>
            <td>Remove date info.</td>
        </tr><tr>
            <th>"orientation"</th>
            <td>Remove orientation info.</td>
        </tr><tr>
            <th>"title"</th>
            <td>Remove all titles.</td>
        </tr><tr>
            <th>"rating"</th>
            <td>Remove rating info.</td>
        </tr><tr>
            <th>"colorlabel"</th>
            <td>Remove color labels info.</td>
        </tr><tr>
            <th>"picklabel"</th>
            <td>Remove pick labels info.</td>
        </tr><tr>
            <th>"gpslocation"</th>
            <td>Return item altitude in meters.</td>
        </tr><tr>
            <th>"tagspath"</th>
            <td>Remove all tags path.</td>
        </tr><tr>
            <th>"copyrights"</th>
            <td>Remove all copyrights info.</td>
        </tr><tr>
            <th>"tags"</th>
            <td>For compatibility. DEPRECATED and replaced by "keywords" attribute.</td>
        </tr><tr>
            <th>"angle"</th>
            <td>For compatibility. DEPRECATED and replaced by "orientation" attribute.</td>
      </tr></table>
    */
    void delAttributes(const QStringList&);

    /** Remove all attribute from the image. See delAttributes() for list of all attributes removed.
     */
    void clearAttributes();

    /** Copies all the attributes from the other imageinfo.
    */
    void cloneData(const ImageInfo& other);

    /** Return item URL.
    */
    QUrl url() const;

private:

    ImageInfo();                            /// Disable
    ImageInfo& operator=(const ImageInfo&); /// Disable

private:

    mutable ImageInfoShared* d;
};

} // namespace KIPI

#endif /* KIPI_IMAGEINFO_H */
