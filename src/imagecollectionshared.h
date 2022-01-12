/*
    SPDX-FileCopyrightText: 2004-2018 Gilles Caulier <caulier dot gilles at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Renchi Raju <renchi dot raju at gmail dot com>
    SPDX-FileCopyrightText: 2004-2005 Jesper K. Pedersen <blackie at kde dot org>
    SPDX-FileCopyrightText: 2004-2005 Aurelien Gateau <aurelien dot gateau at free dot fr>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPI_IMAGECOLLECTIONSHARED_H
#define KIPI_IMAGECOLLECTIONSHARED_H

// Qt Includes.

#include <QString>
#include <QDateTime>
#include <QUrl>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

/**
 * @class ImageCollectionShared imagecollectionshared.h <KIPI/ImageCollectionShared>
 *
 * See ImageCollection documentation for details.
 */
class LIBKIPI_EXPORT ImageCollectionShared
{

public:

    ImageCollectionShared();
    virtual ~ImageCollectionShared();

    /** These methods must be re-implemented in your KIPI host application to manage collection attributes with plugins.
     */
    virtual QList<QUrl> images() = 0;
    virtual QString     name() = 0;

    virtual QString     comment();
    virtual QString     category();
    virtual QDate       date();
    virtual QUrl        url();
    virtual QUrl        uploadUrl();
    virtual QUrl        uploadRootUrl();
    virtual QString     uploadRootName();
    virtual bool        isDirectory();

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
