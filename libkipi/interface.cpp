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
  \enum KIPI::AlbumsHaveDescriptions
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
  \ref addImage() to tell the host application about the new image.
*/

KIPI::Interface::Interface(QObject *parent, const char *name )
    : QObject(parent, name)
{
}

KIPI::Interface::~Interface()
{
}

void KIPI::Interface::refreshImages( const KURL::List& )
{
}

bool KIPI::Interface::hasFeature( KIPI::Features feature )
{
    return ( features() & feature ) != 0;
}

bool KIPI::Interface::hasFeature( const QString& feature )
{
    if ( feature == "AlbumsHaveDescriptions" )
        return hasFeature( KIPI::AlbumsHaveDescriptions );
    else if ( feature == "AlbumEQDir" )
        return hasFeature( KIPI::AlbumEQDir );
    else if ( feature == "ImagesHasComments" )
        return hasFeature( KIPI::ImagesHasComments );
    else {
        kdWarning( 51000 ) << "Unknow feature asked for in KIPI::Interface::hasFeature: " << feature << endl;
        return false;
    }
}

/*!
  Tell the host application that a new image has been made available to it.
  Returns true if the host application did accept the new image.
*/
bool KIPI::Interface::addImage( const KURL& )
{
    qFatal( "This method should only be invoked if the host application supports the KIPI::Features\n"
            "AcceptNewImages - if the host application do support that, then this function should\n"
            "have been overriden in the host application.");
    return false;
}

#include "interface.moc"
