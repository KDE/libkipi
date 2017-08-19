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
