#ifndef IMAGECOLLECTIONDIALOG_H
#define IMAGECOLLECTIONDIALOG_H   

#include <qvaluelist.h>
#include <qwidget.h>

#include <kurl.h>
#include <kdialogbase.h>

#include <libkipi/interface.h>
#include <libkipi/imagecollection.h>


class KListView;
class QListViewItem;

namespace KIPI
{


class ImageCollectionDialog : public KDialogBase {
Q_OBJECT
	class Private;
public:
    ImageCollectionDialog(QWidget*, Interface*);
    ~ImageCollectionDialog();
    KURL url() const;
    static KURL getImageURL(QWidget*, Interface*);

private slots:
    void fillImageList(QListViewItem*);
    void selectImage(QListViewItem*);
    
private:
	Private* d;
};

} // namespace

#endif /* IMAGECOLLECTIONDIALOG_H */
