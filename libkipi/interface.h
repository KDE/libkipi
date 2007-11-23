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

/** @file interface.h */

#ifndef KIPI_INTERFACE_H
#define KIPI_INTERFACE_H

// Qt includes.

#include <QPixmap>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QList>

// KDE includes.

#include <kfileitem.h>
#include <kurl.h>

// Local includes.

#include "imagecollection.h"
#include "imageinfo.h"
#include "libkipi_export.h"

/** @namespace KIPI */

namespace KIPI
{

enum Features 
{
    AlbumsHaveComments         = 1 << 0,
    ImagesHasComments          = 1 << 1,
    ImagesHasTime              = 1 << 2,
    HostSupportsDateRanges     = 1 << 3,
    HostAcceptNewImages        = 1 << 4,
    ImagesHasTitlesWritable    = 1 << 5,
    AlbumsHaveCategory         = 1 << 6,
    AlbumsHaveCreationDate     = 1 << 7,
    AlbumsUseFirstImagePreview = 1 << 8,
    HostSupportsTags           = 1 << 9,
    HostSupportsRating         = 1 << 10,
    ImagesHasThumbnails        = 1 << 11
};

/** class Interface */
class LIBKIPI_EXPORT Interface : public QObject
{
    Q_OBJECT

public:

    Interface(QObject *parent, const char *name=0);
    virtual ~Interface();

    virtual ImageCollection currentAlbum() = 0;
    virtual ImageCollection currentSelection() = 0;
    virtual QList<ImageCollection> allAlbums() = 0;

    virtual ImageInfo info( const KUrl& ) = 0;
    virtual bool addImage( const KUrl&, QString& err );
    virtual void delImage( const KUrl& );

    virtual void refreshImages( const KUrl::List& );

    virtual QString fileExtensions();
    
    virtual void thumbnail( const KUrl& url, int size );
    virtual void thumbnails( const KUrl::List& list, int size );

    bool hasFeature( KIPI::Features feature );

Q_SIGNALS:

    void selectionChanged( bool hasSelection );
    void currentAlbumChanged( bool anyAlbum );
    void gotThumbnail( const KUrl&, const QPixmap& );

protected:

    virtual int features() const = 0;

private slots:

    void gotKDEPreview(const KFileItem& item, const QPixmap &pix);
    void failedKDEPreview(const KFileItem&);

private:

    friend class PluginLoader;
    bool hasFeature( const QString& feature );
};

}  // namespace KIPI

#endif /* KIPI_INTERFACE_H */
