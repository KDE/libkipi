#ifndef TESTIMAGECOLLECTIONSELECTORWINDOW_H
#define TESTIMAGECOLLECTIONSELECTORWINDOW_H

class TestWindow : public QVBox {
Q_OBJECT
public:
    TestWindow(KIPI::Interface* interface)
    : QVBox(0) {
        _selector=new KIPI::ImageCollectionSelector(this, interface);
        QPushButton* button=new QPushButton("Selected image collections", this);
        connect(button, SIGNAL(clicked()), this, SLOT(slotDumpImageCollections()) );
    }

public slots:
    void slotDumpImageCollections() {
        kdDebug() << "Selected image collections:" << endl;
        QValueList<KIPI::ImageCollection> list=_selector->selectedImageCollections();
        QValueList<KIPI::ImageCollection>::ConstIterator it=list.begin();
        for( ; it!=list.end(); ++it) {
            kdDebug() << "- " << (*it).name() << endl;
        }
    }

private:
    KIPI::ImageCollectionSelector* _selector;
};

#endif // TESTIMAGECOLLECTIONSELECTORWINDOW_H
