/** ===========================================================
 * @file
 *
 * This file is a part of kipi-plugins project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2009-11-21
 * @brief  kipi host test application
 *
 * @author Copyright (C) 2009-2010 by Michael G. Hansen
 *         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
 * @author Copyright (C) 2011-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "kipiinterface.h"

// Qt includes

#include <QTextStream>
#include <QDebug>

// KDE includes

#include <klocalizedstring.h>
#include <kfileitem.h>
#include <kio/previewjob.h>

// Libkipi includes

#include "libkipi_version.h"
#include "imagecollection.h"

// local includes

#include "kipiimageinfoshared.h"
#include "kipiimagecollectionselector.h"
#include "kipiuploadwidget.h"
#include "kipiimagecollectionshared.h"

namespace KXMLKipiCmd
{

KipiInterface::KipiInterface(QObject* const parent, const char* name)
    : Interface(parent, name),
      m_selectedImages(),
      m_selectedAlbums(),
      m_albums()
{
}

KipiInterface::~KipiInterface()
{
}

ImageCollection KipiInterface::currentAlbum()
{
    qDebug() << "Called by plugins";

    QUrl currentAlbumUrl;

    if (!m_selectedAlbums.isEmpty())
    {
        currentAlbumUrl = m_selectedAlbums.at(0);
    }

    return (ImageCollection(new KipiImageCollectionShared(currentAlbumUrl)));
}

ImageCollection KipiInterface::currentSelection()
{
    qDebug() << "Called by plugins";

    return (ImageCollection(new KipiImageCollectionShared(m_selectedImages)));
}

QList<ImageCollection> KipiInterface::allAlbums()
{
    QList<ImageCollection> listAllAlbums;

    for (QList<QUrl>::const_iterator it = m_albums.constBegin(); it!=m_albums.constEnd(); ++it)
    {
        listAllAlbums.append(ImageCollection(new KipiImageCollectionShared(*it)));
    }

    // make sure albums which have been specified as selectedalbums are also in the allAlbums list:
    for (QList<QUrl>::const_iterator it = m_selectedAlbums.constBegin(); it!=m_selectedAlbums.constEnd(); ++it)
    {
        if (!m_albums.contains(*it))
        {
            listAllAlbums.append(ImageCollection(new KipiImageCollectionShared(*it)));
        }
    }

    return listAllAlbums;
}

ImageInfo KipiInterface::info(const QUrl &url)
{
    qDebug() << QString( "Plugin wants information about image \"%1\"").arg( url.url() );

    return (ImageInfo(new KipiImageInfoShared(this, url)));
}

bool KipiInterface::addImage(const QUrl &url, QString& errmsg)
{
    Q_UNUSED(errmsg);
    qDebug() << QString( "Plugin added an image: \"%1\"").arg( url.url() );

    return true;
}

void KipiInterface::delImage(const QUrl &url)
{
    qDebug() << QString( "Plugin deleted an image: \"%1\"").arg( url.url() );
}

void KipiInterface::refreshImages(const QList<QUrl>& urls)
{
    qDebug() << QString( "Plugin asks to refresh %1 images:").arg( urls.size() );

    for (QList<QUrl>::ConstIterator it = urls.constBegin(); it!=urls.constEnd(); ++it)
    {
        qDebug() << "  " + (*it).url();
    }
}

int KipiInterface::features() const
{
    qDebug() << "Called by plugins";

    return ImagesHasTime;
}

ImageCollectionSelector* KipiInterface::imageCollectionSelector(QWidget* parent)
{
    qDebug() << "Called by plugins";

    return (new KipiImageCollectionSelector(this, parent));
}

UploadWidget* KipiInterface::uploadWidget(QWidget* parent)
{
    qDebug() << "Called by plugins";

    return (new KipiUploadWidget(this, parent));
}

void KipiInterface::addSelectedImages(const QList<QUrl>& images)
{
    m_selectedImages.append(images);
}

void KipiInterface::addSelectedImage(const QUrl &image)
{
    m_selectedImages.append(image);
}

void KipiInterface::addAlbums(const QList<QUrl>& albums)
{
    for (QList<QUrl>::const_iterator it = albums.constBegin(); it!=albums.constEnd(); ++it)
    {
        addAlbum(*it);
    }
}

void KipiInterface::addAlbum(const QUrl &album)
{
    m_albums.append(album);

    // TODO: recurse through sub-directories?
}

void KipiInterface::addSelectedAlbums(const QList<QUrl>& albums)
{
    for (QList<QUrl>::const_iterator it = albums.constBegin(); it!=albums.constEnd(); ++it)
    {
        addSelectedAlbum(*it);
    }
}

void KipiInterface::addSelectedAlbum(const QUrl &album)
{
    m_selectedAlbums.append(album);

    // TODO: recurse through sub-directories?
}

QVariant KipiInterface::hostSetting(const QString& settingName)
{
    if (settingName == QString("UseXMPSidecar4Reading"))
    {
        return (QVariant::fromValue(true));
    }
    else if (settingName == QString("MetadataWritingMode"))
    {
        return (QVariant::fromValue(false));
    }

    return QVariant();
}

void KipiInterface::thumbnails(const QList<QUrl>& list, int)
{
    foreach(const QUrl &url, list)
    {
        KFileItemList items;
        items.append(KFileItem(url));
        KIO::PreviewJob* const job = KIO::filePreview(items, QSize(256, 256));

        connect(job, &KIO::PreviewJob::gotPreview,
                this, &KipiInterface::slotGotKDEPreview);

        connect(job, &KIO::PreviewJob::failed, this,
                &KipiInterface::slotFailedKDEPreview);
    }
}

void KipiInterface::slotGotKDEPreview(const KFileItem& item, const QPixmap& pix)
{
    emit gotThumbnail(item.url(), pix);
}

void KipiInterface::slotFailedKDEPreview(const KFileItem& item)
{
    emit gotThumbnail(item.url(), QPixmap());
}

} // namespace KXMLKipiCmd
