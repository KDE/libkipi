#include <qguardedptr.h>
#include <qhbox.h>
#include <qlabel.h>

#include <kdebug.h>
#include <klistview.h>
#include <klocale.h>
#include <kio/previewjob.h>

#include "imagechooser.moc"


const int PREVIEW_SIZE=128;

namespace KIPI
{


struct AlbumLVI : public KListViewItem {
    AlbumLVI(KListView* parent, const KIPI::ImageCollection& album)
    : KListViewItem(parent, album.name(), QString::number(album.images().size()))
    , _album(album) {}

    const KIPI::ImageCollection& _album;
};


struct ImageLVI : public KListViewItem {
    ImageLVI(KListView* parent, const KURL& url)
    : KListViewItem(parent, url.fileName()), _url(url) {}

    KURL _url;
};


struct ImageChooser::Private {
    KURL _url;
    KIPI::Interface* _interface;
    KListView* _albumList;
    KListView* _imageList;
    QLabel* _preview;
    QValueList<ImageCollection> _albums;
};


ImageChooser::ImageChooser(QWidget* parent, KIPI::Interface* interface)
    : KDialogBase(parent, "album-dialog", true, i18n("Select an image from an album"),
        KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, true)
{
    d=new Private;
    d->_interface=interface;
    QHBox* box=makeHBoxMainWidget();
    d->_albumList=new KListView(box);
    d->_albumList->addColumn(i18n("Album Name"));
    d->_albumList->addColumn(i18n("Images"));

    d->_imageList=new KListView(box);
    d->_imageList->addColumn(i18n("Image Name"));

    d->_preview=new QLabel(box);
    d->_preview->setAlignment(AlignHCenter | AlignVCenter | WordBreak);
    d->_preview->setFixedWidth(PREVIEW_SIZE);
    d->_preview->setText(i18n("No image selected"));

    d->_albums=d->_interface->allAlbums();
    QValueList<ImageCollection>::ConstIterator it=d->_albums.begin();
    for(; it!=d->_albums.end(); ++it) {
        new AlbumLVI(d->_albumList, *it);
    }

    connect(d->_albumList, SIGNAL(selectionChanged(QListViewItem*)),
        this, SLOT(fillImageList(QListViewItem*)) );

    connect(d->_imageList, SIGNAL(selectionChanged(QListViewItem*)),
        this, SLOT(slotImageSelected(QListViewItem*)) );

    enableButtonOK(false);
}


ImageChooser::~ImageChooser() {
    delete d;
}


KURL ImageChooser::url() const {
    return d->_url;
}


KURL ImageChooser::getImageURL(QWidget* parent, KIPI::Interface* interface) {
    ImageChooser dlg(parent, interface);
    if (dlg.exec()) {
        return dlg.url();
    } else {
        return KURL();
    }
}



void ImageChooser::fillImageList(QListViewItem* item) {
    d->_imageList->clear();
    if (!item) return;

    const KIPI::ImageCollection& album=static_cast<AlbumLVI*>(item)->_album;
    KURL::List images=album.images();

    KURL::List::ConstIterator it=images.begin();
    for (;it!=images.end(); ++it) {
        new ImageLVI(d->_imageList, *it);
    }
}


void ImageChooser::slotImageSelected(QListViewItem* item) {
    if (!item) {
        enableButtonOK(false);
        d->_preview->setText(i18n("No image selected"));
        d->_url=KURL();
        return;
    }
    enableButtonOK(true);
    d->_url=static_cast<ImageLVI*>(item)->_url;
    d->_preview->clear();

    KIO::PreviewJob* thumbJob = KIO::filePreview(d->_url, PREVIEW_SIZE);
    connect( thumbJob, SIGNAL(gotPreview(const KFileItem*, const QPixmap&)),
        SLOT(slotGotPreview(const KFileItem* , const QPixmap&)));
}


void ImageChooser::slotGotPreview(const KFileItem*, const QPixmap& pix) {

    d->_preview->setPixmap(pix);
}


} // namespace
