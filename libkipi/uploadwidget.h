/* ============================================================
 * File   : uploadwidget.h
 * Authors: Jesper K. Pedersen <blackie@kde.org>
 *	    
 * Date   : 2004-02-19
 * Description :
 *
 * Copyright 2004 by Jesper K. Pedersen
 *
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


#ifndef KIPI_UPLOADWIDGET_H
#define KIPI_UPLOADWIDGET_H

// Qt includes.

#include <qwidget.h>

// KDE includes.

#include <kfiletreeview.h>

// LibKIPI includes.

#include <libkipi/interface.h>

namespace KIPI
{
    class UploadWidget :public QWidget {
        Q_OBJECT

    public:
        UploadWidget( KIPI::Interface* interface, QWidget* parent, const char* name = 0 );
        KURL path() const;

    public slots:
        void mkdir();

    protected slots:
        void load();
        void slotAlbumCreated(KIO::Job* job);

    private:
        KFileTreeView* m_treeView;
        KFileTreeBranch* m_item;
        QStringList m_pendingPath;
        QString m_handled;
        KURL m_root;
    };

}

#endif /* KIPI_UPLOADWIDGET_H */

