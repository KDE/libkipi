/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  main kipi host application interface
 *
 * @author Copyright (C) 2004-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Renchi Raju
 *         <a href="mailto:renchi dot raju at gmail dot com">renchi dot raju at gmail dot com</a>
 * @author Copyright (C) 2004-2005 by Jesper K. Pedersen
 *         <a href="mailto:blackie at kde dot org">blackie at kde dot org</a>
 * @author Copyright (C) 2004-2005 by Aurelien Gateau
 *         <a href="mailto:aurelien dot gateau at free dot fr">aurelien dot gateau at free dot fr</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

/** @file interface.cpp */

#include "interface.moc"

// Qt includes

#include <QPixmap>

// KDE includes

#include <kdeversion.h>
#include <kdebug.h>
#include <kfileitem.h>
#include <kimageio.h>
#include <kio/previewjob.h>

// Local includes

#include "version.h"
#include "pluginloader.h"
#include "imageinfo.h"
#include "imagecollection.h"
#include "imagecollectionselector.h"
#include "uploadwidget.h"

namespace KIPI
{

Interface::Interface(QObject* parent, const char* name)
    : QObject(parent)
{
    setObjectName(name);
}

Interface::~Interface()
{
}

QString KIPI::Interface::version()
{
    return QString(kipi_version);
}

void Interface::refreshImages(const KUrl::List&)
{
}

bool Interface::hasFeature(Features feature) const
{
    return ( features() & feature ) != 0;
}

bool Interface::hasFeature( const QString& feature ) const
{
    if ( feature == "CollectionsHaveComments" )
        return hasFeature( CollectionsHaveComments );
    else if ( feature == "CollectionsHaveCategory" )
        return hasFeature( CollectionsHaveCategory );
    else if ( feature == "CollectionsHaveCreationDate" )
        return hasFeature( CollectionsHaveCreationDate );
    else if ( feature == "ImagesHasComments" )
        return hasFeature( ImagesHasComments );
    else if ( feature == "ImagesHasTime" )
        return hasFeature( ImagesHasTime );
    else if ( feature == "ImagesHasTitlesWritable" )
        return hasFeature( ImagesHasTitlesWritable );
    else if ( feature == "HostSupportsThumbnails" )
        return hasFeature( HostSupportsThumbnails );
    else if ( feature == "HostSupportsDateRanges" )
        return hasFeature( HostSupportsDateRanges );
    else if ( feature == "HostAcceptNewImages" )
        return hasFeature( HostAcceptNewImages );
    else if ( feature == "HostSupportsProgressBar" )
        return hasFeature( HostSupportsProgressBar );
    else if ( feature == "HostSupportsTags" )
        return hasFeature( HostSupportsTags );
    else if ( feature == "HostSupportsRating" )
        return hasFeature( HostSupportsRating );
    else if ( feature == "HostSupportsPickLabel" )
        return hasFeature( HostSupportsPickLabel );
    else if ( feature == "HostSupportsColorLabel" )
        return hasFeature( HostSupportsColorLabel );
    else
    {
        kWarning() << "Unknown feature asked for in KIPI::Interface::hasFeature(): " << feature;
        return false;
    }
}

bool Interface::addImage(const KUrl&, QString&)
{
    printWarningMessage("addImage", "AcceptNewImages");
    return false;
}

void Interface::delImage(const KUrl&)
{
}

KIPI::ImageCollection KIPI::Interface::currentAlbum()
{
    // This implementation is just to be able to write documentation above.
    return ImageCollection();
}

ImageCollection Interface::currentSelection()
{
    // This implementation is just to be able to write documentation above.
    return KIPI::ImageCollection();
}

QList<ImageCollection> Interface::allAlbums()
{
    // This implementation is just to be able to write documentation above.
    return QList<ImageCollection>();
}

int Interface::features() const
{
    // This implementation is just to be able to write documentation above.
    return 0;
}

void Interface::thumbnail(const KUrl& url, int size)
{
    KUrl::List list;
    list << url;
    thumbnails(list, size);
}

void Interface::thumbnails(const KUrl::List& list, int size)
{
#if KDE_IS_VERSION(4,7,0)
    KFileItemList items;
    for (KUrl::List::ConstIterator it = list.begin() ; it != list.end() ; ++it)
    {
        if ((*it).isValid())
            items.append(KFileItem(KFileItem::Unknown, KFileItem::Unknown, *it, true));
    }
    KIO::PreviewJob* job = KIO::filePreview(items, QSize(size, size));
#else
    KIO::PreviewJob *job = KIO::filePreview(list, size);
#endif

    connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)),
            this, SLOT(gotKDEPreview(KFileItem,QPixmap)));

    connect(job, SIGNAL(failed(KFileItem)),
            this, SLOT(failedKDEPreview(KFileItem)));
}

