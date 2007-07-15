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

#include <QtCore/QList>
#include <QtGui/QWidget>

// KDE includes.

#include <kurl.h>
#include <kdialog.h>

// LibKipi includes.

#include "interface.h"
#include "imagecollection.h"
#include "libkipi_export.h"

class Q3ListViewItem;

class K3FileItem;

namespace KIPI
{

class LIBKIPI_EXPORT ImageDialog : public KDialog 
{
Q_OBJECT

    class Private;

public:

    ImageDialog(QWidget*, Interface*, bool singleSelection=false);
    ~ImageDialog();

    KUrl       url() const;
    KUrl::List urls() const;

    static KUrl getImageURL(QWidget*, Interface*);
    static KUrl::List getImageURLs(QWidget*, Interface*);

private Q_SLOTS:

    void fillImageList(Q3ListViewItem*);
    void slotImageSelected(Q3ListViewItem*);
    void slotImagesSelected();
    void slotGotPreview(const K3FileItem*, const QPixmap&);
    void slotInitialShow();

private:

    Private* d;
};

} // namespace KIPI

#endif /* IMAGECOLLECTIONDIALOG_H */
