/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : interface for host application.
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
  \enum KIPI::SupportsDateRanges
  This feature specify whether the host application supports that the user can specify a date range for images,
  like 1998-2000.
*/

/*!
  \enum KIPI::AcceptNewImages
  This feature specifies that the host application do accept new images.
  Use \ref ImageCollection::uploadPath to find the location to place the image, and
  \ref KIPI::Interface::addImage() to tell the host application about the new image.
*/

/*!
  \enum KIPI::ImageTitlesWritable
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


KIPI::Interface::Interface(QObject *parent, const char *name )
    : QObject(parent, name)
{
}

KIPI::Interface::~Interface()
{
}

/**
   Tells the host app that the following images has changed on disk
*/
void KIPI::Interface::refreshImages( const KURL::List& )
{
}

/**
   Tells whether the host application under which the plugin currently executes a given feature.
   See KIPI::Features for details on the individual features.
*/
bool KIPI::Interface::hasFeature( KIPI::Features feature )
{
    return ( features() & feature ) != 0;
}

bool KIPI::Interface::hasFeature( const QString& feature )
{
    if ( feature == "AlbumsHaveComments" )
        return hasFeature( KIPI::AlbumsHaveComments );
    else if ( feature == "ImagesHasComments" )
        return hasFeature( KIPI::ImagesHasComments );
    else if ( feature == "ImagesHasTime" )
        return hasFeature( KIPI::ImagesHasTime );
    else if ( feature == "SupportsDateRanges" )
        return hasFeature( KIPI::SupportsDateRanges );
    else if ( feature == "AcceptNewImages" )
        return hasFeature( KIPI::AcceptNewImages );
    else if ( feature == "ImageTitlesWritable" )
        return hasFeature( KIPI::ImageTitlesWritable );
    else if ( feature == "AlbumsHaveCategory" )
        return hasFeature( KIPI::AlbumsHaveCategory );
    else if ( feature == "AlbumsHaveCreationDate" )
        return hasFeature( KIPI::AlbumsHaveCreationDate );
    else if ( feature == "AlbumsUseFirstImagePreview" )
        return hasFeature( KIPI::AlbumsUseFirstImagePreview );
    else if ( feature == "HostSupportsTags" )
        return hasFeature( KIPI::HostSupportsTags );
    else {
        kdWarning( 51000 ) << "Unknown feature asked for in KIPI::Interface::hasFeature: " << feature << endl;
        return false;
    }
}

/*!
  Tell the host application that a new image has been made available to it.
  Returns true if the host application did accept the new image, otherwise err will be filled with
  an error description.
*/
bool KIPI::Interface::addImage( const KURL&, QString& /*err*/ )
{
    kdWarning(51000) << "Interface::addImage should only be invoked if the host application supports the KIPI::Features\n"
            "AcceptNewImages - if the host application do support that, then this function should\n"
            "have been overridden in the host application.\n" << endl;
    return false;
}

void KIPI::Interface::delImage( const KURL& )
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
    return KIPI::ImageCollection();
}

/**
   Current selection in a thumbnail view for example.
   If there are no current selection, the returned
   KIPI::ImageCollection::isValid() will return false.
*/
KIPI::ImageCollection KIPI::Interface::currentSelection()
{
    // This implementation is just to be able to write documentation above.
    return KIPI::ImageCollection();
}

/**
   Returns a list of albums.
*/
QList<KIPI::ImageCollection> KIPI::Interface::allAlbums()
{
    // This implementation is just to be able to write documentation above.
    return QList<KIPI::ImageCollection>();
}


/**
   Return a bitwise or of the KIPI::Features that thus application support.
*/
int KIPI::Interface::features() const
{
    // This implementation is just to be able to write documentation above.
    return 0;
}

/**
   Return a list of images file extension will be used in the plugins for 
   to sort the files list before a treatment. The default implementation return,
   the supported images formats by KDE.
*/
QString KIPI::Interface::fileExtensions()
{
    QStringList KDEImagetypes = KImageIO::mimeTypes( KImageIO::Reading );
    QString imagesFileFilter = KDEImagetypes.join(" ");
    return ( imagesFileFilter.lower() + " " + imagesFileFilter.upper() );
}
