/* ============================================================
 * File  : interface.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>, Jesper K. Pedersen <blackie@kde.org>
 * Date  : 2004-02-19
 * Description :
 *
 * Copyright 2004 by Renchi Raju, Jesper K. Pedersen

 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "pluginloader.h"
#include "interface.h"
#include <kdebug.h>

/*!
  \enum KIPI::Features
  Not all host applications support the full subset of features that KIPI
  allows access to. As an example <a
  href="http://ktown.kde.org/kimdaba/">KimDaBa</a> do not support comments
  for albums. Thus before a plugin expect a decant value for the comment,
  it should check whether \ref KIPI::AlbumsHaveComments are set. It does so
  using \ref KIPI::Interface::hasFeature()
*/

/*!
  \enum KIPI::AlbumsHaveComments
  This feature specify that albums have descriptions associated to them.
 */

/*!
  \enum KIPI::AlbumEQDir
  This feature specifies that each album is equal to a directory for the host application.
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

KIPI::Interface::Interface(QObject *parent, const char *name )
    : QObject(parent, name)
{
    connect( this, SIGNAL( selectionChanged( bool ) ), this, SLOT( stateChange() ) );
    connect( this, SIGNAL( currentAlbumChanged( bool ) ), this, SLOT( stateChange() ) );
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
   See \ref KIPI::Features for details on the individual features.
*/
bool KIPI::Interface::hasFeature( KIPI::Features feature )
{
    return ( features() & feature ) != 0;
}

bool KIPI::Interface::hasFeature( const QString& feature )
{
    if ( feature == "AlbumsHaveComments" )
        return hasFeature( KIPI::AlbumsHaveComments );
    else if ( feature == "AlbumEQDir" )
        return hasFeature( KIPI::AlbumEQDir );
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
            "have been overriden in the host application.\n";
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
QValueList<KIPI::ImageCollection> KIPI::Interface::allAlbums()
{
    // This implementation is just to be able to write documentation above.
    return QValueList<KIPI::ImageCollection>();
}


/**
   Returns the current selection if one exists, otherwise the current
   album.
   Plugins should with everything else equal act the same way, which is to
   use the selection if one exists, otherwise use the whole album.
   This instead of writing code like below, you should instead use this
   function:
   \code
    ImageCollection images = interface->currentSelection();
    if ( !images.isValid() )
        images = interface->currentAlbum();
   \endcode
*/
KIPI::ImageCollection KIPI::Interface::currentScope()
{
    ImageCollection images = currentSelection();
    if ( images.isValid() )
        return images;
    else
        return currentAlbum();
}

void KIPI::Interface::stateChange()
{
    ImageCollection images = currentScope();
    emit currentScopeChanged( images.isValid() );
}

/**
   Return a bitwise or of the KIPI::Features that thus application support.
*/
int KIPI::Interface::features() const
{
    // This implementation is just to be able to write documentation above.
    return 0;
}


#include "interface.moc"
