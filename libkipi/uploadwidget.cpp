#include "uploadwidget.h"
#include <qlayout.h>
#include "libkipi/imagecollection.h"
#include <klocale.h>
#include <qheader.h>
#include <qapplication.h>
#include <kdebug.h>
#include <kinputdialog.h>
#include <kio/jobclasses.h>
#include <kmessagebox.h>

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
    m_item = m_treeView->addBranch( album.uploadRoot(), i18n("Images") );
    m_treeView->setDirOnlyMode( m_item, true );

    m_treeView->addColumn( i18n("Folder" ) );

    m_treeView->header()->setStretchEnabled( true, 0 );

    QString root = album.uploadRoot().path();
    QString uploadPath = album.uploadPath().path();
    if ( !uploadPath.startsWith( root ) ) {
        kdWarning(51000) << "Error in Host application: uploadPath() should start with uploadRoot()." << endl
                         << "uploadPath() = " << album.uploadPath() << endl
                         << "uploadRoot() = " << album.uploadRoot() << endl;
    }
    else {
        uploadPath = uploadPath.mid( root.length() );
        m_pendingPath = QStringList::split( "/", uploadPath, true );
        if ( !m_pendingPath[0].isEmpty() )
            m_pendingPath.prepend( "" ); // ensure we open the root first.

        load();
        connect( m_item, SIGNAL( populateFinished(KFileTreeViewItem *) ), this, SLOT( load() ) );
    }
}

KURL KIPI::UploadWidget::path() const
{
    return m_treeView->currentURL();
}

void KIPI::UploadWidget::load()
{
    if ( m_pendingPath.isEmpty() ) {
        disconnect( m_item, SIGNAL( populateFinished(KFileTreeViewItem *) ), this, SLOT( load() ) );
        return;
    }

    QString item = m_pendingPath.front();
    m_pendingPath.pop_front();
    m_handled += "/" + item;

    KFileTreeViewItem* branch = m_treeView->findItem( m_item, m_handled );
    if ( !branch ) {
        kdDebug( 51000 ) << "Unable to open " << m_handled << endl;
    }
    else {
        branch->setOpen( true );
        m_treeView->setSelected( branch, true );
        if ( branch->alreadyListed() )
            load();
    }

}

void KIPI::UploadWidget::mkdir()
{
    if ( !path().isValid() ) {
        KMessageBox::error( this, i18n("Please select a directory first") );
        return;
    }

    bool ok;
    QString dir = KInputDialog::getText( i18n("Create directory"),
                                         i18n("<qt>Enter new directory name (to be created as subdir of %1</qt>")
                                        .arg(path().prettyURL()), "", &ok, this);
    if (!ok) return;

    KURL url = path();
    url.addPath( dir );

    KIO::SimpleJob* job = KIO::mkdir(url);
    connect(job, SIGNAL(result(KIO::Job*)), this, SLOT(slotAlbumCreated(KIO::Job*)));
}

void KIPI::UploadWidget::slotAlbumCreated(KIO::Job* job)
{
    int code = job->error();
    if ( code )
        job->showErrorDialog( this );
}

#include "uploadwidget.moc"
