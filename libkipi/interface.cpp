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

#include "interface.moc"
