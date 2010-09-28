/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  image collection
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

#include "imagecollection.h"

// KDE includes.

#include <kdebug.h>

// Local includes.

#include "imagecollectionshared.h"

namespace KIPI
{

/**
   @file imagecollection.cpp
   returns the comment for the collection of images or QString::null if that doesn't make any sense.
   A comment makes sense for an album, but not for a KIPI::Interface::currentSelection().
*/
QString ImageCollection::comment() const
{
    if ( d )
    {
        return d->comment();
    }
    else
    {
        printNullError();
        return QString::null;
    }
}

/**
   PENDING(blackie) document
*/
QString ImageCollection::name() const
{
    if ( d )
    {
        return d->name();
    }
    else
    {
        printNullError();
        return QString();
    }
}

/**
   Return the category of the image collection. For example in Digikam,
   a category is a sorting class like 'travels', 'friends', 'monuments', etc.
*/
QString ImageCollection::category() const
{
    if ( d )
    {
        return d->category();
    }
    else
    {
        printNullError();
        return QString::null;
    }
}

/**
   Return the Creation date of the image collection. The default implementation
   return a null date.
*/
QDate ImageCollection::date() const
{
    if ( d )
    {
        return d->date();
    }
    else
    {
        printNullError();
        return QDate();
    }
}

/**
   PENDING(blackie) document
*/
KUrl::List ImageCollection::images() const
{
    if ( d )
    {
        return d->images();
    }
    else
    {
        printNullError();
        return KUrl::List();
    }
}

ImageCollection::ImageCollection( ImageCollectionShared* const data )
               : d( data )
{
}

ImageCollection::ImageCollection( const ImageCollection& rhs )
{
    if ( rhs.d )
    {
        d = rhs.d;
        d->addRef();
    }
    else
    {
        d = 0;
    }
}

ImageCollection::ImageCollection()
{
    d = 0;
}

ImageCollection::~ImageCollection()
{
    if ( d )
        d->removeRef();
}

ImageCollection& ImageCollection::operator=( const ImageCollection& rhs )
{
    if ( rhs.d == d )
        return *this;

    if ( d )
        d->removeRef();

    if ( !rhs.d )
    {
        printNullError();
        d = 0;
    }
    else
    {
        d = rhs.d;
        d->addRef();
    }
    return *this;
}

/*!
  Returns the directory for the image collection.
  The host application may, however, return anything in case this
  imagecollection is not a directory (check isDirectory()),  or may
  return the directory of the first image in the collection, the root
   of the image collection (in case all images has a common root), or
   even an empty URL.
*/
KUrl ImageCollection::path() const
{
    if ( d )
    {
        return d->path();
    }
    else
    {
        printNullError();
        return KUrl();
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
KUrl ImageCollection::uploadPath() const
{
    if ( d )
    {
        return d->uploadPath();
    }
    else
    {
        printNullError();
        return KUrl();
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
KUrl ImageCollection::uploadRoot() const
{
    if ( d )
    {
        return d->uploadRoot();
    }
    else
    {
        printNullError();
        return KUrl();
    }
}

/*!
  This fonction return the name of the upload root path used by the
  the KIPI::UploadWidget. This name can be different for each host
  app (like "Images" for Kimdaba or "My Albums" for Digikam).
*/
QString ImageCollection::uploadRootName() const
{
    if ( d )
    {
        return d->uploadRootName();
    }
    else
    {
        printNullError();
        return QString();
    }
}

/*!
  Returns whether an imagecollection is a physical folder on the filesystem
  or not. It is important to check this, if your plugin needs to do folder
  based operations for an image collection.
*/
bool ImageCollection::isDirectory() const
{
    if ( d )
    {
        return d->isDirectory();
    }
    else
    {
        printNullError();
        return false;
    }
}

bool ImageCollection::isValid() const
{
    return (d != 0);
}

void ImageCollection::printNullError() const
{
    kWarning() << "Image collection is invalid - this might be the case if you asked for an album, " << endl
               << "and not album existed. You should check using .isValid() first." << endl
               << "Notice: Plugins should never create an instance of ImageCollection, only the host application "
               << "should do that.";
}

bool ImageCollection::operator==(const KIPI::ImageCollection& ic) const
{
    if (!d || !(ic.d))
    {
        printNullError();
        return false;
    }
    return *d == *(ic.d);
}

} // namespace KIPI
