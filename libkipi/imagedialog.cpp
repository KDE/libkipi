/* ============================================================
 * File   : imagedialog.cpp
 * Authors: KIPI team developers (see AUTHORS files for details)
 *      
 * Date   : 2004-05
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
#include <qguardedptr.h>
#include <qlabel.h>
#include <qsplitter.h>

#include "imagedialog.h"

#include <kdebug.h>
#include <klistview.h>
#include <klocale.h>
#include <kio/previewjob.h>

#include "imagedialog.moc"


const int PREVIEW_SIZE=128;

namespace KIPI
{


struct AlbumLVI : public KListViewItem {
    AlbumLVI(KListView* parent, const KIPI::ImageCollection& album)
    : KListViewItem(parent, album.name())
    , _album(album) {}

    const KIPI::ImageCollection& _album;
};


struct ImageLVI : public KListViewItem {
    ImageLVI(KListView* parent, const KURL& url)
    : KListViewItem(parent, url.fileName()), _url(url) {}

    KURL _url;
};


struct ImageDialog::Private {
    KURL _url;
    KURL::List _urls;
    KIPI::Interface* _interface;
    KListView* _albumList;
    KListView* _imageList;
    QLabel* _preview;
    QValueList<ImageCollection> _albums;
    bool _singleSelection;
};


ImageDialog::ImageDialog(QWidget* parent, KIPI::Interface* interface,
                         bool singleSelection)
    : KDialogBase(parent, "album-dialog", true, i18n("Select Image From Album"),
                  KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, true)
{
    d=new Private;
    d->_interface=interface;
    d->_singleSelection = singleSelection;
    
    QSplitter* splitter=new QSplitter(this);
    setMainWidget(splitter);
    d->_albumList=new KListView(splitter);
    d->_albumList->addColumn(i18n("Album Name"));
    d->_albumList->setMinimumWidth(200);
    d->_albumList->setResizeMode(QListView::LastColumn);

    d->_imageList=new KListView(splitter);
    d->_imageList->addColumn(i18n("Image Name"));
    d->_imageList->setMinimumWidth(200);
    d->_imageList->setSelectionMode(singleSelection ? QListView::Single :
                                    QListView::Extended);
    d->_imageList->setResizeMode(QListView::LastColumn);
    
    d->_preview=new QLabel(splitter);
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

    if (singleSelection)
        connect(d->_imageList, SIGNAL(selectionChanged(QListViewItem*)),
                this, SLOT(slotImageSelected(QListViewItem*)) );
    else
        connect(d->_imageList, SIGNAL(selectionChanged()),
                this, SLOT(slotImagesSelected()));

    enableButtonOK(false);
}


ImageDialog::~ImageDialog() {
    delete d;
}


KURL ImageDialog::url() const {
    return d->_url;
}

KURL::List ImageDialog::urls() const
{
    return d->_urls;    
}

KURL ImageDialog::getImageURL(QWidget* parent, KIPI::Interface* interface) {
    ImageDialog dlg(parent, interface, true);
    if (dlg.exec() == QDialog::Accepted) {
        return dlg.url();
    } else {
        return KURL();
    }
}

KURL::List ImageDialog::getImageURLs(QWidget* parent, Interface* interface)
{
    ImageDialog dlg(parent, interface, false);
    if (dlg.exec() == QDialog::Accepted)
        return dlg.urls();
    else
    {
        KURL::List urls;
        return urls;
    }
}


void ImageDialog::fillImageList(QListViewItem* item) {
    d->_imageList->clear();
    if (!item) return;

    const KIPI::ImageCollection& album=static_cast<AlbumLVI*>(item)->_album;
    KURL::List images=album.images();

    KURL::List::ConstIterator it=images.begin();
    for (;it!=images.end(); ++it) {
        new ImageLVI(d->_imageList, *it);
    }
}


void ImageDialog::slotImageSelected(QListViewItem* item) {
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

void ImageDialog::slotImagesSelected()
{
    d->_url = KURL();
    d->_urls.clear();
    d->_preview->clear();

    QListViewItem* selectedItem = 0;
    QListViewItem* listItem = d->_imageList->firstChild();
    while (listItem)
    {
        if (listItem->isSelected())
        {
            selectedItem = listItem;
            d->_urls.append(static_cast<ImageLVI*>(listItem)->_url);
        }
        listItem = listItem->nextSibling();
    }

    if (!selectedItem)
    {
        enableButtonOK(false);
        d->_preview->setText(i18n("No images selected"));
        d->_url=KURL();
        d->_urls.clear();
        return;
    }

    enableButtonOK(true);
    
    if (d->_urls.count() == 1)
    {
        d->_url = d->_urls.first();

        KIO::PreviewJob* thumbJob = KIO::filePreview(d->_url, PREVIEW_SIZE);
        connect( thumbJob, SIGNAL(gotPreview(const KFileItem*, const QPixmap&)),
                 SLOT(slotGotPreview(const KFileItem* , const QPixmap&)));
    }
    else
    {
        d->_url = d->_urls.first();
        d->_preview->setText(i18n("%1 images selected")
                             .arg(d->_urls.count()));
    }
    
}


void ImageDialog::slotGotPreview(const KFileItem*, const QPixmap& pix) {

    d->_preview->setPixmap(pix);
}


} // namespace
