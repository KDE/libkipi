/* ============================================================
 * File  : interface.h
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-02
 * Description :
 *
 * Copyright 2004 by the KIPI team
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * ============================================================ */

/** @file interface.h */
#ifndef KIPI_INTERFACE_H
#define KIPI_INTERFACE_H

// Qt includes.

#include <qstring.h>
#include <qobject.h>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "libkipi/imagecollection.h"
#include "libkipi/imageinfo.h"
#include "libkipi/libkipi_export.h"

/** @namespace KIPI */
namespace KIPI
{
    enum Features {
        AlbumsHaveComments         = 1 << 0,
        ImagesHasComments          = 1 << 1,
        ImagesHasTime              = 1 << 2,
        SupportsDateRanges         = 1 << 3,
        AcceptNewImages            = 1 << 4,
        ImageTitlesWritable        = 1 << 5,
        AlbumsHaveCategory         = 1 << 6,
        AlbumsHaveCreationDate     = 1 << 7,
        AlbumsUseFirstImagePreview = 1 << 8 
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
        virtual QValueList<ImageCollection> allAlbums() = 0;

        virtual ImageInfo info( const KURL& ) = 0;
        virtual bool addImage( const KURL&, QString& err );
        virtual void delImage( const KURL& );

        virtual void refreshImages( const KURL::List& );

        virtual QString fileExtensions();

        bool hasFeature( KIPI::Features feature );

    protected:
        virtual int features() const = 0;

    private:
        friend class PluginLoader;
        bool hasFeature( const QString& feature );

    signals:
        void selectionChanged( bool hasSelection );
        void currentAlbumChanged( bool anyAlbum );
    };
}

#endif /* KIPI_INTERFACE_H */
