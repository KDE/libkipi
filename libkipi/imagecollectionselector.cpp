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

// Qt includes.

#include <qheader.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qvgroupbox.h>
#include <qtimer.h>

// KDE includes.

#include <kbuttonbox.h>
#include <kdialog.h>
#include <klistview.h>
#include <klocale.h>
#include <kglobal.h>
#include <kio/previewjob.h>

// KIPI includes.

#include "libkipi/interface.h"

// Local includes.

#include "imagecollectionselector.h"

namespace KIPI 
{

class ImageCollectionItem : public QCheckListItem
{
public:
    ImageCollectionItem(ImageCollectionSelector* selector,
                        QListView * parent, ImageCollection collection)
    : QCheckListItem( parent, collection.name(), QCheckListItem::CheckBox),
      _imageCollection(collection), _selector(selector)
    {}

    ImageCollection imageCollection() const { return _imageCollection; }

protected:

    virtual void stateChange(bool val)
    {
        QCheckListItem::stateChange(val);
        _selector->emitSelectionChanged();
    }
    
private:
    
    ImageCollection          _imageCollection;
    ImageCollectionSelector* _selector;
};

struct ImageCollectionSelector::Private {
    Interface* _interface;
    KListView* _list;
    QLabel*    _thumbLabel;
    QLabel*    _textLabel;
    QListViewItem* _itemToSelect;
};


ImageCollectionSelector::ImageCollectionSelector(QWidget* parent, Interface* interface, const char* name)
                       : QWidget(parent, name)
{
    d=new Private;
    d->_interface=interface;
    d->_itemToSelect = 0;
    
    d->_list=new KListView(this);
    d->_list->setResizeMode( QListView::LastColumn );
    d->_list->addColumn("");
    d->_list->header()->hide();

    connect(d->_list, SIGNAL(selectionChanged(QListViewItem*)),
            SLOT(slotSelectionChanged(QListViewItem*)));
    
    QHBoxLayout* mainLayout=new QHBoxLayout(this, 0, KDialog::spacingHint());
    mainLayout->addWidget(d->_list);

    QVBoxLayout* rightLayout = new QVBoxLayout(mainLayout, 0);

    KButtonBox* box=new KButtonBox(this, Vertical);
    rightLayout->addWidget(box);
    QPushButton* selectAll=box->addButton(i18n("Select All"));
    QPushButton* invertSelection=box->addButton(i18n("Invert Selection"));
    QPushButton* selectNone=box->addButton(i18n("Select None"));
    box->layout();

    connect(selectAll, SIGNAL(clicked()),
            this, SLOT(slotSelectAll()) );
    connect(invertSelection, SIGNAL(clicked()),
            this, SLOT(slotInvertSelection()) );
    connect(selectNone, SIGNAL(clicked()), 
            this, SLOT(slotSelectNone()) );

    rightLayout->addItem(new QSpacerItem(10,20,QSizePolicy::Fixed,
                                         QSizePolicy::Expanding));
    
    QVGroupBox* rightBox = new QVGroupBox(this);
    rightBox->setInsideMargin(KDialog::marginHint());
    rightBox->setInsideSpacing(KDialog::spacingHint());
    rightLayout->addWidget(rightBox);

    if (interface->hasFeature(AlbumsUseFirstImagePreview))
    {
        d->_thumbLabel = new QLabel(rightBox);
        d->_thumbLabel->setFixedSize(QSize(128,128));
        d->_thumbLabel->setAlignment(AlignHCenter | AlignVCenter);
    }
    else
    {
        d->_thumbLabel = 0;
    }
    d->_textLabel = new QLabel(rightBox);
    
    fillList();
    QTimer::singleShot(0, this, SLOT(slotInitialShow()));
}


ImageCollectionSelector::~ImageCollectionSelector() {
    delete d;
}


void ImageCollectionSelector::fillList() {
    QValueList<ImageCollection> collections = d->_interface->allAlbums();
    d->_list->clear();
    ImageCollection current = d->_interface->currentAlbum();
    bool currentWasInList = false;

    /* note: the extensive use of blocksignals is to prevent bombarding
       the plugin with too many selection changed signals. do not remove
       them */
    
    blockSignals(true);
    for( QValueList<ImageCollection>::Iterator it = collections.begin() ;
         it != collections.end() ; ++it )
    {
        ImageCollectionItem* item = new ImageCollectionItem( this, d->_list, *it);
        if (!currentWasInList && *it == current) {
            item->setOn(true);
            currentWasInList = true;
            if (!d->_itemToSelect)
                d->_itemToSelect = item;
        }
    }

    if (!currentWasInList) {
        slotSelectAll();
        d->_itemToSelect = d->_list->firstChild();
    }
    blockSignals(false);
}

void ImageCollectionSelector::emitSelectionChanged()
{
    emit selectionChanged();
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

    /* note: the extensive use of blocksignals is to prevent bombarding
       the plugin with too many selection changed signals. do not remove
       them */
    blockSignals(true);
    for (; it.current(); ++it) {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(true);
    }
    blockSignals(false);
    
    emit selectionChanged();
}


void ImageCollectionSelector::slotInvertSelection() {
    QListViewItemIterator it( d->_list );

    /* note: the extensive use of blocksignals is to prevent bombarding
       the plugin with too many selection changed signals. do not remove
       them */
    blockSignals(true);
    for (; it.current(); ++it) {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(!item->isOn());
    }
    blockSignals(false);

    emit selectionChanged();
}


void ImageCollectionSelector::slotSelectNone() {
    QListViewItemIterator it( d->_list );

    /* note: the extensive use of blocksignals is to prevent bombarding
       the plugin with too many selection changed signals. do not remove
       them */
    blockSignals(true);
    for (; it.current(); ++it) {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(false);
    }
    blockSignals(false);
    
    emit selectionChanged();
}

void ImageCollectionSelector::slotSelectionChanged(QListViewItem* listItem)
{
    if (d->_thumbLabel)
        d->_thumbLabel->clear();
    d->_textLabel->clear();

    if (!listItem)
        return;

    ImageCollectionItem* imcollItem =
        static_cast<ImageCollectionItem*>(listItem);

    if (d->_thumbLabel)
    {
        KURL::List images(imcollItem->imageCollection().images());
        if (!images.isEmpty())
        {
            KIO::PreviewJob* thumbJob = KIO::filePreview(images.first(), 128);
            connect( thumbJob, SIGNAL(gotPreview(const KFileItem*, const QPixmap&)),
                     SLOT(slotGotPreview(const KFileItem* , const QPixmap&)));
        }
    }
    
    // Layout the ImageCollection information nicely
    
    QString cellBeg("<tr><td><nobr><font size=-1><i>");
    QString cellMid("</i></font></nobr></td><td><font size=-1>");
    QString cellEnd("</font></td></tr>");

    QString text("<table cellspacing=0 cellpadding=0>");

    // number of images 
    text += cellBeg + i18n("Images:") +
            cellMid + QString::number(imcollItem->imageCollection().images().count()) +
            cellEnd;

    // Optional features -------------------------------------------------------
    
    // Album Comments
    if (d->_interface->hasFeature(AlbumsHaveComments))
    {
        // Limit the comments string to 20 char...
        QString comments = imcollItem->imageCollection().comment();
	if (!comments.isEmpty())
	{
	comments.truncate(20);
	comments.append("...");
	}
	
        text += cellBeg + i18n("Comments:") +
                cellMid + comments +
                cellEnd;
    }

    // Album Category
    if (d->_interface->hasFeature(AlbumsHaveCategory))
    {
        text += cellBeg + i18n("Category:") +
                cellMid + imcollItem->imageCollection().category() +
                cellEnd;
    }

    // Album Creation Date
    if (d->_interface->hasFeature(AlbumsHaveCreationDate))
    {
        QDate date(imcollItem->imageCollection().date());
        text += cellBeg + i18n("Date:") +
                cellMid + KGlobal::locale()->formatDate(date) +
                cellEnd;
    }

    
    text += "</table>";

    d->_textLabel->setText(text);
    
    emit selectionChanged();
}

void ImageCollectionSelector::slotGotPreview(const KFileItem*, const QPixmap& pix)
{
    d->_thumbLabel->setPixmap(pix);
}

void ImageCollectionSelector::slotInitialShow()
{
    if (d->_itemToSelect)
    {
        d->_list->setSelected(d->_itemToSelect, true);
        d->_list->ensureItemVisible(d->_itemToSelect);
        d->_itemToSelect = 0;
    }
    emit selectionChanged();
}

} // KIPI

#include "imagecollectionselector.moc"
