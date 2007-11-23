/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : main kipi host application interface .
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

/** @file interface.cpp */

// KDE includes.

#include <kdebug.h>
#include <kimageio.h>
#include <kio/previewjob.h>

// Local includes.
 
#include "pluginloader.h"
#include "interface.h"
#include "interface.moc"

/*!
  @enum KIPI::Features
  Not all host applications support the full subset of features that KIPI
  allows access to. As an example <a
  href="http://ktown.kde.org/kimdaba/">KimDaBa</a> do not support comments
  for albums. Thus before a plugin expect a decant value for the comment,
  it should check whether KIPI::AlbumsHaveComments are set. It does so
  using KIPI::Interface::hasFeature()
  When adding new items, remember to update "hasFeature( const QString& feature )"
  and the hello world plugin.
  */

/*!
  \enum KIPI::AlbumsHaveComments
  This feature specify that albums have descriptions associated to them.
 */

/*!
  \enum KIPI::ImagesHasComments
  This feature specifies that images in the host application has descriptions associated to them.
 */

/*!
  \enum KIPI::ImagesHasTime
  This feature specifies that images has a date associated with it, which the host application can display and set
*/

/*!
  \enum KIPI::HostSupportsDateRanges
  This feature specify whether the host application supports that the user can specify a date range for images,
  like 1998-2000.
*/

/*!
  \enum KIPI::HostAcceptNewImages
  This feature specifies that the host application do accept new images.
  Use \ref ImageCollection::uploadPath to find the location to place the image, and
  \ref KIPI::Interface::addImage() to tell the host application about the new image.
*/

/*!
  \enum KIPI::ImagesHasTitlesWritable
  This features specifies whether the plugin can change the title for images
*/

/*!
  \enum KIPI::AlbumsHaveCategory
  This feature specify that albums are category associated to them ('travels', 'friends', 'monuments', etc.).
*/

/*!
  \enum KIPI::AlbumsHaveCreationDate
  This feature specify that albums are a creation date associated to them.
*/

/*!
  \enum KIPI::AlbumsUseFirstImagePreview
  This feature specify that albums use the first image of the collection like preview.
  This flag is used in ImageGallery, CDArchiving, and FinDuplicateImages dialog for 
  to lauch a preview of the album.
*/

/*!
  \enum KIPI::HostSupportsTags
  This feature specify whether the host application supports keywords for images,
*/

/*!
  \enum KIPI::HostSupportsRating
  This feature specify whether the host application supports rating values for images,
*/

/*!
  \enum KIPI::HostSupportsThumbnails
  This feature specifies that host application can provide image thumbnails.
 */

