/* ============================================================
 * File   : uploadwidget.cpp
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-02
 * Description :
 *
 * Copyright 2004 by the KIPI team
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

// Qt includes. 
  
#include <qlayout.h>
#include <qheader.h>
#include <qlistview.h>

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

#include "uploadwidget.h"
#include "libkipi/imagecollection.h"


/*!
  \class KIPI::UploadWidget
  This widget is used to specify an upload directory for new images.
*/

KIPI::UploadWidget::UploadWidget( KIPI::Interface* interface, QWidget* parent, const char* name )
                  : QWidget( parent, name )
{
    QVBoxLayout* layout = new QVBoxLayout( this, 0 );
    m_treeView = new KFileTreeView( this );
    layout->addWidget( m_treeView );

    // Fetch the current album, so we can start out there.
    KIPI::ImageCollection album = interface->currentAlbum();
    
    // If no current album selected, get the first album in the list.
    if ( !album.isValid() ) 
       album = interface->allAlbums().first();
    
    m_item = m_treeView->addBranch( album.uploadRoot(), album.uploadRootName() );
    m_treeView->setDirOnlyMode( m_item, true );

    m_treeView->addColumn( i18n("Folder" ) );

    m_treeView->header()->setStretchEnabled( true, 0 );

    QString root = album.uploadRoot().path();
    QString uploadPath = album.uploadPath().path();
    
    if ( !uploadPath.startsWith( root ) ) 
        {
        kdWarning(51000) << "Error in Host application: uploadPath() should start with uploadRoot()." << endl
                         << "uploadPath() = " << album.uploadPath().prettyURL() << endl
                         << "uploadRoot() = " << album.uploadRoot().prettyURL() << endl;
        }
    else
        {
        uploadPath = uploadPath.mid( root.length() );
        
        m_pendingPath = QStringList::split( "/", uploadPath, true );
        
        if ( !m_pendingPath[0].isEmpty() )
            m_pendingPath.prepend( "" ); // ensure we open the root first.
        
        load();
        
        connect( m_item, SIGNAL( populateFinished(KFileTreeViewItem *) ),
                 this, SLOT( load() ) );
        }
    
    connect( m_treeView, SIGNAL( executed(QListViewItem *) ),
             this, SLOT( slotFolderSelected(QListViewItem *) ) );
}

KURL KIPI::UploadWidget::path() const
{
    return m_treeView->currentURL();
}

void KIPI::UploadWidget::load()
{
    if ( m_pendingPath.isEmpty() ) 
        {
        disconnect( m_item, SIGNAL( populateFinished(KFileTreeViewItem *) ), 
                    this, SLOT( load() ) );
        return;
        }

    QString item = m_pendingPath.front();

    m_pendingPath.pop_front();

    m_handled += "/" + item;
    
    KFileTreeViewItem* branch = m_treeView->findItem( m_item, m_handled );
    
    if ( !branch ) 
        {
        kdDebug( 51000 ) << "Unable to open " << m_handled << endl;
        }
    else
        {
        branch->setOpen( true );
        m_treeView->setSelected( branch, true );
        m_treeView->ensureItemVisible ( branch );
        
        if ( branch->alreadyListed() )
            load();
        }

}

void KIPI::UploadWidget::mkdir()
{
    if ( !path().isValid() ) 
        {
        KMessageBox::error( this, i18n("Please select a directory first") );
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
    emit folderItemSelected(m_treeView->currentURL());
}

#include "uploadwidget.moc"
