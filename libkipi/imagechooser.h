#ifndef IMAGECHOOSER_H
#define IMAGECHOOSER_H

#include <qvaluelist.h>
#include <qwidget.h>

#include <kurl.h>
#include <kdialogbase.h>

#include <libkipi/interface.h>
#include <libkipi/imagecollection.h>


class KFileItem;
class KListView;
class QListViewItem;

namespace KIPI
{


class ImageChooser : public KDialogBase {
Q_OBJECT
	class Private;
public:
    ImageChooser(QWidget*, Interface*);
    ~ImageChooser();
    KURL url() const;
    static KURL getImageURL(QWidget*, Interface*);

private slots:
    void fillImageList(QListViewItem*);
    void slotImageSelected(QListViewItem*);
	void slotGotPreview(const KFileItem* , const QPixmap&);

private:
	Private* d;
};

} // namespace

#endif /* IMAGECOLLECTIONDIALOG_H */
