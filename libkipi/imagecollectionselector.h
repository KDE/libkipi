#ifndef IMAGECOLLECTIONSELECTOR_H
#define IMAGECOLLECTIONSELECTOR_H   

// Qt
#include <qwidget.h>
#include <qvaluelist.h>

// KIPI
#include "libkipi/imagecollection.h"

namespace KIPI {

class Interface;
    
class ImageCollectionSelector : public QWidget {
Q_OBJECT
    class Private;

public:
    ImageCollectionSelector(QWidget* parent, Interface*, const char* name=0);
    ~ImageCollectionSelector();
    QValueList<ImageCollection> selectedImageCollections() const;
    
private:
    Private* d;
    void fillList();

private slots:
    void slotSelectAll();
    void slotInvertSelection();
    void slotSelectNone();
};

} // KIPI


#endif /* IMAGECOLLECTIONSELECTOR_H */
