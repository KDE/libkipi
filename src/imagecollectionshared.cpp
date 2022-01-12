/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// Local includes

#include "imagecollectionshared.h"
#include "imagecollection.h"
#include "libkipi_debug.h"

// Macros

#define PrintWarningMessageFeature(feature)                                                       \
        qCWarning(LIBKIPI_LOG) << "This should only be invoked if the host application supports " \
                                  "KIPI::Features (" << feature << "). If host application do "   \
                                  "support that, then this function should have been overridden " \
                                  "in the KIPI host interface."

namespace KIPI
{

ImageCollectionShared::ImageCollectionShared()
    : m_count(1)
{
}

ImageCollectionShared::~ImageCollectionShared()
{
}

void ImageCollectionShared::addRef()
{
    m_count++;
}

void ImageCollectionShared::removeRef()
{
    m_count--;

    if ( m_count == 0 )
    {
        //qCDebug(LIBKIPI_LOG) << "Deleting!";
        delete this;
    }
}

QUrl ImageCollectionShared::url()
{
    qCWarning(LIBKIPI_LOG) << "This method should only be invoked if this imagecollection is a directory. "
                           << "See KIPI::ImageCollectionShared::isDirectory()";
    return QUrl();
}

QUrl ImageCollectionShared::uploadUrl()
{
    PrintWarningMessageFeature("AcceptNewImages");
    return QUrl();
}

QUrl ImageCollectionShared::uploadRootUrl()
{
    QUrl path = uploadUrl();

    if ( path.isValid() )
    {
        path.setPath(QString::fromLatin1("/"));
        return path;
    }
    else
    {
        return QUrl::fromLocalFile( QString::fromLatin1("file:/") );
    }
}

QString ImageCollectionShared::uploadRootName()
{
   return (QString::fromLatin1("Images"));   // No i18n here. This must be done on KIPI host interface.
}

bool ImageCollectionShared::isDirectory()
{
    return false;
}

QString ImageCollectionShared::comment()
{
    PrintWarningMessageFeature("AlbumsHaveComments");
    return QString();
}

QString ImageCollectionShared::category()
{
    PrintWarningMessageFeature("AlbumsHaveCategory");
    return QString();
}

QDate ImageCollectionShared::date()
{
    PrintWarningMessageFeature("AlbumsHaveCreationDate");
    return QDate();
}

bool ImageCollectionShared::operator==(ImageCollectionShared& ics)
{
    return (images() == ics.images());
}

} // namespace KIPI
