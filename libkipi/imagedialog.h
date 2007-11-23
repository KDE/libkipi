/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2004-05-01
 * Description : an image files selector dialog.
 *
 * Copyright (C) 2004-2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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
#include <QStringList>

// KDE includes.

#include <kurl.h>
#include <kpreviewwidgetbase.h>

// LibKipi includes.

#include "interface.h"
#include "libkipi_export.h"

namespace KIPI
{

class ImageDialogPrivate;
class ImageDialogPreviewPrivate;

class LIBKIPI_EXPORT ImageDialogPreview : public KPreviewWidgetBase
{
    Q_OBJECT

public:

    ImageDialogPreview(Interface *iface, QWidget *parent=0);
    ~ImageDialogPreview();

    QSize sizeHint() const;

private slots:

    void showPreview();
    void showPreview(const KUrl &url);
    void slotThumbnail(const KUrl& url, const QPixmap& pix);
    void clearPreview();

private:

    void resizeEvent(QResizeEvent *e);

private:

    class ImageDialogPreviewPrivate *d;
};

// ------------------------------------------------------------------------

class LIBKIPI_EXPORT ImageDialog
{

public:

    ImageDialog(QWidget* parent, Interface* iface, bool singleSelection=false, bool onlyRaw=false);
    ~ImageDialog();

    KUrl       url() const;
    KUrl::List urls() const;

    bool    onlyRaw() const;
    bool    singleSelect() const;
    QString fileformats() const;

    static KUrl getImageURL(QWidget* parent, Interface* iface, bool onlyRaw=false);
    static KUrl::List getImageURLs(QWidget* parent, Interface* iface, bool onlyRaw=false);

private:

    ImageDialogPrivate* d;
};

} // namespace KIPI

#endif /* IMAGECOLLECTIONDIALOG_H */
