/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-05-01
 * Description : an image files selector dialog.
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

#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

// Qt includes.

#include <qvaluelist.h>
#include <qwidget.h>

// KDE includes.

#include <kurl.h>
#include <kdialogbase.h>

// LibKipi includes.

#include "libkipi/interface.h"
#include "libkipi/imagecollection.h"
#include "libkipi/libkipi_export.h"

class QListViewItem;

class KFileItem;
class KListView;

namespace KIPI
{

class LIBKIPI_EXPORT ImageDialog : public KDialogBase 
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
