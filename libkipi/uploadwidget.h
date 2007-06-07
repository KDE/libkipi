/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-02-01
 * Description : a widget to upload item.
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

#ifndef KIPI_UPLOADWIDGET_H
#define KIPI_UPLOADWIDGET_H

// Qt includes.

#include <QWidget>

// KDE includes.

#include <k3filetreeview.h>
#include <kurl.h>

// Local includes.

#include "interface.h"
#include "libkipi_export.h"

namespace KIPI
{

    class LIBKIPI_EXPORT UploadWidget : public QWidget 
    {
        Q_OBJECT

    public:

        UploadWidget( Interface* interface, QWidget* parent, const char* name = 0 );
        ~UploadWidget();
        KUrl path() const;

    public slots:

        void mkdir();

    signals :
    
        void folderItemSelected(const KUrl&);

    protected slots:

        void load();
        void slotAlbumCreated(KIO::Job* job);
        void slotFolderSelected();

    private slots:

        void slotPopulateFinished(K3FileTreeViewItem *);

    private:

        struct Private;
        Private* d;
    };
    
} // namespace KIPI

#endif /* KIPI_UPLOADWIDGET_H */
