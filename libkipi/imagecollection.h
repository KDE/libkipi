/** ===========================================================
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  image collection
 *
 * @author Copyright (C) 2004-2010 by Gilles Caulier
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

/** @file imagecollection.h */

#ifndef KIPI_IMAGECOLLECTION_H
#define KIPI_IMAGECOLLECTION_H

// Qt includes

#include <QtCore/QString>
#include <QtCore/QDateTime>

// KDE includes.

#include <kurl.h>

// Local includes.

#include "libkipi_export.h"

/** @namespace KIPI */
namespace KIPI
{

class ImageCollectionShared;

/** @class ImageCollection
    Holds info about the collection
 */
class LIBKIPI_EXPORT ImageCollection
{

public:

    QString name() const;
    QString comment() const;
    QString category() const;
    QDate date() const;
    KUrl::List images() const;
    KUrl path() const;
    KUrl uploadPath() const;
    KUrl uploadRoot() const;
    QString uploadRootName() const;
    bool isDirectory() const;
    bool isValid() const;

    // Interface for host application + general stuff
    ImageCollection( ImageCollectionShared* );
    ImageCollection( const ImageCollection& other );
    ~ImageCollection();
    ImageCollection();
    ImageCollection& operator=( const ImageCollection& );

    bool operator==(const ImageCollection&) const;

private:

    mutable ImageCollectionShared* _data;
    void printNullError() const;
};

} // namespace KIPI

#endif /* IMAGECOLLECTION_H */
