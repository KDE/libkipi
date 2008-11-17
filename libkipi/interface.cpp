/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : main kipi host application interface.
 *
 * Copyright (C) 2004-2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

// Qt includes.

#include <QPixmap>
#include <QWidget>

// KDE includes.

#include <kdebug.h>
#include <kfileitem.h>
#include <kimageio.h>
#include <kio/previewjob.h>

// Local includes.

#include "version.h"
#include "pluginloader.h"
#include "imagecollectionselector.h"
#include "uploadwidget.h"
#include "interface.h"
#include "interface.moc"

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

QString KIPI::Interface::version()
{
    return QString(kipi_version);
}

void Interface::refreshImages( const KUrl::List& )
{
}

bool Interface::hasFeature( KIPI::Features feature ) const
{
    return ( features() & feature ) != 0;
}

bool Interface::hasFeature( const QString& feature ) const
{
    if ( feature == "CollectionsHaveComments" )
        return hasFeature( CollectionsHaveComments );
    else if ( feature == "CollectionsHaveCategory" )
        return hasFeature( CollectionsHaveCategory );
    else if ( feature == "CollectionsHaveCreationDate" )
        return hasFeature( CollectionsHaveCreationDate );
    else if ( feature == "ImagesHasComments" )
        return hasFeature( ImagesHasComments );
    else if ( feature == "ImagesHasTime" )
        return hasFeature( ImagesHasTime );
    else if ( feature == "ImagesHasTitlesWritable" )
        return hasFeature( ImagesHasTitlesWritable );
    else if ( feature == "HostSupportsThumbnails" )
        return hasFeature( HostSupportsThumbnails );
    else if ( feature == "HostSupportsDateRanges" )
        return hasFeature( HostSupportsDateRanges );
    else if ( feature == "HostAcceptNewImages" )
        return hasFeature( HostAcceptNewImages );
    else if ( feature == "HostSupportsProgressBar" )
        return hasFeature( HostSupportsProgressBar );
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

KIPI::ImageCollection KIPI::Interface::currentAlbum()
{
    // This implementation is just to be able to write documentation above.
    return ImageCollection();
}

ImageCollection Interface::currentSelection()
{
    // This implementation is just to be able to write documentation above.
    return KIPI::ImageCollection();
}

QList<ImageCollection> Interface::allAlbums()
{
    // This implementation is just to be able to write documentation above.
    return QList<ImageCollection>();
}

int Interface::features() const
{
    // This implementation is just to be able to write documentation above.
    return 0;
}

QString Interface::fileExtensions()
{
    QStringList KDEImagetypes = KImageIO::mimeTypes( KImageIO::Reading );
    QString imagesFileFilter = KDEImagetypes.join(" ");
    return ( imagesFileFilter.toLower() + " " + imagesFileFilter.toUpper() );
}

void Interface::thumbnail( const KUrl& url, int size )
{
    KUrl::List list;
    list << url;
    thumbnails(list, size);
}

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

QVariant Interface::hostSetting(const QString& /*settingName*/)
{
    return QVariant();
}

} // namespace KIPI
