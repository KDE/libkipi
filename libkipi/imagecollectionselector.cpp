/* ============================================================
 * File   : imagecollectionselector.cpp
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-07
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
#include "imagecollectionselector.moc"

// Qt
#include <qheader.h>
#include <qlayout.h>
#include <qpushbutton.h>

// KDE
#include <kbuttonbox.h>
#include <kdialog.h>
#include <klistview.h>
#include <klocale.h>

// KIPI
#include "libkipi/interface.h"

/* Missing features:
 * - Album details:
 *  - comments
 *  - date
 *  - preview
 *  - image count
 */

namespace KIPI {

class ImageCollectionItem : public QCheckListItem
{
public:
    ImageCollectionItem(QListView * parent, ImageCollection collection)
    : QCheckListItem( parent, collection.name(), QCheckListItem::CheckBox),
      _imageCollection(collection)
    {}

    ImageCollection imageCollection() const { return _imageCollection; }

private:
    ImageCollection _imageCollection;
};


struct ImageCollectionSelector::Private {
    Interface* _interface;
    KListView* _list;
};


ImageCollectionSelector::ImageCollectionSelector(QWidget* parent, Interface* interface, const char* name)
: QWidget(parent, name)
{
    d=new Private;
    d->_interface=interface;
    
    d->_list=new KListView(this);
    d->_list->setResizeMode( QListView::LastColumn );
    d->_list->addColumn("");
    d->_list->header()->hide();
    
    QHBoxLayout* mainLayout=new QHBoxLayout(this, 0, KDialog::spacingHint());
    mainLayout->addWidget(d->_list);
    
    KButtonBox* box=new KButtonBox(this, Vertical);
    mainLayout->addWidget(box);
    QPushButton* selectAll=box->addButton(i18n("Select All"));
    QPushButton* invertSelection=box->addButton(i18n("Invert Selection"));
    QPushButton* selectNone=box->addButton(i18n("Select None"));
    box->layout();

    connect(selectAll, SIGNAL(clicked()), this, SLOT(slotSelectAll()) );
    connect(invertSelection, SIGNAL(clicked()), this, SLOT(slotInvertSelection()) );
    connect(selectNone, SIGNAL(clicked()), this, SLOT(slotSelectNone()) );
    
    fillList();
}


ImageCollectionSelector::~ImageCollectionSelector() {
    delete d;
}


void ImageCollectionSelector::fillList() {
    QValueList<ImageCollection> collections = d->_interface->allAlbums();
    d->_list->clear();
    ImageCollection current = d->_interface->currentAlbum();
    bool currentWasInList = false;
    
    for( QValueList<ImageCollection>::Iterator it = collections.begin() ;
         it != collections.end() ; ++it )
    {
        ImageCollectionItem* item = new ImageCollectionItem( d->_list, *it);
        if (!currentWasInList && *it == current) {
            item->setOn(true);
            currentWasInList = true;
        }
    }

    if (!currentWasInList) {
        slotSelectAll();
    }
}

QValueList<ImageCollection> ImageCollectionSelector::selectedImageCollections() const {
    QValueList<ImageCollection> list;

    QListViewItemIterator it( d->_list );

    for (; it.current(); ++it) {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );

        if (item->isOn()) {
            list << item->imageCollection();
        }
    }

    return list;
}

void ImageCollectionSelector::slotSelectAll() {
    QListViewItemIterator it( d->_list );

    for (; it.current(); ++it) {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(true);
    }
}


void ImageCollectionSelector::slotInvertSelection() {
    QListViewItemIterator it( d->_list );

    for (; it.current(); ++it) {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(!item->isOn());
    }
}


void ImageCollectionSelector::slotSelectNone() {
    QListViewItemIterator it( d->_list );

    for (; it.current(); ++it) {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(false);
    }
}

} // KIPI
