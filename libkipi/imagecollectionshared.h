/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2004-02-01
 * @brief  image collection shared
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

#ifndef KIPI_IMAGECOLLECTIONSHARED_H
#define KIPI_IMAGECOLLECTIONSHARED_H

// Qt Includes.

#include <QtCore/QString>
#include <QtCore/QDateTime>

// KDE includes

#include <kurl.h>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

/** See ImageCollection documentation for details.
 */
class LIBKIPI_EXPORT ImageCollectionShared
{

public:

    ImageCollectionShared();
    virtual ~ImageCollectionShared();

    /** These methods must be re-implemented in your KIPI host application to manage collection attributes with plugins.
     */
    virtual KUrl::List images() = 0;
    virtual QString    name() = 0;

    virtual QString    comment();
    virtual QString    category();
    virtual QDate      date();
    virtual KUrl       path();
    virtual KUrl       uploadPath();
    virtual KUrl       uploadRoot();
    virtual QString    uploadRootName();
    virtual bool       isDirectory();

    virtual bool operator==(ImageCollectionShared&);

private:

    void addRef();
    void removeRef();

private:

    int m_count;

private:

    friend class ImageCollection;
};

} // namespace KIPI

#endif /* IMAGECOLLECTIONSHARED_H */
