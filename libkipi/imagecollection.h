/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  image collection
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

#ifndef KIPI_IMAGECOLLECTION_H
#define KIPI_IMAGECOLLECTION_H

// Qt includes

#include <QtCore/QString>
#include <QtCore/QDateTime>

// KDE includes

#include <kurl.h>

// Local includes

#include "libkipi_export.h"

/** @namespace KIPI */
namespace KIPI
{

class ImageCollectionShared;

/** @class ImageCollection
    Holds info about the collection from KIPI host application.
    NOTE: Plugins should never create an instance of ImageCollection, only the host application should do that.
 */
class LIBKIPI_EXPORT ImageCollection
{

public:

    ImageCollection(ImageCollectionShared* const);
    ImageCollection(const ImageCollection& other);
    ImageCollection();
    ~ImageCollection();

    ImageCollection& operator=(const ImageCollection&);
    bool operator==(const ImageCollection&) const;

    /// Collection properties ---------------------------------------------------------------------------

    /**
     * Returns the name of collection.
     */
    QString    name() const;

    /**
     * Returns the comment for the collection of images or QString::null if that doesn't make any sense.
     * A comment makes sense for an album, but not for a KIPI::Interface::currentSelection().
     */
    QString    comment() const;

    /**
     * Return the category of the image collection. For example in digiKam,
     * a category is a sorting class like 'travels', 'friends', 'monuments', etc.
     */
    QString    category() const;

    /**
     * Returns the Creation date of the image collection. The default implementation
     * return a null date.
     */
    QDate      date() const;

    /**
     * Returns a list of image urls hosted by collection.
     */
    KUrl::List images() const;

    /**
     * Returns the directory for the image collection.
     * The host application may, however, return anything in case this
     * imagecollection is not a directory (check isDirectory()),  or may
     * return the directory of the first image in the collection, the root
     * of the image collection (in case all images has a common root), or
     * even an empty URL.
     */
    KUrl       path() const;

    /**
     * Returns the directory to place images into.
     * This function should only be called if KIPI::Features AcceptNewImages
     * is available.
     *
     * The function may choose to return the directory for the image collection
     * or if images from the collection are not available in a common directory,
     * then instead a common upload directory.
     * In contrast to \ref path, this function must return a valid url.
     *
     * IMPORTANT: uploadRoot() must be a subpath of uploadPath()
     */
    KUrl       uploadPath() const;

    /**
     * When a plugin wants to upload images, it may choose to display an upload widget,
     * which gives the user the possible to show a directory from a tree view.
     *
     * This tree view widget needs to starts at some URL. This function specifies that location.
     * Here are a couble of possible return value different host applications may choose.
     *
     * If all images are stored rooted at some tree (which is the case for KPhotoAlbum), 
     * then this function may return this directory unconditionally.
     * The root directory returned by uploadPath() (which is the default implementation for this method)
     * will be the directory returned by uploadPath().
     * 
     * IMPORTANT: uploadRoot() must be a subpath of uploadPath()
     */
    KUrl       uploadRoot() const;

    /**
     * This fonction return the name of the upload root path used by the
     * the KIPI::UploadWidget. This name can be different for each host
     * app (like "Images" for Kphotoalbum or "My Albums" for digiKam).
     */
    QString    uploadRootName() const;

    /**
     * Returns whether an imagecollection is a physical folder on the filesystem
     * or not. It is important to check this, if your plugin needs to do folder
     * based operations for an image collection.
     */
    bool       isDirectory() const;

    /**
     * Returns true if this Collection is valid
     */
    bool       isValid() const;

private:

    mutable ImageCollectionShared* d;
};

} // namespace KIPI

#endif /* IMAGECOLLECTION_H */
