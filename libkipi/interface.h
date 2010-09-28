/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  main kipi host application interface
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

/** @file interface.h */

#ifndef KIPI_INTERFACE_H
#define KIPI_INTERFACE_H

// Qt includes.

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QList>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "imagecollection.h"
#include "imageinfo.h"
#include "libkipi_export.h"

class QPixmap;
class QWidget;
class KFileItem;
class QAbstractItemModel;

/** @namespace KIPI */

namespace KIPI
{

class ImageCollectionSelector;
class UploadWidget;

/*!
  @enum KIPI::Features
  Not all host applications support the full subset of features that KIPI
  allows access to. As an example <a
  href="http://www.digikam.org">digiKam</a> support comments
  for albums. Thus before a plugin expect a decant value for the comment,
  it should check whether KIPI::CollectionsHaveComments are set. It does so
  using KIPI::Interface::hasFeature()
  When adding new items, remember to update "hasFeature( const QString& feature )"
  and the hello world plugin.
  */

/*!
  \enum KIPI::CollectionsHaveComments
  This feature specify that albums have descriptions associated to them.
 */

/*!
  \enum KIPI::ImagesHasComments
  This feature specifies that images in the host application has descriptions associated to them.
 */

/*!
  \enum KIPI::ImagesHasTime
  This feature specifies that images has a date associated with it, which the host application can display and set.
*/

/*!
  \enum KIPI::HostSupportsDateRanges
  This feature specify whether the host application supports that the user can specify a date range for images, like 1998-2000.
*/

/*!
  \enum KIPI::HostAcceptNewImages
  This feature specifies that the host application do accept new images.
  Use \ref ImageCollection::uploadPath to find the location to place the image, and
  \ref KIPI::Interface::addImage() to tell the host application about the new image.
*/

/*!
  \enum KIPI::ImagesHasTitlesWritable
  This features specifies whether the plugin can change the title for images.
*/

/*!
  \enum KIPI::CollectionHaveCategory
  This feature specify that collections are category associated to them ('travels', 'friends', 'monuments', etc.).
*/

/*!
  \enum KIPI::CollectionsHaveCreationDate
  This feature specify that collections are a creation date associated to them.
*/

/*!
  \enum KIPI::HostSupportsProgressBar
  This feature specify whether the host application has a progress bar available for batch operations.
*/

/*!
  \enum KIPI::HostSupportsTags
  This feature specify whether the host application supports keywords for images.
*/

/*!
  \enum KIPI::HostSupportsRating
  This feature specify whether the host application supports rating values for images.
*/

/*!
  \enum KIPI::HostSupportsThumbnails
  This feature specifies that host application can provide image thumbnails.
 */

enum Features
{
    CollectionsHaveComments     = 1 << 0,
    ImagesHasComments           = 1 << 1,
    ImagesHasTime               = 1 << 2,
    HostSupportsDateRanges      = 1 << 3,
    HostAcceptNewImages         = 1 << 4,
    ImagesHasTitlesWritable     = 1 << 5,
    CollectionsHaveCategory     = 1 << 6,
    CollectionsHaveCreationDate = 1 << 7,
    HostSupportsProgressBar     = 1 << 8,
    HostSupportsTags            = 1 << 9,
    HostSupportsRating          = 1 << 10,
    HostSupportsThumbnails      = 1 << 11
};

/** class Interface */
class LIBKIPI_EXPORT Interface : public QObject
{
    Q_OBJECT

public:

    Interface(QObject* parent, const char* name=0);
    virtual ~Interface();

    /**
      Returns list of all images in current album.
      If there are no current album, the returned
      KIPI::ImageCollection::isValid() will return false.
    */
    virtual ImageCollection currentAlbum() = 0;

    /**
      Current selection in a thumbnail view for example.
      If there are no current selection, the returned
      KIPI::ImageCollection::isValid() will return false.
    */
    virtual ImageCollection currentSelection() = 0;

    /**
      Returns a list of albums.
    */
    virtual QList<ImageCollection> allAlbums() = 0;

    virtual ImageInfo info(const KUrl&) = 0;

    /**
      Tell the host application that a new image has been made available to it.
      Returns true if the host application did accept the new image, otherwise err will be filled with
      an error description.
    */
    virtual bool addImage(const KUrl&, QString& err);
    virtual void delImage(const KUrl&);

    /**
      Tells the host app that the following images has changed on disk
    */
    virtual void refreshImages(const KUrl::List&);

    /**
      Ask to Kipi host application to render a thumbnail for an image. If this method is not
      re-implemented in host, standard KIO::filePreview is used to generated a thumbnail.
      Use gotThumbnail() signal to take thumb.
    */
    virtual void thumbnail(const KUrl& url, int size);
    /**
      Ask to Kipi host application to render thumbnails for a list of images. If this method is not
      re-implemented in host, standard KIO::filePreview is used to generated a thumbnail.
      Use gotThumbnail() signal to take thumbs.
    */
    virtual void thumbnails(const KUrl::List& list, int size);

    virtual ImageCollectionSelector* imageCollectionSelector(QWidget* parent)=0;
    virtual UploadWidget* uploadWidget(QWidget* parent)=0;
    virtual QAbstractItemModel* getTagTree() const;

    /**
      Ask to Kipi host application to return a setting to share with plugins, for example to write
      metadata on RAW files.
      Current setting names are:
         "WriteMetadataUpdateFiletimeStamp" (bool)       is true if file timestamp are updated when metadata are saved.
         "WriteMetadataToRAW"               (bool)       is true if RAW files metadata can be changed.
         "FileExtensions"                   (QString)    same than fileExtensions().

      This method return the default settings. Re-implement this method in your dedicated kipi interface
      to control kipi-plugins rules with your kipi host application settings.
    */
    virtual QVariant hostSetting(const QString& settingName);

    /**
      Tells whether the host application under which the plugin currently executes a given feature.
      See KIPI::Features for details on the individual features.
    */
    bool hasFeature(KIPI::Features feature) const;

    /**
      Returns a string version of libkipi release
    */
    static QString version();

Q_SIGNALS:

    void selectionChanged(bool hasSelection);
    void currentAlbumChanged(bool anyAlbum);
    void gotThumbnail(const KUrl&, const QPixmap&);

protected:

    /**
      Return a bitwise or of the KIPI::Features that thus application support.
    */
    virtual int features() const = 0;

private Q_SLOTS:

    void gotKDEPreview(const KFileItem& item, const QPixmap& pix);
    void failedKDEPreview(const KFileItem&);

private:

    bool hasFeature(const QString& feature) const;

    /**
      Return a list of images file extensions supported by KDE
    */
    QString KDEfileExtensions() const;

private:

    friend class PluginLoader;
};

}  // namespace KIPI

#endif /* KIPI_INTERFACE_H */
