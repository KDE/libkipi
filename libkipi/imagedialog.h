/* ============================================================
 * File   : imagedialog.h
 * Authors: KIPI team developers (see AUTHORS files for details)
 *	    
 * Date   : 2004-05
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
#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

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


class ImageDialog : public KDialogBase {
Q_OBJECT
	class Private;
public:
    ImageDialog(QWidget*, Interface*);
    ~ImageDialog();
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
