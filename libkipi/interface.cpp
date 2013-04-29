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
 * @author Copyright (C) 2006-2012 by Marcel Wiesweg
 *         <a href="mailto:marcel dot wiesweg at gmx dot de">marcel dot wiesweg at gmx dot de</a>
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
#include "imageinfo.h"
#include "imagecollection.h"
#include "imagecollectionselector.h"
#include "imageinfoshared.h"
#include "pluginloader.h"
#include "uploadwidget.h"

// Macros

#define PrintWarningMessageFeature(feature)                                           \
        kWarning() << "This should only be invoked if the host application supports " \
                      "KIPI::Features (" << feature << "). If host application do "   \
                      "support that, then this function should have been overridden " \
                      "in the KIPI host interface."

#define PrintWarningMessage() kWarning() << "This method should have been overridden in the kipi host interface."

namespace KIPI
{

Interface::Interface(QObject* const parent, const char* name)
    : QObject(parent)
{
    setObjectName(name);
}

Interface::~Interface()
{
}

QString Interface::version()
{
    return QString(kipi_version);
}

void Interface::refreshImages(const KUrl::List&)
{
    PrintWarningMessage();
}

bool Interface::hasFeature(Features feature) const
{
    return (features() & feature) != 0;
}

bool Interface::hasFeature( const QString& feature ) const
{
    if       ( feature == "CollectionsHaveComments" )
        return hasFeature( CollectionsHaveComments );
    else if  ( feature == "CollectionsHaveCategory" )
        return hasFeature( CollectionsHaveCategory );
    else if  ( feature == "CollectionsHaveCreationDate" )
        return hasFeature( CollectionsHaveCreationDate );
    else if  ( feature == "ImagesHasComments" )
        return hasFeature( ImagesHasComments );
    else if  ( feature == "ImagesHasTime" )
        return hasFeature( ImagesHasTime );
    else if  ( feature == "ImagesHasTitlesWritable" )
        return hasFeature( ImagesHasTitlesWritable );
    else if  ( feature == "HostSupportsThumbnails" )
        return hasFeature( HostSupportsThumbnails );
    else if  ( feature == "HostSupportsReadWriteLock" )
        return hasFeature( HostSupportsReadWriteLock );
    else if  ( feature == "HostSupportsDateRanges" )
        return hasFeature( HostSupportsDateRanges );
    else if  ( feature == "HostAcceptNewImages" )
        return hasFeature( HostAcceptNewImages );
    else if  ( feature == "HostSupportsProgressBar" )
        return hasFeature( HostSupportsProgressBar );
    else if  ( feature == "HostSupportsTags" )
        return hasFeature( HostSupportsTags );
    else if  ( feature == "HostSupportsRating" )
        return hasFeature( HostSupportsRating );
    else if  ( feature == "HostSupportsPickLabel" )
        return hasFeature( HostSupportsPickLabel );
    else if  ( feature == "HostSupportsColorLabel" )
        return hasFeature( HostSupportsColorLabel );
    else if  ( feature == "HostSupportsItemReservation" )
        return hasFeature( HostSupportsItemReservation );
    else
    {
        kWarning() << "Unknown feature asked for in KIPI::Interface::hasFeature(): " << feature;
        return false;
    }
}

bool Interface::addImage(const KUrl&, QString&)
{
    PrintWarningMessageFeature("AcceptNewImages");
    return false;
}

void Interface::delImage(const KUrl&)
{
    PrintWarningMessage();
}

ImageCollection Interface::currentAlbum()
{
    PrintWarningMessage();
    return ImageCollection();
}

ImageCollection Interface::currentSelection()
{
    PrintWarningMessage();
    return ImageCollection();
}

QList<ImageCollection> Interface::allAlbums()
{
    PrintWarningMessage();
    return QList<ImageCollection>();
}

int Interface::features() const
{
    PrintWarningMessage();
    return 0;
}

void Interface::thumbnail(const KUrl& url, int size)
{
    thumbnails(KUrl::List() << url, size);
}

void Interface::thumbnails(const KUrl::List& list, int size)
{
    PrintWarningMessageFeature("HostSupportsThumbnails");

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
    PrintWarningMessage();

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
    }
    else if (settingName == QString("VideoExtensions"))
    {
    }
    else if (settingName == QString("AudioExtensions"))
    {
    }

