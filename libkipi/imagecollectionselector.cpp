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
    for( QValueList<ImageCollection>::Iterator it = collections.begin() ;
         it != collections.end() ; ++it )
    {
        new ImageCollectionItem( d->_list, *it);
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
