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


namespace KIPI
{

    // When writing the plugin howto, I'm sure I found a very good reason why this must inherit QObject
    // I just can't remember which -- 13 Mar. 2004 20:12 -- Jesper K. Pedersen
	// I would say this is needed to be able to emit signals -- 16 Mar. 2004 -- Aurélien Gâteau
    class Interface : public QObject
    {
        Q_OBJECT

    public:
        Interface(QObject *parent, const char *name=0);
        virtual ~Interface();

        /** List in current album say 500 images of Jesper */
        virtual ImageCollection* currentAlbum() = 0;

        /** Current set of images in the thumbnail viewer - the first 150 images of Jesper */
        virtual ImageCollection* currentView() { return currentAlbum(); }

        /** current selection in the thumbnail viewer - 5 images selected e.g. */
        virtual ImageCollection* currentSelection() = 0;

        /** list of albums, in digikam this would be all the albums, in KimDaBa this would not make any sence */
        virtual QValueList<ImageCollection*> allAlbums() = 0;

        /** Application keeps owner ship of the returned pointer, will only be valid till next call of this method */
        virtual ImageInfo info( const KURL& ) = 0;


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
