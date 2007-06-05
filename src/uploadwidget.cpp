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

// Qt includes. 

#include <qlayout.h>
#include <qheader.h>
#include <qlistview.h>
#include <qdir.h>

// KDE includes

#include <kdebug.h>
#include <klocale.h>
#include <kio/jobclasses.h>
#include <kmessagebox.h>

#include <kdeversion.h>
#if KDE_VERSION >= 0x30200
#include <kinputdialog.h>
#else
#include <klineeditdlg.h>
#define KInputDialog KLineEditDlg
#endif

// Local includes.

#include "libkipi/imagecollection.h"
#include "uploadwidget.h"
#include "uploadwidget.moc"

struct KIPI::UploadWidget::Private
{
    KFileTreeView* m_treeView;
    KFileTreeBranch* m_branch;
    QStringList m_pendingPath;
};

/*!
  \class KIPI::UploadWidget
  This widget is used to specify an upload directory for new images.
*/
KIPI::UploadWidget::UploadWidget( KIPI::Interface* interface, QWidget* parent, const char* name )
                  : QWidget( parent, name )
{
    d = new Private;

    QVBoxLayout* layout = new QVBoxLayout( this, 0 );
    d->m_treeView = new KFileTreeView( this );
    d->m_treeView->setRootIsDecorated( true );
    layout->addWidget( d->m_treeView );

    // Fetch the current album, so we can start out there.
    KIPI::ImageCollection album = interface->currentAlbum();

    // If no current album selected, get the first album in the list.
    if ( !album.isValid() || !album.isDirectory() ) 
       album = interface->allAlbums().first();

    d->m_branch = d->m_treeView->addBranch( QDir::cleanDirPath(album.uploadRoot().path()),
                                          album.uploadRootName() );
    d->m_treeView->setDirOnlyMode( d->m_branch, true );

    d->m_treeView->addColumn( i18n("Folder" ) );

    d->m_treeView->header()->setStretchEnabled( true, 0 );
    d->m_treeView->header()->hide();

    QString root = album.uploadRoot().path();
    QString uploadPath = album.isDirectory() ? album.uploadPath().path() : root;

    root       = QDir::cleanDirPath(root);
    uploadPath = QDir::cleanDirPath(uploadPath);

    if ( !uploadPath.startsWith( root ) ) 
    {
        kdWarning(51000) << "Error in Host application: uploadPath() should start with uploadRoot()." << endl
                         << "uploadPath() = " << album.uploadPath().prettyURL() << endl
                         << "uploadRoot() = " << album.uploadRoot().prettyURL() << endl;
    }
    else
    {
        uploadPath = uploadPath.mid( root.length() );

        d->m_pendingPath = QStringList::split( "/", uploadPath, false );

        connect( d->m_branch, SIGNAL( populateFinished(KFileTreeViewItem *) ),
                 this, SLOT( slotPopulateFinished(KFileTreeViewItem *) ) );

        d->m_branch->setOpen(true);
    }

    connect( d->m_treeView, SIGNAL( executed(QListViewItem *) ),
             this, SLOT( slotFolderSelected(QListViewItem *) ) );
}

KIPI::UploadWidget::~UploadWidget()
{
    delete d;
}

KURL KIPI::UploadWidget::path() const
{
    return d->m_treeView->currentURL();
}

void KIPI::UploadWidget::load( )
{
    kdWarning() << "KIPI::UploadWidget::load(): This method is obsolete\n";
}

void KIPI::UploadWidget::slotPopulateFinished( KFileTreeViewItem * parentItem )
{
    if ( d->m_pendingPath.isEmpty() ) 
    {
        disconnect( d->m_branch, SIGNAL( populateFinished(KFileTreeViewItem *) ), 
                    this, SLOT( slotPopulateFinished(KFileTreeViewItem *) ) );
        return;
    }

    QString itemName = d->m_pendingPath.front();

    d->m_pendingPath.pop_front();

    QListViewItem * item;
    for ( item = parentItem->firstChild(); item; item = item->nextSibling() )
    {
        if ( item->text(0) == itemName )
        {
            break;
        }
    }

    if ( !item ) 
    {
        kdDebug( 51000 ) << "Unable to open " << itemName << endl;
    }
    else
    {
        item->setOpen( true );
        d->m_treeView->setSelected( item, true );
        d->m_treeView->ensureItemVisible ( item );

        KFileTreeViewItem * ftvItem = static_cast<KFileTreeViewItem *>( item );
        if ( ftvItem->alreadyListed() )
            slotPopulateFinished( ftvItem );
    }
}

void KIPI::UploadWidget::mkdir()
{
    if ( !path().isValid() ) 
    {
        KMessageBox::error( this, i18n("Please select a directory first.") );
        return;
    }

    bool ok;
    QString dir = KInputDialog::getText( i18n("Create Directory"),
                                         i18n("<qt>Enter new directory name (to be created as subdir of %1):</qt>")
                                        .arg(path().prettyURL()), "", &ok, this);

    if (!ok) return;

    KURL url = path();
    url.addPath( dir );

    KIO::SimpleJob* job = KIO::mkdir(url);

    connect(job, SIGNAL(result(KIO::Job*)), 
            this, SLOT(slotAlbumCreated(KIO::Job*)));
}

void KIPI::UploadWidget::slotAlbumCreated(KIO::Job* job)
{
    int code = job->error();

    if ( code )
        job->showErrorDialog( this );
}

void KIPI::UploadWidget::slotFolderSelected(QListViewItem *)
{
    emit folderItemSelected(d->m_treeView->currentURL());
}
