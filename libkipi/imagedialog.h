/* ============================================================
 * File   : imagedialog.h
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-05
 * Description : an image files selector dialog.
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
 
#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

// Qt includes.

#include <qvaluelist.h>
#include <qwidget.h>

// KDE includes.

#include <kurl.h>
#include <kdialogbase.h>

// LibKipi includes.

#include <libkipi/interface.h>
#include <libkipi/imagecollection.h>


class QListViewItem;

class KFileItem;
class KListView;

namespace KIPI
{


class ImageDialog : public KDialogBase 
{
Q_OBJECT
    
    class Private;
    
public:

    ImageDialog(QWidget*, Interface*, bool singleSelection=false);
    ~ImageDialog();

    KURL       url() const;
    KURL::List urls() const;
    
    static KURL getImageURL(QWidget*, Interface*);
    static KURL::List getImageURLs(QWidget*, Interface*);

private slots:

    void fillImageList(QListViewItem*);
    void slotImageSelected(QListViewItem*);
    void slotImagesSelected();
    void slotGotPreview(const KFileItem* , const QPixmap&);
    void slotHelp( void );
    void slotInitialShow();

private:
    Private* d;
};

} // namespace KIPI

#endif /* IMAGECOLLECTIONDIALOG_H */
