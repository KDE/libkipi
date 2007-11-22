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

// KDE includes.

#include <kdebug.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kimageio.h>

// LibKDcraw includes.

#include <libkdcraw/rawfiles.h>
#include <libkdcraw/dcrawbinary.h>

// Local includes.

#include "version.h"
#include "interface.h"
#include "imagecollection.h"
#include "imagedialog.h"

namespace KIPI
{

class ImageDialogPrivate 
{
public:

    ImageDialogPrivate()
    {
        singleSelect = false;
        iface        = 0;
    }

    bool             singleSelect;

    QString          fileformats;

    KUrl             url;
    KUrl::List       urls;

    KIPI::Interface *iface;
};

ImageDialog::ImageDialog(QWidget* parent, KIPI::Interface* iface, bool singleSelect)
{
    d = new ImageDialogPrivate;
    d->iface        = iface;
    d->singleSelect = singleSelect;
 
    QStringList patternList = KImageIO::pattern(KImageIO::Reading).split('\n', QString::SkipEmptyParts);
    
    // All Images from list must been always the first entry given by KDE API
    QString allPictures = patternList[0];
    
    // Add other files format witch are missing to All Images" type mime provided by KDE and remplace current.
    if (KDcrawIface::DcrawBinary::instance()->versionIsRight())
    {
        allPictures.insert(allPictures.indexOf("|"), QString(raw_file_extentions) + QString(" *.JPE *.TIF"));
        patternList.removeAll(patternList[0]);
        patternList.prepend(allPictures);
    }
    
    // Added RAW file formats supported by dcraw program like a type mime. 
    // Nota: we cannot use here "image/x-raw" type mime from KDE because it uncomplete 
    // or unavailable(see file #121242 in B.K.O).
    if (KDcrawIface::DcrawBinary::instance()->versionIsRight())
        patternList.append(i18n("\n%1|Camera RAW files",QString(raw_file_extentions)));
    
    d->fileformats = patternList.join("\n");

    if (singleSelect)
    {
        d->url = KFileDialog::getOpenUrl(d->iface->currentAlbum().path(), 
                                         d->fileformats, parent, i18n("Select an Image"));
    }
    else
    {
        d->urls = KFileDialog::getOpenUrls(d->iface->currentAlbum().path(), 
                                           d->fileformats, parent, i18n("Select Images"));
    }
}

ImageDialog::~ImageDialog() 
{
    delete d;
}

KUrl ImageDialog::url() const 
{
    return d->url;
}

KUrl::List ImageDialog::urls() const
{
    return d->urls;
}

KUrl ImageDialog::getImageURL(QWidget* parent, Interface* iface) 
{
    ImageDialog dlg(parent, iface, true);

    if (dlg.url().isValid())
    {
        return dlg.url();
    }
    else 
    {
        return KUrl();
    }
}

KUrl::List ImageDialog::getImageURLs(QWidget* parent, Interface* iface)
{
    ImageDialog dlg(parent, iface, false);
    if (!dlg.urls().isEmpty())
    {
        return dlg.urls();
    }
    else
    {
        return KUrl::List();
    }
}

} // namespace KIPI
