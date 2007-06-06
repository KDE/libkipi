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

struct AlbumLVI : public KListViewItem 
{
    AlbumLVI(KListView* parent, const KIPI::ImageCollection& album)
        : KListViewItem(parent, album.name()), _album(album) {}

    const KIPI::ImageCollection& _album;
};


struct ImageLVI : public KListViewItem 
{
    ImageLVI(KListView* parent, const KUrl& url)
        : KListViewItem(parent, url.fileName()), _url(url) {}

    KUrl _url;
};

struct ImageDialog::Private 
{
    KUrl                        _url;
    KUrl::List                  _urls;
    KIPI::Interface*            _interface;
    KListView*                  _albumList;
    KListView*                  _imageList;
    QLabel*                     _preview;
    QValueList<ImageCollection> _albums;
    bool                        _singleSelection;
};

ImageDialog::ImageDialog(QWidget* parent, KIPI::Interface* interface,
                         bool singleSelection)
           : KDialog(KDialogBase::Plain, i18n("Select Image From Album"), Help|Ok|Cancel,
                         Ok, parent, true, true)
{
    d = new Private;
    d->_interface       = interface;
    d->_singleSelection = singleSelection;

    QWidget* box = plainPage();
    QVBoxLayout *dvlay = new QVBoxLayout( box, 6 );

    //---------------------------------------------

    QFrame *headerFrame = new QFrame( box );
    headerFrame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    QHBoxLayout* layout = new QHBoxLayout( headerFrame );
    layout->setMargin( 2 ); // to make sure the frame gets displayed
    layout->setSpacing( 0 );
    QLabel *pixmapLabelLeft = new QLabel( headerFrame, "pixmapLabelLeft" );
    pixmapLabelLeft->setScaledContents( false );
    layout->addWidget( pixmapLabelLeft );
    QLabel *labelTitle = new QLabel( i18n("Select Image From Album"), headerFrame, "labelTitle" );
    layout->addWidget( labelTitle );
    layout->setStretchFactor( labelTitle, 1 );
    dvlay->addWidget( headerFrame );

    QString directory;
    KGlobal::dirs()->addResourceType("kipi_banner_left", KGlobal::dirs()->kde_default("data") + "kipi/data");
    directory = KGlobal::dirs()->findResourceDir("kipi_banner_left", "banner_left.png");

    pixmapLabelLeft->setPaletteBackgroundColor( QColor(201, 208, 255) );
    pixmapLabelLeft->setPixmap( QPixmap( directory + "banner_left.png" ) );
    labelTitle->setPaletteBackgroundColor( QColor(201, 208, 255) );

    //---------------------------------------------

    QSplitter* splitter = new QSplitter(box);

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

    dvlay->addWidget( splitter );

    d->_albums=d->_interface->allAlbums();
    QValueList<ImageCollection>::ConstIterator it=d->_albums.begin();

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

    enableButtonOK(false);
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

KUrl ImageDialog::getImageURL(QWidget* parent, KIPI::Interface* interface) 
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

void ImageDialog::fillImageList(QListViewItem* item) 
{
    d->_imageList->clear();
    if (!item) return;

    const ImageCollection& album=static_cast<AlbumLVI*>(item)->_album;
    KUrl::List images=album.images();

    KUrl::List::ConstIterator it=images.begin();
    for (;it!=images.end(); ++it) 
    {
        new ImageLVI(d->_imageList, *it);
    }
}

void ImageDialog::slotImageSelected(QListViewItem* item) 
{
    if (!item) 
    {
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
    d->_url = KUrl();
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
        d->_url=KUrl();
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
        d->_preview->setText(i18n("1 image selected", "%n images selected", d->_urls.count()));
    }
}

void ImageDialog::slotGotPreview(const KFileItem*, const QPixmap& pix) 
{
    d->_preview->setPixmap(pix);
}

void ImageDialog::slotInitialShow()
{
    ImageCollection current = d->_interface->currentAlbum();

    QListViewItemIterator it( d->_albumList );
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

