/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2004-02-01
 * @brief  main kipi host application interface
 *
 * @author Copyright (C) 2004-2018 by Gilles Caulier
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

#include "interface.h"

// Qt includes

#include <QPixmap>
#include <QImageReader>
#include <QImageWriter>
#include <QDir>

// Local includes

#include "libkipi_version.h"
#include "libkipi_debug.h"
#include "imageinfo.h"
#include "imagecollection.h"
#include "imagecollectionselector.h"
#include "imageinfoshared.h"
#include "pluginloader.h"
#include "uploadwidget.h"

// Macros

#define PrintWarningMessageFeature(feature)                                                       \
        qCWarning(LIBKIPI_LOG) << "This should only be invoked if the host application supports " \
                                  "KIPI::Features (" << feature << "). If host application do "   \
                                  "support that, then this function should have been overridden " \
                                  "in the KIPI host interface."

#define PrintWarningMessage() qCWarning(LIBKIPI_LOG) << "This method should have been overridden in the kipi host interface."

inline void initLibkipiResource()
{
    Q_INIT_RESOURCE(libkipi);
}

namespace KIPI
{

Interface::Interface(QObject* const parent, const QString& name)
    : QObject(parent)
{
    initLibkipiResource();

    setObjectName(name);
}

Interface::~Interface()
{
}

QString Interface::version()
{
    return QString::fromLatin1(KIPI_VERSION_STRING);
}

void Interface::refreshImages(const QList<QUrl>&)
{
    PrintWarningMessage();
}

bool Interface::hasFeature(Features feature) const
{
    return (features() & feature) != 0;
}

bool Interface::hasFeature( const QString& feature ) const
{
    if       ( feature == QString::fromLatin1("CollectionsHaveComments" ))
        return hasFeature( CollectionsHaveComments );
    else if  ( feature == QString::fromLatin1("CollectionsHaveCategory" ))
        return hasFeature( CollectionsHaveCategory );
    else if  ( feature == QString::fromLatin1("CollectionsHaveCreationDate" ))
        return hasFeature( CollectionsHaveCreationDate );
    else if  ( feature == QString::fromLatin1("ImagesHasComments" ))
        return hasFeature( ImagesHasComments );
    else if  ( feature == QString::fromLatin1("ImagesHasTime" ))
        return hasFeature( ImagesHasTime );
    else if  ( feature == QString::fromLatin1("ImagesHasTitlesWritable" ))
        return hasFeature( ImagesHasTitlesWritable );
    else if  ( feature == QString::fromLatin1("HostSupportsThumbnails" ))
        return hasFeature( HostSupportsThumbnails );
    else if  ( feature == QString::fromLatin1("HostSupportsReadWriteLock" ))
        return hasFeature( HostSupportsReadWriteLock );
    else if  ( feature == QString::fromLatin1("HostSupportsDateRanges" ))
        return hasFeature( HostSupportsDateRanges );
    else if  ( feature == QString::fromLatin1("HostAcceptNewImages" ))
        return hasFeature( HostAcceptNewImages );
    else if  ( feature == QString::fromLatin1("HostSupportsProgressBar" ))
        return hasFeature( HostSupportsProgressBar );
    else if  ( feature == QString::fromLatin1("HostSupportsTags" ))
        return hasFeature( HostSupportsTags );
    else if  ( feature == QString::fromLatin1("HostSupportsRating" ))
        return hasFeature( HostSupportsRating );
    else if  ( feature == QString::fromLatin1("HostSupportsPickLabel" ))
        return hasFeature( HostSupportsPickLabel );
    else if  ( feature == QString::fromLatin1("HostSupportsColorLabel" ))
        return hasFeature( HostSupportsColorLabel );
    else if  ( feature == QString::fromLatin1("HostSupportsItemReservation" ))
        return hasFeature( HostSupportsItemReservation );
    else if  ( feature == QString::fromLatin1("HostSupportsPreviews" ))
        return hasFeature( HostSupportsPreviews );
    else if  ( feature == QString::fromLatin1("HostSupportsRawProcessing" ))
        return hasFeature( HostSupportsRawProcessing );
    else
    {
        qCWarning(LIBKIPI_LOG) << "Unknown feature asked for in KIPI::Interface::hasFeature(): " << feature;
        return false;
    }
}

bool Interface::addImage(const QUrl&, QString&)
{
    PrintWarningMessageFeature("AcceptNewImages");
    return false;
}

void Interface::delImage(const QUrl&)
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

void Interface::thumbnail(const QUrl& url, int size)
{
    thumbnails(QList<QUrl>() << url, size);
}

void Interface::thumbnails(const QList<QUrl>& list, int size)
{
    Q_UNUSED(size);
    PrintWarningMessageFeature("HostSupportsThumbnails");

    for (QList<QUrl>::ConstIterator it = list.begin() ; it != list.end() ; ++it)
    {
        if ((*it).isValid())
            emit gotThumbnail((*it), QPixmap());
    }
}

QImage Interface::preview(const QUrl& url)
{
    Q_UNUSED(url);
    PrintWarningMessageFeature("HostSupportsPreviews");

    return QImage();
}

bool Interface::saveImage(const QUrl& url, const QString& format,
                          const QByteArray& data, uint width, uint height,
                          bool  sixteenBit, bool hasAlpha,
                          bool* cancel)
{
    Q_UNUSED(url);
    Q_UNUSED(format);
    Q_UNUSED(data);
    Q_UNUSED(format);
    Q_UNUSED(width);
    Q_UNUSED(height);
    Q_UNUSED(sixteenBit);
    Q_UNUSED(hasAlpha);
    Q_UNUSED(cancel);
    PrintWarningMessageFeature("HostSupportsSaveImages");
    return false;
}

void Interface::preview(const QUrl& url, int resizedTo)
{
    Q_UNUSED(url);
    Q_UNUSED(resizedTo);
    PrintWarningMessageFeature("HostSupportsPreviews");

    if (url.isValid())
        emit gotPreview(url, QImage());
}

QString Interface::rawFiles()
{
    PrintWarningMessage();
    return QString();
}

QAbstractItemModel* Interface::getTagTree() const
{
    PrintWarningMessage();
    return nullptr;
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

bool Interface::reserveForAction(const QUrl&, QObject* const, const QString&) const
{
    PrintWarningMessageFeature("HostSupportsItemReservation");
    return false;
}

void Interface::clearReservation(const QUrl&, QObject* const)
{
    PrintWarningMessageFeature("HostSupportsItemReservation");
}

bool Interface::itemIsReserved(const QUrl&, QString* const) const
{
    PrintWarningMessageFeature("HostSupportsItemReservation");
    return false;
}

FileReadWriteLock* Interface::createReadWriteLock(const QUrl&) const
{
    PrintWarningMessageFeature("HostSupportsReadWriteLock");
    return nullptr;
}

MetadataProcessor* Interface::createMetadataProcessor() const
{
    PrintWarningMessageFeature("HostSupportsMetadataProcessing");
    return nullptr;
}

void Interface::aboutToEdit(const QUrl&, EditHints)
{
}

void Interface::editingFinished(const QUrl&, EditHints)
{
}

QStringList Interface::supportedImageMimeTypes(bool readWrite)
{
    QStringList       mimeTypes;
    QList<QByteArray> supported = readWrite ? QImageWriter::supportedMimeTypes()
                                            : QImageReader::supportedMimeTypes();

    Q_FOREACH(QByteArray mimeType, supported)
        mimeTypes.append(QString::fromLatin1(mimeType));

    return mimeTypes;
}

// -----------------------------------------------------------------------------------------------------------

FileReadLocker::FileReadLocker(Interface* const iface, const QUrl& url)
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

FileWriteLocker::FileWriteLocker(Interface* const iface, const QUrl& url)
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

class Q_DECL_HIDDEN EditHintScope::Private
{

public:

    Private() :
        iface(nullptr)
    {
    }

public:

    Interface* iface;
    QUrl       url;
    EditHints  hints;
};

EditHintScope::EditHintScope(Interface* const iface, const QUrl& url, EditHints hints)
    : d(new Private)
{
    d->iface = iface;
    d->url   = url;
    d->hints = hints;

    if (d->iface)
    {
        d->iface->aboutToEdit(d->url, d->hints);
    }
}

EditHintScope::~EditHintScope()
{
    if (d->iface)
    {
        d->iface->editingFinished(d->url, d->hints);
    }

    delete d;
}

void EditHintScope::changeAborted()
{
    d->hints |= HintEditAborted;
}

} // namespace KIPI
