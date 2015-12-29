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
 * @author Copyright (C) 2011-2016 by Gilles Caulier
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
#include <QIcon>
#include <QFileInfo>

// Libkipi includes

#include "libkipi_version.h"
#include "imagecollection.h"

// KDE includes

#ifdef HAVE_KDCRAW
#   include <kdcraw/kdcraw.h>
#endif

#ifdef HAVE_KEXIV2
#   include <kexiv2/kexiv2.h>
#endif

// Local includes

#include "kipiimageinfoshared.h"
#include "kipiimagecollectionselector.h"
#include "kipiuploadwidget.h"
#include "kipiimagecollectionshared.h"

namespace KXMLKipiCmd
{

KipiInterface::KipiInterface(QObject* const parent, const QString& name)
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

ImageInfo KipiInterface::info(const QUrl& url)
{
    qDebug() << QString::fromLatin1( "Plugin wants information about image \"%1\"").arg( url.url() );

    return (ImageInfo(new KipiImageInfoShared(this, url)));
}

bool KipiInterface::addImage(const QUrl& url, QString& errmsg)
{
    Q_UNUSED(errmsg);
    qDebug() << QString::fromLatin1( "Plugin added an image: \"%1\"").arg( url.url() );

    return true;
}

void KipiInterface::delImage(const QUrl& url)
{
    qDebug() << QString::fromLatin1( "Plugin deleted an image: \"%1\"").arg( url.url() );
}

void KipiInterface::refreshImages(const QList<QUrl>& urls)
{
    qDebug() << QString::fromLatin1( "Plugin asks to refresh %1 images:").arg( urls.size() );

    for (QList<QUrl>::ConstIterator it = urls.constBegin(); it!=urls.constEnd(); ++it)
    {
        qDebug() << QString::fromLatin1("  ") + (*it).url();
    }
}

int KipiInterface::features() const
{
    qDebug() << "Called by plugins";

    return   ImagesHasTime
#ifdef HAVE_KDCRAW
           | HostSupportsRawProcessing
#endif
#ifdef HAVE_KEXIV2
           | HostSupportsMetadataProcessing
#endif

    ;
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

void KipiInterface::addSelectedImage(const QUrl& image)
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

void KipiInterface::addAlbum(const QUrl& album)
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

void KipiInterface::addSelectedAlbum(const QUrl& album)
{
    m_selectedAlbums.append(album);

    // TODO: recurse through sub-directories?
}

QVariant KipiInterface::hostSetting(const QString& settingName)
{
    Q_UNUSED(settingName);
    return QVariant();
}

void KipiInterface::thumbnails(const QList<QUrl>& list, int)
{
    foreach(const QUrl& url, list)
    {
        QIcon icon(url.url());
        emit gotThumbnail(url, icon.pixmap(256));
    }
}

// ---------------------------------------------------------------------------------------

#ifdef HAVE_KDCRAW

class KipiRawProcessor : public RawProcessor
{
public:

    KipiRawProcessor()          {};
    ~KipiRawProcessor() {};

    bool loadRawPreview(const QUrl& url, QImage& image)
    {
        return m_decoder.loadRawPreview(image, url.toLocalFile());
    }

    bool decodeRawImage(const QUrl& url, QByteArray& imageData, int& width, int& height, int& rgbmax)
    {
        return m_decoder.decodeRAWImage(url.toLocalFile(), KDcrawIface::RawDecodingSettings(),
                                        imageData, width, height, rgbmax);
    }

    void cancel()
    {
        m_decoder.cancel();
    }

    bool isRawFile(const QUrl& url)
    {
        QString   rawFilesExt = QLatin1String(m_decoder.rawFiles());
        QFileInfo fileInfo(url.toLocalFile());

        return (rawFilesExt.toUpper().contains(fileInfo.suffix().toUpper()));
    }

    QString rawFiles()
    {
        return QLatin1String(m_decoder.rawFiles());
    }

private:
    
    KDcrawIface::KDcraw m_decoder;
};

RawProcessor* KipiInterface::createRawProcessor() const
{
    return (new KipiRawProcessor);
}

#else // HAVE_KDCRAW

RawProcessor* KipiInterface::createRawProcessor() const
{
    qDebug() << "This interface was not compiled with libkdcraw to support Raw processing";
    return 0;
}

#endif // HAVE_KDCRAW

// ---------------------------------------------------------------------------------------

#ifdef HAVE_KEXIV2

class KipiMetadataProcessor : public KIPI::MetadataProcessor
{
public:

    KipiMetadataProcessor()  {};
    ~KipiMetadataProcessor() {};

    bool load(const QUrl& url)
    {
        return m_meta.load(url.toLocalFile());
    }

    bool save(const QUrl& url, bool writeToFileOnly)
    {
        if (writeToFileOnly)
            m_meta.setMetadataWritingMode((int)DMetadata::WRITETOIMAGEONLY);

        return m_meta.save(url.toLocalFile());
    }

    bool applyChanges()
    {
        return m_meta.applyChanges();
    }

    QSize getPixelSize()
    {
        return m_meta.getPixelSize();
    }

    bool setImageProgramId(const QString& program, const QString& version)
    {
        return m_meta.setImageProgramId(program, version);
    }

    QSize getImageDimensions()
    {
        return m_meta.getImageDimensions();
    }

    bool setImageDimensions(const QSize& size)
    {
        return m_meta.setImageDimensions(size);
    }

    int getImageOrientation()
    {
        return m_meta.getImageOrientation();
    }

    bool setImageOrientation(int orientation)
    {
        return m_meta.setImageOrientation((DMetadata::ImageOrientation)orientation);
    }

    bool rotateExifQImage(QImage& img, int orientation)
    {
        return m_meta.rotateExifQImage(img, (DMetadata::ImageOrientation)orientation);
    }

    QDateTime getImageDateTime()
    {
        return m_meta.getImageDateTime();
    }

    bool setImageDateTime(const QDateTime& dt)
    {
        return m_meta.setImageDateTime(dt);
    }

    bool getImagePreview(QImage& img)
    {
        return m_meta.getImagePreview(img);
    }

    bool setImagePreview(const QImage& img)
    {
        return m_meta.setImagePreview(img);
    }

    bool hasExif()
    {
        return m_meta.hasExif();
    }

    bool hasIptc()
    {
        return m_meta.hasIptc();
    }

    bool hasXmp()
    {
        return m_meta.hasXmp();
    }

    QByteArray getExif()
    {
        return m_meta.getExifEncoded();
    }

    QByteArray getIptc()
    {
        return m_meta.getIptc();
    }

    QByteArray getXmp()
    {
        return m_meta.getXmp();
    }

    bool setExif(const QByteArray& data)
    {
        return m_meta.setExif(data);
    }

    bool setIptc(const QByteArray& data)
    {
        return m_meta.setIptc(data);
    }

    bool setXmp(const QByteArray& data)
    {
        return m_meta.setXmp(data);
    }

    bool registerXmpNameSpace(const QString& uri, const QString& prefix)
    {
        return m_meta.registerXmpNameSpace(uri, prefix);
    }

    bool supportXmp()
    {
        return m_meta.supportXmp();
    }

    bool canWriteXmp(const QUrl& url)
    {
        return m_meta.canWriteXmp(url.toLocalFile());
    }

    bool removeExifTags(const QStringList& tagFilters)
    {
        DMetadata::MetaDataMap m = m_meta.getExifTagsDataList(tagFilters);

        if (m.isEmpty())
            return false;

        for (DMetadata::MetaDataMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            m_meta.removeExifTag(it.key().toLatin1().constData());
        }

        return true;
    }

    bool removeIptcTags(const QStringList& tagFilters)
    {
        DMetadata::MetaDataMap m = m_meta.getIptcTagsDataList(tagFilters);

        if (m.isEmpty())
            return false;

        for (DMetadata::MetaDataMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            m_meta.removeIptcTag(it.key().toLatin1().constData());
        }

        return true;
    }

    bool removeXmpTags(const QStringList& tagFilters)
    {
        DMetadata::MetaDataMap m = m_meta.getXmpTagsDataList(tagFilters);

        if (m.isEmpty())
            return false;

        for (DMetadata::MetaDataMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            m_meta.removeXmpTag(it.key().toLatin1().constData());
        }

        return true;
    }

    bool getGPSInfo(double& alt, double& lat, double& lon)
    {
        return m_meta.getGPSInfo(alt, lat, lon);
    }

    bool setGPSInfo(const double alt, const double lat, const double lon)
    {
        return m_meta.setGPSInfo(alt, lat, lon);
    }

    bool removeGPSInfo()
    {
        return m_meta.removeGPSInfo();
    }

    QString getExifTagString(const QString& tag)
    {
        return m_meta.getExifTagString(tag.toLatin1().constData());
    }

    bool setExifTagString(const QString& tag, const QString& val)
    {
        return m_meta.setExifTagString(tag.toLatin1().constData(), val);
    }

    bool getExifTagRational(const QString& tag, long int& num, long int& den)
    {
        return m_meta.getExifTagRational(tag.toLatin1().constData(), num, den);
    }

    bool setExifTagRational(const QString& tag, long int num, long int den)
    {
        return m_meta.setExifTagRational(tag.toLatin1().constData(), num, den);
    }

    QString getXmpTagString(const QString& tag)
    {
        return m_meta.getXmpTagString(tag.toLatin1().constData());
    }

    bool setXmpTagString(const QString& tag, const QString& val)
    {
        return m_meta.setXmpTagString(tag.toLatin1().constData(), val);
    }

    QStringList getXmpKeywords()
    {
        return m_meta.getXmpKeywords();
    }

    bool  setXmpKeywords(const QStringList& keywords)
    {
        return m_meta.setXmpKeywords(keywords);
    }

    QVariant getXmpTagVariant(const QString& tag)
    {
        return m_meta.getXmpTagVariant(tag.toLatin1().constData());
    }

private:

    KExiv2Iface::KExiv2 m_meta;
};

MetadataProcessor* KipiInterface::createMetadataProcessor() const
{
    return (new KipiMetadataProcessor);
}

#else // HAVE_KEXIV2

MetadataProcessor* KipiInterface::createMetadataProcessor() const
{
    qDebug() << "This interface was not compiled with libkexiv2 to support Metadata processing";
    return 0;
}

#endif // HAVE_KEXIV2

// ---------------------------------------------------------------------------------------

FileReadWriteLock* KipiInterface::createReadWriteLock(const QUrl&) const
{
    return 0;  // TODO
}

} // namespace KXMLKipiCmd
