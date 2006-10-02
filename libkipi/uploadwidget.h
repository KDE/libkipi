/* ============================================================
 * File   : uploadwidget.h
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-02-19
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


#ifndef KIPI_UPLOADWIDGET_H
#define KIPI_UPLOADWIDGET_H

// Qt includes.

#include <qwidget.h>

// KDE includes.

#include <kfiletreeview.h>
#include <kurl.h>

// LibKIPI includes.

#include "libkipi/interface.h"
#include "libkipi/libkipi_export.h"

class QListViewItem;

namespace KIPI
{
    class LIBKIPI_EXPORT UploadWidget :public QWidget 
    {
        Q_OBJECT

    public:
        UploadWidget( KIPI::Interface* interface, QWidget* parent, const char* name = 0 );
        ~UploadWidget();
        KURL path() const;

    public slots:
        void mkdir();

    signals :
        void folderItemSelected(const KURL &url);
        
    protected slots:
        void load();
        void slotAlbumCreated(KIO::Job* job);
        void slotFolderSelected(QListViewItem *);

    private slots:
        void slotPopulateFinished(KFileTreeViewItem *);

    private:
        struct Private;
        Private* d;
    };
}

#endif /* KIPI_UPLOADWIDGET_H */

