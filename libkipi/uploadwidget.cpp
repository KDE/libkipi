#include "uploadwidget.h"
#include <qlayout.h>
#include "libkipi/imagecollection.h"
#include <klocale.h>
#include <qheader.h>

/*!
  \class KIPI::UploadWidget
  This widget is used to specify an upload directory for new images.
*/

KIPI::UploadWidget::UploadWidget( KIPI::Interface* interface, QWidget* parent, const char* name )
    :QWidget( parent, name )
{
    QVBoxLayout* layout = new QVBoxLayout( this, 0 );
    m_treeView = new KFileTreeView( this );
    layout->addWidget( m_treeView );

    // Fetch the current album, so we can start out there.
    KIPI::ImageCollection album = interface->currentAlbum();
    KFileTreeBranch* item = m_treeView->addBranch( album.uploadRoot(), i18n("Images") );
    m_treeView->setDirOnlyMode( item, true );

    m_treeView->addColumn( i18n("Folder" ) );
    item->setOpen( true );

    m_treeView->header()->setStretchEnabled( true, 0 );

#ifdef TEMPORARILY_REMOVED
    qApp->processEvents();
        KFileTreeViewItem* branch = view->findItem( item, "cd1" );
        if ( branch )
            branch->setOpen( true );
        qApp->processEvents();
        branch = view->findItem( item, "cd1/AlbumL9" );
        if ( branch )
            branch->setOpen( true );
        else
            qDebug("shiot");
#endif
}
