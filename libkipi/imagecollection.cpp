/* ============================================================
 * File   : imagecollection.cpp
 * Authors: KIPI team developers
 *	    
 * Date   : 2004-02
 * Description :
 *
 * Copyright 2004 by the KIPI team
 *
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

// KDE includes.

#include <kdebug.h>
#include <klocale.h>

// Local includes.
  
#include "imagecollection.h"
#include "imagecollectionshared.h"

/**
   returns the comment for the collection of images or QString::null if that doesn't make any sense.
   A comment makes sense for an album, but not for a KIPI::Interface::currentSelection().
*/
QString KIPI::ImageCollection::comment() const
{
    if ( _data )
        return _data->comment();
    else {
        printNullError();
        return QString::null;
    }
}

/**
   PENDING(blackie) document
*/
QString KIPI::ImageCollection::name() const
{
    if ( _data )
        return _data->name();
    else {
        printNullError();
        return QString::null;
    }
}

/**
   PENDING(blackie) document
*/
KURL::List KIPI::ImageCollection::images() const
{
    if ( _data )
        return _data->images();
    else {
        printNullError();
        return KURL::List();
    }
}

KIPI::ImageCollection::ImageCollection( ImageCollectionShared* data )
    : _data( data )
{
}

KIPI::ImageCollection::~ImageCollection()
{
    if ( _data )
        _data->removeRef();
}

KIPI::ImageCollection::ImageCollection( const ImageCollection& rhs )
{
    if ( rhs._data ) {
        _data = rhs._data;
        _data->addRef();
    }
    else
        _data = 0;
}

KIPI::ImageCollection::ImageCollection()
{
    _data = 0;
}

KIPI::ImageCollection& KIPI::ImageCollection::operator=( const KIPI::ImageCollection& rhs )
{
    if ( rhs._data == _data )
        return *this;

    if ( _data )
        _data->removeRef();
    if ( !rhs._data ) {
        printNullError();
        _data = 0;
    }
    else {
        _data = rhs._data;
        _data->addRef();
    }
    return *this;
}

/*!
  Returns the directory for the image collections.
  The host application may, however, return anything in case it does not
  support the KIPI::Features AlbumEQDir, e.g. the directory of the first
  image in the collection, the root of the image collection (in case all
  images has a common root), or even an empty URL.
*/
KURL KIPI::ImageCollection::path() const
{
    if ( _data )
        return _data->path();
    else {
        printNullError();
        return KURL();
    }
}

/*!
  Returns the directory to place images into.
  This function should only be called if KIPI::Features AcceptNewImages
  is available.

  The function may choose to return the directory for the image collection
  or if images from the collection are not available in a common directory,
  then instead a common upload directory.
  In contrast to \ref path, this function must return a valid url.

  <b>IMPORTANT:</b> uploadRoot() must be a subpath of uploadPath()
*/
KURL KIPI::ImageCollection::uploadPath() const
{
    if ( _data )
        return _data->uploadPath();
    else {
        printNullError();
        return KURL();
    }
}

/*!
  When a plugin wants to upload images, it may choose to display an upload widget,
  which gives the user the possible to show a directory from a tree view.

  This tree view widget needs to starts at some URL. This function specifies that location.
  Here are a couble of possible return value different host applications may choose.
  <ul>
  <li> If all images are stored rooted at some tree (which is the case for
  KimDaBa), then this function may return this directory unconditionally.
  <li> The root directory returned by uploadPath() (which is the default implementation for this method)
  <li> The directory returned by uploadPath().
  </ul>

  <b>IMPORTANT:</b> uploadRoot() must be a subpath of uploadPath()
*/
KURL KIPI::ImageCollection::uploadRoot() const
{
    if ( _data )
        return _data->uploadRoot();
    else {
        printNullError();
        return KURL();
    }
}

/*!
  This fonction return the name of the upload root path used by the 
  the KIPI::UploadWidget. This name can be different for each host 
  app (like "Images" for Kimdaba or "My Albums" for Digikam).
  .
*/
QString KIPI::ImageCollection::uploadRootName() const
{
    if ( _data )
        return _data->uploadRootName();
    else {
        printNullError();
        return QString::null;
    }
}

bool KIPI::ImageCollection::isValid() const
{
    return (_data != 0);
}

void KIPI::ImageCollection::printNullError() const
{
    kdWarning( 51000 ) << "Image collection is invalid - this might be the case if you asked for an album, " << endl
                       << "and not album existed. You should check using .isValid() first." << endl
                       << "Notice: Plugins should never create an instance of ImageCollection, only the host application "
                       << "should do that." << endl;
}

