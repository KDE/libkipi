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
 * Public License as published bythe Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * ============================================================ */

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

namespace KIPI
{
    enum Features {
        AlbumsHaveComments         = 0x0001,
        ImagesHasComments          = 0x0002,
        ImagesHasTime              = 0x0004,
        SupportsDateRanges         = 0x0008,
        AcceptNewImages            = 0x0010,
        ImageTitlesWritable        = 0x0020,
        AlbumsHaveCategory         = 0x0040,
        AlbumsHaveCreationDate     = 0x0060,
        AlbumsUseFirstImagePreview = 0x0080
        // When adding new items, remember to update "hasFeature( const QString& feature )"
        // and the hello world plugin.
    };

    class Interface : public QObject
    {
        Q_OBJECT

    public:
        Interface(QObject *parent, const char *name=0);
        virtual ~Interface();

        virtual ImageCollection currentAlbum() = 0;
        virtual ImageCollection currentSelection() = 0;
        virtual ImageCollection currentScope();
        virtual QValueList<ImageCollection> allAlbums() = 0;

        virtual ImageInfo info( const KURL& ) = 0;
        virtual bool addImage( const KURL&, QString& err );
        virtual void delImage( const KURL& );

        virtual void refreshImages( const KURL::List& );

        virtual QString fileExtensions();

        bool hasFeature( KIPI::Features feature );

    protected:
        virtual int features() const = 0;

    protected slots:
        void stateChange();


    private:
        friend class PluginLoader;
        bool hasFeature( const QString& feature );

    signals:
        void selectionChanged( bool hasSelection );
        void currentAlbumChanged( bool anyAlbum );
        void currentScopeChanged( bool asScope );
    };
}

#endif /* KIPI_INTERFACE_H */
