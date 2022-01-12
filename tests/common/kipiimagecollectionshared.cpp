/*
    SPDX-FileCopyrightText: 2009-2010 Michael G. Hansen <mike at mghansen dot de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "kipiimagecollectionshared.h"

// Qt includes

#include <QDir>

namespace KXMLKipiCmd
{

KipiImageCollectionShared::KipiImageCollectionShared(const QUrl& albumPath)
    : ImageCollectionShared(),
      m_albumPath(albumPath),
      m_images()
{
    // go through the album and add its images:
    const QString albumPathString = m_albumPath.toLocalFile();

    // add only the files, because recursion through directories should be
    // handled in KipiInterface::add[Selected]Album
    // TODO: restrict the search to images!
    const QFileInfoList files     = QDir(albumPathString).entryInfoList(QDir::Files);

    for (QFileInfoList::const_iterator it = files.constBegin(); it!=files.constEnd(); ++it)
    {
            m_images.append(QUrl::fromLocalFile(it->absoluteFilePath()));
    }
}

KipiImageCollectionShared::KipiImageCollectionShared(const QList<QUrl>& images)
    : ImageCollectionShared(),
      m_images(images)
{
}

KipiImageCollectionShared::~KipiImageCollectionShared()
{
}

QString KipiImageCollectionShared::name()
{
    return m_albumPath.url();
}

QList<QUrl> KipiImageCollectionShared::images()
{
    return m_images;
}

void KipiImageCollectionShared::addImages(const QList<QUrl>& images)
{
    m_images.append(images);
}

void KipiImageCollectionShared::addImage(const QUrl &image)
{
    m_images.append(image);
}

QUrl KipiImageCollectionShared::path()
{
    return m_albumPath;
}

QUrl KipiImageCollectionShared::uploadPath()
{
    return m_albumPath;
}

QUrl KipiImageCollectionShared::uploadRoot()
{
    return m_albumPath;
}

bool KipiImageCollectionShared::isDirectory()
{
    return true;
}

} // namespace KXMLKipiCmd