namespace KIPI
{

Interface::Interface(QObject *parent, const char *name )
         : QObject(parent)
{
    setObjectName(name);
}

Interface::~Interface()
{
}

/**
   Tells the host app that the following images has changed on disk
*/
void Interface::refreshImages( const KUrl::List& )
{
}

/**
   Tells whether the host application under which the plugin currently executes a given feature.
   See KIPI::Features for details on the individual features.
*/
bool Interface::hasFeature( KIPI::Features feature )
{
    return ( features() & feature ) != 0;
}

bool Interface::hasFeature( const QString& feature )
{
    if ( feature == "AlbumsHaveComments" )
        return hasFeature( AlbumsHaveComments );
    else if ( feature == "ImagesHasComments" )
        return hasFeature( ImagesHasComments );
    else if ( feature == "ImagesHasTime" )
        return hasFeature( ImagesHasTime );
    else if ( feature == "HostSupportsThumbnails" )
        return hasFeature( HostSupportsThumbnails );
    else if ( feature == "HostSupportsDateRanges" )
        return hasFeature( HostSupportsDateRanges );
    else if ( feature == "HostAcceptNewImages" )
        return hasFeature( HostAcceptNewImages );
    else if ( feature == "ImagesHasTitlesWritable" )
        return hasFeature( ImagesHasTitlesWritable );
    else if ( feature == "AlbumsHaveCategory" )
        return hasFeature( AlbumsHaveCategory );
    else if ( feature == "AlbumsHaveCreationDate" )
        return hasFeature( AlbumsHaveCreationDate );
    else if ( feature == "AlbumsUseFirstImagePreview" )
        return hasFeature( AlbumsUseFirstImagePreview );
    else if ( feature == "HostSupportsTags" )
        return hasFeature( HostSupportsTags );
    else if ( feature == "HostSupportsRating" )
        return hasFeature( HostSupportsRating );
    else 
    {
        kWarning( 51000 ) << "Unknown feature asked for in KIPI::Interface::hasFeature: " << feature;
        return false;
    }
}

/*!
  Tell the host application that a new image has been made available to it.
  Returns true if the host application did accept the new image, otherwise err will be filled with
  an error description.
*/
bool Interface::addImage( const KUrl&, QString& /*err*/ )
{
    kWarning(51000) << "Interface::addImage should only be invoked if the host application supports the KIPI::Features\n"
            "AcceptNewImages - if the host application do support that, then this function should\n"
            "have been overridden in the host application.\n" << endl;
    return false;
}

void Interface::delImage( const KUrl& )
{
}

/**
   Returns list of all images in current album.
   If there are no current album, the returned
   KIPI::ImageCollection::isValid() will return false.
*/
KIPI::ImageCollection KIPI::Interface::currentAlbum()
{
    // This implementation is just to be able to write documentation above.
    return ImageCollection();
}

/**
   Current selection in a thumbnail view for example.
   If there are no current selection, the returned
   KIPI::ImageCollection::isValid() will return false.
*/
ImageCollection Interface::currentSelection()
{
    // This implementation is just to be able to write documentation above.
    return KIPI::ImageCollection();
}

/**
   Returns a list of albums.
*/
QList<ImageCollection> Interface::allAlbums()
{
    // This implementation is just to be able to write documentation above.
    return QList<ImageCollection>();
}

/**
   Return a bitwise or of the KIPI::Features that thus application support.
*/
int Interface::features() const
{
    // This implementation is just to be able to write documentation above.
    return 0;
}

/**
   Return a list of images file extension will be used in the plugins for 
   to sort the files list before a treatment. The default implementation return,
   the supported images formats by KDE.
*/
QString Interface::fileExtensions()
{
    QStringList KDEImagetypes = KImageIO::mimeTypes( KImageIO::Reading );
    QString imagesFileFilter = KDEImagetypes.join(" ");
    return ( imagesFileFilter.toLower() + " " + imagesFileFilter.toUpper() );
}

/**
  Ask to Kipi host application to render a thumbnail for an image. If this method is not 
  re-implemented in host, standard KIO::filePreview is used to generated a thumbnail.
  Use gotThumbnail() signal to take thumb.
*/
void Interface::thumbnail( const KUrl& url, int size )
{
    KUrl::List list;
    list << url;
    thumbnails(list, size);
}

/**
  Ask to Kipi host application to render thumbnails for a list of images. If this method is not 
  re-implemented in host, standard KIO::filePreview is used to generated a thumbnail. 
  Use gotThumbnail() signal to take thumbs.
*/
void Interface::thumbnails( const KUrl::List& list, int size )
{
    KIO::PreviewJob *job = KIO::filePreview(list, size);

    connect(job, SIGNAL(gotPreview(const KFileItem &, const QPixmap &)),
            this, SLOT(gotKDEPreview(const KFileItem &, const QPixmap &)));

    connect(job, SIGNAL(failed(const KFileItem &)),
            this, SLOT(failedKDEPreview(const KFileItem &)));
}

void Interface::gotKDEPreview(const KFileItem& item, const QPixmap &pix)
{
    emit gotThumbnail(item.url(), pix);
}

void Interface::failedKDEPreview(const KFileItem& item)
{
    emit gotThumbnail(item.url(), QPixmap());
}

} // namespace KIPI