    return QVariant();
}

QAbstractItemModel* Interface::getTagTree() const
{
    PrintWarningMessage();
    return 0;
}

QString Interface::progressScheduled(const QString& title, bool canBeCanceled, bool hasThumb) const
{
    PrintWarningMessageFeature("HostSupportsProgressBar");
    Q_UNUSED(title);
    Q_UNUSED(canBeCanceled);
    Q_UNUSED(hasThumb);
    return QString();
}

void Interface::progressValueChanged(const QString& id, float percent)
{
    PrintWarningMessageFeature("HostSupportsProgressBar");
    Q_UNUSED(id);
    Q_UNUSED(percent);
}

void Interface::progressStatusChanged(const QString& id, const QString& status)
{
    PrintWarningMessageFeature("HostSupportsProgressBar");
    Q_UNUSED(id);
    Q_UNUSED(status);
}

void Interface::progressThumbnailChanged(const QString& id, const QPixmap& thumb)
{
    PrintWarningMessageFeature("HostSupportsProgressBar");
    Q_UNUSED(id);
    Q_UNUSED(thumb);
}

void Interface::progressCompleted(const QString& id)
{
    PrintWarningMessageFeature("HostSupportsProgressBar");
    Q_UNUSED(id);
}

bool Interface::reserveForAction(const KUrl&, QObject* const, const QString&) const
{
    PrintWarningMessageFeature("HostSupportsItemReservation");
    return false;
}

void Interface::clearReservation(const KUrl&, QObject* const)
{
    PrintWarningMessageFeature("HostSupportsItemReservation");
}

bool Interface::itemIsReserved(const KUrl&, QString* const) const
{
    PrintWarningMessageFeature("HostSupportsItemReservation");
    return false;
}

FileReadWriteLock* Interface::createReadWriteLock(const KUrl&) const
{
    // Dont print warning as we use the feature from low-level kipi libraries without testing for support
    //PrintWarningMessageFeature("HostSupportsReadWriteLock");
    return 0;
}

void Interface::aboutToEdit(const KUrl&, EditHints)
{
}

void Interface::editingFinished(const KUrl&, EditHints)
{
}


// -----------------------------------------------------------------------------------------------------------

FileReadLocker::FileReadLocker(Interface* const iface, const KUrl& url)
    : d(iface->createReadWriteLock(url))
{
    relock();
}

FileReadLocker::FileReadLocker(ImageInfoShared* const info)
    : d(info->createReadWriteLock())
{
    relock();
}

FileReadLocker::~FileReadLocker()
{
    unlock();
}

FileReadWriteLock* FileReadLocker::fileReadWriteLock() const
{
    return d;
}

void FileReadLocker::relock()
{
    if (d)
    {
        d->lockForRead();
    }
}

void FileReadLocker::unlock()
{
    if (d)
    {
        d->unlock();
    }
}

// -----------------------------------------------------------------------------------------------------------

FileWriteLocker::FileWriteLocker(Interface* const iface, const KUrl& url)
    : d(iface->createReadWriteLock(url))
{
    relock();
}

FileWriteLocker::FileWriteLocker(ImageInfoShared* const info)
    : d(info->createReadWriteLock())
{
    relock();
}

FileWriteLocker::~FileWriteLocker()
{
    unlock();
}

FileReadWriteLock* FileWriteLocker::fileReadWriteLock() const
{
    return d;
}

void FileWriteLocker::relock()
{
    if (d)
    {
        d->lockForWrite();
    }
}

void FileWriteLocker::unlock()
{
    if (d)
    {
        d->unlock();
    }
}

// -----------------------------------------------------------------------------------------------------------

EditHintScope::EditHintScope(Interface* const iface, const KUrl& url, EditHints hints)
    : iface(iface), url(url), hints(hints)
{
    if (iface)
    {
        iface->aboutToEdit(url, hints);
    }
}

EditHintScope::~EditHintScope()
{
    if (iface)
    {
        iface->editingFinished(url, hints);
    }
}

void EditHintScope::changeAborted()
{
    hints |= HintEditAborted;
}


} // namespace KIPI
