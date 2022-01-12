/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPI_IMAGECOLLECTION_H
#define KIPI_IMAGECOLLECTION_H

// Qt includes

#include <QString>
#include <QDateTime>
#include <QUrl>

// Local includes

#include "libkipi_export.h"

/** @namespace KIPI */
namespace KIPI
{

class ImageCollectionShared;

/** @class ImageCollection imagecollection.h <KIPI/ImageCollection>
 *
    Holds info about the collection from KIPI host application.
    @note Plugins should never create an instance of ImageCollection, only the host application should do that.
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

    // Collection properties ---------------------------------------------------------------------------

    /**
     * Returns the name of collection.
     */
    QString    name() const;

    /**
     * Returns the comment for the collection of images or QString() if that does not make any sense.
     * A comment makes sense for an album, but not for a KIPI::Interface::currentSelection().
     */
    QString    comment() const;

    /**
     * Return the category of the image collection. For example,
     * a category can be a sorting class like 'travels', 'friends', 'monuments', etc.
     */
    QString    category() const;

    /**
     * Returns the Creation date of the image collection. The default implementation
     * return a null date.
     */
    QDate      date() const;

    /**
     * Returns a list of image URLs hosted by collection.
     */
    QList<QUrl> images() const;

    /**
     * Returns the directory for the image collection.
     * The host application may, however, return anything in case this
     * imagecollection is not a directory (check isDirectory()),  or may
     * return the directory of the first image in the collection, the root
     * of the image collection (in case all images has a common root), or
     * even an empty URL.
     */
    QUrl       url() const;

    /**
     * Returns the directory to place images into.
     * This function should only be called if KIPI::Features AcceptNewImages
     * is available.
     *
     * The function may choose to return the directory for the image collection
     * or if images from the collection are not available in a common directory,
     * then instead a common upload directory.
     *
     * @note uploadRootUrl() must be a subpath of uploadUrl()
     */
    QUrl       uploadUrl() const;

    /**
     * When a plugin wants to upload images, it may choose to display an upload widget,
     * which gives the user the possible to show a directory from a tree view.
     *
     * This tree view widget needs to starts at some URL. This function specifies that location.
     * Here are a couple of possible return value different host applications may choose.
     *
     * If all images are stored rooted at some tree (which is the case for KPhotoAlbum),
     * then this function may return this directory unconditionally.
     * The root directory returned by uploadUrl() (which is the default implementation for this method)
     * will be the directory returned by uploadUrl().
     *
     * @note uploadRootUrl() must be a subpath of uploadUrl()
     */
    QUrl       uploadRootUrl() const;

    /**
     * This function return the name of the upload root path used by the
     * the KIPI::UploadWidget. This name can be different for each host
     * app (like "Images" for KPhotoAlbum).
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
