/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-05-01
 * Description : an image files selector dialog.
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
 
#include <QPointer>
#include <QLabel>
#include <QSplitter>
#include <QLayout>
#include <QFrame>
#include <QPushButton>
#include <QTimer>

// KDE includes.

#include <kvbox.h>
#include <kdebug.h>
#include <k3listview.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kio/previewjob.h>
#include <kiconloader.h>

// Local includes.

#include "version.h"
#include "imagedialog.h"
#include "imagedialog.moc"

const int PREVIEW_SIZE = 128;

namespace KIPI
{

struct AlbumLVI : public K3ListViewItem 
{
    AlbumLVI(K3ListView* parent, const ImageCollection& album)
        : K3ListViewItem(parent, album.name()), _album(album) {}

    const ImageCollection& _album;
};


struct ImageLVI : public K3ListViewItem 
{
    ImageLVI(K3ListView* parent, const KUrl& url)
        : K3ListViewItem(parent, url.fileName()), _url(url) {}

    KUrl _url;
};

struct ImageDialog::Private 
{
    KUrl                   _url;
    KUrl::List             _urls;
    KIPI::Interface*       _interface;
    K3ListView*            _albumList;
    K3ListView*            _imageList;
    QLabel*                _preview;
    QList<ImageCollection> _albums;
    bool                   _singleSelection;
};

ImageDialog::ImageDialog(QWidget* parent, KIPI::Interface* interface,
                         bool singleSelection)
           : KDialog(parent)
{
    d = new Private;
    d->_interface       = interface;
    d->_singleSelection = singleSelection;

    setCaption(i18n("Select Image From Album"));
    setButtons(KDialog::Help | KDialog::Ok | KDialog::Cancel);

    KVBox *box = new KVBox( this );
    setMainWidget( box );
    QVBoxLayout *dvlay = new QVBoxLayout( box );

    //---------------------------------------------

    QSplitter* splitter = new QSplitter(box);

    d->_albumList = new K3ListView(splitter);
    d->_albumList->addColumn(i18n("Album Name"));
    d->_albumList->setMinimumWidth(200);
    d->_albumList->setResizeMode(Q3ListView::LastColumn);

    d->_imageList = new K3ListView(splitter);
    d->_imageList->addColumn(i18n("Image Name"));
    d->_imageList->setMinimumWidth(200);
    d->_imageList->setSelectionMode(singleSelection ? Q3ListView::Single :
                                    Q3ListView::Extended);
    d->_imageList->setResizeMode(Q3ListView::LastColumn);

    d->_preview = new QLabel(splitter);
    d->_preview->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    d->_preview->setFixedWidth(PREVIEW_SIZE);
    d->_preview->setText(i18n("No image selected"));

    dvlay->addWidget( splitter );

    d->_albums = d->_interface->allAlbums();
    QList<ImageCollection>::ConstIterator it=d->_albums.begin();

    for(; it!=d->_albums.end(); ++it) 
    {
        new AlbumLVI(d->_albumList, *it);
    }
    QTimer::singleShot(0, this, SLOT(slotInitialShow()));

    connect(d->_albumList, SIGNAL(selectionChanged(QListViewItem*)),
            this, SLOT(fillImageList(QListViewItem*)) );

    if (singleSelection)
        connect(d->_imageList, SIGNAL(selectionChanged(QListViewItem*)),
                this, SLOT(slotImageSelected(QListViewItem*)) );
    else
        connect(d->_imageList, SIGNAL(selectionChanged()),
                this, SLOT(slotImagesSelected()));

    enableButtonOk(false);
}

ImageDialog::~ImageDialog() 
{
    delete d;
}

KUrl ImageDialog::url() const 
{
    return d->_url;
}

KUrl::List ImageDialog::urls() const
{
    return d->_urls;
}

KUrl ImageDialog::getImageURL(QWidget* parent, Interface* interface) 
{
    ImageDialog dlg(parent, interface, true);
    if (dlg.exec() == QDialog::Accepted) 
    {
        return dlg.url();
    }
    else 
    {
        return KUrl();
    }
}

KUrl::List ImageDialog::getImageURLs(QWidget* parent, Interface* interface)
{
    ImageDialog dlg(parent, interface, false);
    if (dlg.exec() == QDialog::Accepted)
        return dlg.urls();
    else
    {
        KUrl::List urls;
        return urls;
    }
}

void ImageDialog::fillImageList(Q3ListViewItem* item) 
{
    d->_imageList->clear();
    if (!item) return;

    const ImageCollection& album = static_cast<AlbumLVI*>(item)->_album;
    KUrl::List images=album.images();

    KUrl::List::ConstIterator it=images.begin();
    for (;it!=images.end(); ++it) 
    {
        new ImageLVI(d->_imageList, *it);
    }
}

void ImageDialog::slotImageSelected(Q3ListViewItem* item) 
{
    if (!item) 
    {
        enableButtonOk(false);
        d->_preview->setText(i18n("No image selected"));
        d->_url=KUrl();
        return;
    }
    enableButtonOk(true);
    d->_url = static_cast<ImageLVI*>(item)->_url;
    d->_preview->clear();

    KIO::PreviewJob* thumbJob = KIO::filePreview(d->_url, PREVIEW_SIZE);
    connect( thumbJob, SIGNAL(gotPreview(const K3FileItem*, const QPixmap&)),
             SLOT(slotGotPreview(const K3FileItem* , const QPixmap&)));
}

void ImageDialog::slotImagesSelected()
{
    d->_url = KUrl();
    d->_urls.clear();
    d->_preview->clear();

    Q3ListViewItem* selectedItem = 0;
    Q3ListViewItem* listItem = d->_imageList->firstChild();
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
        enableButtonOk(false);
        d->_preview->setText(i18n("No images selected"));
        d->_url=KUrl();
        d->_urls.clear();
        return;
    }

    enableButtonOk(true);

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
        d->_preview->setText(i18np("1 image selected", "%1 images selected", d->_urls.count()));
    }
}

void ImageDialog::slotGotPreview(const K3FileItem*, const QPixmap& pix) 
{
    d->_preview->setPixmap(pix);
}

void ImageDialog::slotInitialShow()
{
    ImageCollection current = d->_interface->currentAlbum();

    Q3ListViewItemIterator it( d->_albumList );
    while ( it.current() )
    {
        AlbumLVI* lvi = static_cast<AlbumLVI*>( it.current() );
        if ( lvi->_album == current )
        {
            d->_albumList->ensureItemVisible( lvi );
            d->_albumList->setSelected( lvi, true );
            break;
        }
        ++it;
    }
}

} // namespace KIPI

