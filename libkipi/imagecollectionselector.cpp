/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-07-01
 * Description : image collection selector
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

#include <Q3Header>
#include <QLayout>
#include <QPushButton>
#include <QList>
#include <QLabel>
#include <Q3VGroupBox>
#include <QTimer>

// KDE includes.

#include <kdialog.h>
#include <k3listview.h>
#include <k3buttonbox.h>
#include <klocale.h>
#include <kglobal.h>
#include <kio/previewjob.h>

// Local includes.

#include "interface.h"
#include "imagecollectionselector.h"
#include "imagecollectionselector.moc"

namespace KIPI 
{

class ImageCollectionItem : public Q3CheckListItem
{
public:

    ImageCollectionItem(ImageCollectionSelector* selector,
                        Q3ListView* parent, ImageCollection collection)
        : Q3CheckListItem( parent, collection.name(), Q3CheckListItem::CheckBox),
          _imageCollection(collection), _selector(selector)
    {}

    ImageCollection imageCollection() const { return _imageCollection; }

protected:

    virtual void stateChange(bool val)
    {
        Q3CheckListItem::stateChange(val);
        _selector->emitSelectionChanged();
    }

private:

    ImageCollection          _imageCollection;
    ImageCollectionSelector* _selector;
};

struct ImageCollectionSelector::Private 
{
    Interface*      _interface;
    K3ListView*     _list;
    QLabel*         _thumbLabel;
    QLabel*         _textLabel;
    Q3ListViewItem* _itemToSelect;
};

ImageCollectionSelector::ImageCollectionSelector(QWidget* parent, Interface* interface, const char* name)
                       : QWidget(parent)
{
    setObjectName(name);
    d = new Private;
    d->_interface=interface;
    d->_itemToSelect = 0;

    d->_list=new K3ListView(this);
    d->_list->setResizeMode( Q3ListView::LastColumn );
    d->_list->addColumn("");
    d->_list->header()->hide();

    connect(d->_list, SIGNAL(selectionChanged(Q3ListViewItem*)),
            SLOT(slotSelectionChanged(Q3ListViewItem*)));

    QHBoxLayout* mainLayout=new QHBoxLayout(this);
    mainLayout->setSpacing(KDialog::spacingHint());
    mainLayout->addWidget(d->_list);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    mainLayout->addLayout(rightLayout);

    K3ButtonBox* box = new K3ButtonBox(this, Qt::Vertical);
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

    rightLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Expanding));

    Q3VGroupBox* rightBox = new Q3VGroupBox(this);
    rightBox->setInsideMargin(KDialog::marginHint());
    rightBox->setInsideSpacing(KDialog::spacingHint());
    rightLayout->addWidget(rightBox);

    if (interface->hasFeature(AlbumsUseFirstImagePreview))
    {
        d->_thumbLabel = new QLabel(rightBox);
        d->_thumbLabel->setFixedSize(QSize(128,128));
        d->_thumbLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    else
    {
        d->_thumbLabel = 0;
    }
    d->_textLabel = new QLabel(rightBox);

    fillList();
    QTimer::singleShot(0, this, SLOT(slotInitialShow()));
}

ImageCollectionSelector::~ImageCollectionSelector() 
{
    delete d;
}

void ImageCollectionSelector::fillList() 
{
    QList<ImageCollection> collections = d->_interface->allAlbums();
    d->_list->clear();
    ImageCollection current = d->_interface->currentAlbum();
    bool currentWasInList = false;

    /* note: the extensive use of blocksignals is to prevent bombarding
       the plugin with too many selection changed signals. do not remove
       them */

    blockSignals(true);
    for( QList<ImageCollection>::Iterator it = collections.begin() ;
         it != collections.end() ; ++it )
    {
        ImageCollectionItem* item = new ImageCollectionItem( this, d->_list, *it);
        if (!currentWasInList && *it == current) 
        {
            item->setOn(true);
            currentWasInList = true;
            if (!d->_itemToSelect)
                d->_itemToSelect = item;
        }
    }

    if (!currentWasInList) 
    {
        slotSelectAll();
        d->_itemToSelect = d->_list->firstChild();
    }
    blockSignals(false);
}

void ImageCollectionSelector::emitSelectionChanged()
{
    emit selectionChanged();
}

QList<ImageCollection> ImageCollectionSelector::selectedImageCollections() const 
{
    QList<ImageCollection> list;

    Q3ListViewItemIterator it( d->_list );

    for (; it.current(); ++it) 
    {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );

        if (item->isOn()) 
        {
            list << item->imageCollection();
        }
    }

    return list;
}

void ImageCollectionSelector::slotSelectAll() 
{
    Q3ListViewItemIterator it( d->_list );

    /* note: the extensive use of blocksignals is to prevent bombarding
       the plugin with too many selection changed signals. do not remove
       them */
    blockSignals(true);
    for (; it.current(); ++it) 
    {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(true);
    }
    blockSignals(false);

    emit selectionChanged();
}

void ImageCollectionSelector::slotInvertSelection() 
{
    Q3ListViewItemIterator it( d->_list );

    /* note: the extensive use of blocksignals is to prevent bombarding
       the plugin with too many selection changed signals. do not remove
       them */
    blockSignals(true);
    for (; it.current(); ++it) 
    {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(!item->isOn());
    }
    blockSignals(false);

    emit selectionChanged();
}

void ImageCollectionSelector::slotSelectNone() 
{
    Q3ListViewItemIterator it( d->_list );

    /* note: the extensive use of blocksignals is to prevent bombarding
       the plugin with too many selection changed signals. do not remove
       them */
    blockSignals(true);
    for (; it.current(); ++it) 
    {
        ImageCollectionItem *item = static_cast<ImageCollectionItem*>( it.current() );
        item->setOn(false);
    }
    blockSignals(false);

    emit selectionChanged();
}

void ImageCollectionSelector::slotSelectionChanged(Q3ListViewItem* listItem)
{
    if (d->_thumbLabel)
        d->_thumbLabel->clear();
    d->_textLabel->clear();

    if (!listItem)
        return;

    ImageCollectionItem* imcollItem = static_cast<ImageCollectionItem*>(listItem);

    if (d->_thumbLabel)
    {
        KUrl::List images(imcollItem->imageCollection().images());
        if (!images.isEmpty())
        {
            KIO::PreviewJob* thumbJob = KIO::filePreview(images.first(), 128);
            connect( thumbJob, SIGNAL(gotPreview(const K3FileItem*, const QPixmap&)),
                     SLOT(slotGotPreview(const K3FileItem* , const QPixmap&)));
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

void ImageCollectionSelector::slotGotPreview(const K3FileItem*, const QPixmap& pix)
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

} // namespace KIPI
