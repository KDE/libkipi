/* ============================================================
 * File  : interface.h
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>, Jesper Pedersen <blackie@kde.org>
 * Date  : 2004-02-19
 * Description :
 *
 * Copyright 2004 by Renchi Raju, Jesper Pedersen

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

#ifndef KIPI_INTERFACE_H
#define KIPI_INTERFACE_H

#include <qobject.h>
#include "libkipi/imagecollection.h"
#include "libkipi/imageinfo.h"
#include <kurl.h>


namespace KIPI
{
    enum Features {
        AlbumsHaveComments  = 0x0001,
        AlbumEQDir          = 0x0002,
        ImagesHasComments   = 0x0004,
        ImagesHasTime       = 0x0008,
        SupportsDateRanges  = 0x0010,
        AcceptNewImages     = 0x0020,
        ImageTitlesWritable = 0x0040
    };

    class Interface : public QObject
    {
        Q_OBJECT

    public:
        Interface(QObject *parent, const char *name=0);
        virtual ~Interface();

        /** List in current album say 500 images of Jesper */
        virtual ImageCollection currentAlbum() = 0;

        /** current selection in the thumbnail viewer - 5 images selected e.g. */
        virtual ImageCollection currentSelection() = 0;

        /** list of albums, in digikam this would be all the albums, in KimDaBa this would not make any sence */
        virtual QValueList<ImageCollection> allAlbums() = 0;

        virtual ImageInfo info( const KURL& ) = 0;
        virtual bool addImage( const KURL&, QString& err );
        virtual void delImage( const KURL& );

        /** Tells the host app that the following images has changed on disk */
        virtual void refreshImages( const KURL::List& );

        /** Return a bitwise or if the KIPI::Features that thus application support. */
        virtual int features() const = 0;
        bool hasFeature( KIPI::Features feature );
        bool hasFeature( const QString& feature );

    signals:
        // PENDING(blackie) signals is something we haven't discussed yet.
        //void signalAlbumCurrentChanged(Album* album);
        //void signalItemsSelected(bool val);
        //
        //void signalAlbumAdded(Album* album);
        //void signalAlbumRemoved(Album* album);
        //void signalAlbumsCleared();
    };
}

#endif /* KIPI_INTERFACE_H */
