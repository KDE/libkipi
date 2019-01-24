/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2009-11-21
 * @brief  kipi host test application
 *
 * @author Copyright (C) 2009-2010 by Michael G. Hansen
 *         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
 * @author Copyright (C) 2011-2018 by Gilles Caulier
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

// KF includes

#ifdef HAVE_KEXIV2
#   include <kexiv2/kexiv2.h>
#endif

// Local includes

#include "kipiimageinfoshared.h"
#include "kipiimagecollectionselector.h"
#include "kipiuploadwidget.h"
#include "kipiimagecollectionshared.h"
#include "kipiwriteimage.h"

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

bool KipiInterface::saveImage(const QUrl& url, const QString& format,
                              const QByteArray& data, uint width, uint height,
                              bool  sixteenBit, bool hasAlpha, bool* cancel)
{
    KIPIWriteImage writer;
    writer.setImageData(data, width, height, sixteenBit, hasAlpha);
    writer.setCancel(cancel);

    if (format.toUpper() == QLatin1String("JPG") ||
        format.toUpper() == QLatin1String("JPEG"))
    {
        return writer.write2JPEG(url.toLocalFile());
    }

    if (format.toUpper() == QLatin1String("TIF") ||
        format.toUpper() == QLatin1String("TIFF"))
    {
        return writer.write2TIFF(url.toLocalFile());
    }

    if (format.toUpper() == QLatin1String("PNG"))
    {
        return writer.write2PNG(url.toLocalFile());
    }

    if (format.toUpper() == QLatin1String("PPM"))
    {
        return writer.write2PPM(url.toLocalFile());
    }

    return false;
}

// ---------------------------------------------------------------------------------------

#ifdef HAVE_KEXIV2

class KipiMetadataProcessor : public KIPI::MetadataProcessor
{
public:

    KipiMetadataProcessor()  {};
    ~KipiMetadataProcessor() override {};

    bool load(const QUrl& url) override
    {
        return m_meta.load(url.toLocalFile());
    }

    bool save(const QUrl& url, bool writeToFileOnly) override
    {
        if (writeToFileOnly)
            m_meta.setMetadataWritingMode((int) KExiv2Iface::KExiv2::WRITETOIMAGEONLY);

        return m_meta.save(url.toLocalFile());
    }

    bool applyChanges() override
    {
        return m_meta.applyChanges();
    }

    QSize getPixelSize() override
    {
        return m_meta.getPixelSize();
    }

    bool setImageProgramId(const QString& program, const QString& version) override
    {
        return m_meta.setImageProgramId(program, version);
    }

    QSize getImageDimensions() override
    {
        return m_meta.getImageDimensions();
    }

    bool setImageDimensions(const QSize& size) override
    {
        return m_meta.setImageDimensions(size);
    }

    int getImageOrientation() override
    {
        return m_meta.getImageOrientation();
    }

    bool setImageOrientation(int orientation) override
    {
        return m_meta.setImageOrientation((KExiv2Iface::KExiv2::ImageOrientation)orientation);
    }

    bool rotateExifQImage(QImage& img, int orientation) override
    {
        return m_meta.rotateExifQImage(img, (KExiv2Iface::KExiv2::ImageOrientation)orientation);
    }

    QDateTime getImageDateTime() override
    {
        return m_meta.getImageDateTime();
    }

    bool setImageDateTime(const QDateTime& dt) override
    {
        return m_meta.setImageDateTime(dt);
    }

    bool getImagePreview(QImage& img) override
    {
        return m_meta.getImagePreview(img);
    }

    bool setImagePreview(const QImage& img) override
    {
        return m_meta.setImagePreview(img);
    }

    bool hasExif() override
    {
        return m_meta.hasExif();
    }

    bool hasIptc() override
    {
        return m_meta.hasIptc();
    }

    bool hasXmp() override
    {
        return m_meta.hasXmp();
    }

    QByteArray getExif() override
    {
        return m_meta.getExifEncoded();
    }

    QByteArray getIptc() override
    {
        return m_meta.getIptc();
    }

    QByteArray getXmp() override
    {
        return m_meta.getXmp();
    }

    bool setExif(const QByteArray& data) override
    {
        return m_meta.setExif(data);
    }

    bool setIptc(const QByteArray& data) override
    {
        return m_meta.setIptc(data);
    }

    bool setXmp(const QByteArray& data) override
    {
        return m_meta.setXmp(data);
    }

    bool registerXmpNameSpace(const QString& uri, const QString& prefix) override
    {
        return m_meta.registerXmpNameSpace(uri, prefix);
    }

    bool supportXmp() override
    {
        return m_meta.supportXmp();
    }

    bool canWriteXmp(const QUrl& url) override
    {
        return m_meta.canWriteXmp(url.toLocalFile());
    }

    bool removeExifTags(const QStringList& tagFilters) override
    {
        KExiv2Iface::KExiv2::MetaDataMap m = m_meta.getExifTagsDataList(tagFilters);

        if (m.isEmpty())
            return false;

        for (KExiv2Iface::KExiv2::MetaDataMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            m_meta.removeExifTag(it.key().toLatin1().constData());
        }

        return true;
    }

    bool removeIptcTags(const QStringList& tagFilters) override
    {
        KExiv2Iface::KExiv2::MetaDataMap m = m_meta.getIptcTagsDataList(tagFilters);

        if (m.isEmpty())
            return false;

        for (KExiv2Iface::KExiv2::MetaDataMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            m_meta.removeIptcTag(it.key().toLatin1().constData());
        }

        return true;
    }

    bool removeXmpTags(const QStringList& tagFilters) override
    {
        KExiv2Iface::KExiv2::MetaDataMap m = m_meta.getXmpTagsDataList(tagFilters);

        if (m.isEmpty())
            return false;

        for (KExiv2Iface::KExiv2::MetaDataMap::iterator it = m.begin(); it != m.end(); ++it)
        {
            m_meta.removeXmpTag(it.key().toLatin1().constData());
        }

        return true;
    }

    bool getGPSInfo(double& alt, double& lat, double& lon) override
    {
        return m_meta.getGPSInfo(alt, lat, lon);
    }

    bool setGPSInfo(const double alt, const double lat, const double lon) override
    {
        return m_meta.setGPSInfo(alt, lat, lon);
    }

    bool removeGPSInfo() override
    {
        return m_meta.removeGPSInfo();
    }

    QString getExifTagString(const QString& tag) override
    {
        return m_meta.getExifTagString(tag.toLatin1().constData());
    }

    bool setExifTagString(const QString& tag, const QString& val) override
    {
        return m_meta.setExifTagString(tag.toLatin1().constData(), val);
    }

    bool getExifTagRational(const QString& tag, long int& num, long int& den) override
    {
        return m_meta.getExifTagRational(tag.toLatin1().constData(), num, den);
    }

    bool setExifTagRational(const QString& tag, long int num, long int den) override
    {
        return m_meta.setExifTagRational(tag.toLatin1().constData(), num, den);
    }

    QString getXmpTagString(const QString& tag) override
    {
        return m_meta.getXmpTagString(tag.toLatin1().constData());
    }

    bool setXmpTagString(const QString& tag, const QString& val) override
    {
        return m_meta.setXmpTagString(tag.toLatin1().constData(), val);
    }

    QStringList getXmpKeywords() override
    {
        return m_meta.getXmpKeywords();
    }

    bool  setXmpKeywords(const QStringList& keywords) override
    {
        return m_meta.setXmpKeywords(keywords);
    }

    QVariant getXmpTagVariant(const QString& tag) override
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
    return nullptr;  // TODO
}

} // namespace KXMLKipiCmd