void Interface::gotKDEPreview(const KFileItem& item, const QPixmap& pix)
{
    emit gotThumbnail(item.url(), pix);
}

void Interface::failedKDEPreview(const KFileItem& item)
{
    emit gotThumbnail(item.url(), QPixmap());
}

QVariant Interface::hostSetting(const QString& settingName)
{
    if (settingName == QString("WriteMetadataUpdateFiletimeStamp"))
    {
        return false;
    }
    else if (settingName == QString("WriteMetadataToRAW"))
    {
        return false;
    }
    if (settingName == QString("UseXMPSidecar4Reading"))
    {
        return false;
    }
    if (settingName == QString("MetadataWritingMode"))
    {
        return 0;
    }
    else if (settingName == QString("FileExtensions") || settingName == QString("ImagesExtensions"))
    {
        // Return a list of images file extensions supported by KDE.
        QStringList KDEImagetypes = KImageIO::mimeTypes( KImageIO::Reading );
        QString imagesFileFilter  = KDEImagetypes.join(" ");
        return QString( imagesFileFilter.toLower() + ' ' + imagesFileFilter.toUpper() );
    }
    else if (settingName == QString("RawExtensions"))
    {
        // TODO
    }
    else if (settingName == QString("VideoExtensions"))
    {
        // TODO
    }
    else if (settingName == QString("AudioExtensions"))
    {
        // TODO
    }

    return QVariant();
}

QAbstractItemModel* Interface::getTagTree() const
{
    return NULL;
}

QString Interface::progressScheduled(const QString& title, bool canBeCanceled, bool hasThumb) const
{
    printWarningMessage("progressScheduled", "HostSupportsProgressBar");
    Q_UNUSED(title);
    Q_UNUSED(canBeCanceled);
    Q_UNUSED(hasThumb);
    return QString();
}

void Interface::progressValueChanged(const QString& id, float percent)
{
    printWarningMessage("progressValueChanged", "HostSupportsProgressBar");
    Q_UNUSED(id);
    Q_UNUSED(percent);
}

void Interface::progressStatusChanged(const QString& id, const QString& status)
{
    printWarningMessage("progressStatusChanged", "HostSupportsProgressBar");
    Q_UNUSED(id);
    Q_UNUSED(status);
}

void Interface::progressThumbnailChanged(const QString& id, const QPixmap& thumb)
{
    printWarningMessage("progressThumbnailChanged", "HostSupportsProgressBar");
    Q_UNUSED(id);
    Q_UNUSED(thumb);
}

void Interface::progressCompleted(const QString& id)
{
    printWarningMessage("progressCompleted", "HostSupportsProgressBar");
    Q_UNUSED(id);
}

bool Interface::lockItem(const KUrl& url) const
{
    printWarningMessage("lockItem", "HostSupportsItemLock");
    Q_UNUSED(url);
    return false;
}

bool Interface::unlockItem(const KUrl& url) const
{
    printWarningMessage("unlockItem", "HostSupportsItemLock");
    Q_UNUSED(url);
    return false;
}

bool Interface::itemIsLocked(const KUrl& url) const
{
    printWarningMessage("itemIsLocked", "HostSupportsItemLock");
    Q_UNUSED(url);
    return false;
}

void Interface::printWarningMessage(const QString& method, const QString& feature) const
{
    kWarning() << "KIPI::Interface::" << method << "() should only be invoked if the host application supports the\n"
                  "KIPI::Features::" << feature << ". If the host application do support that, then this function should\n"
                  "have been overridden in the host application.";
}

} // namespace KIPI
