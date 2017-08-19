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

#ifndef __KIPIIMAGECOLLECTIONSHARED_H
#define __KIPIIMAGECOLLECTIONSHARED_H

// Libkipi includes

#include "imagecollectionshared.h"

using namespace KIPI;

namespace KXMLKipiCmd
{

class KipiImageCollectionShared : public ImageCollectionShared
{
public:

    // re-implemented inherited functions:
    KipiImageCollectionShared(const QUrl& albumPath);
    KipiImageCollectionShared(const QList<QUrl>& images);
    virtual ~KipiImageCollectionShared();

    QString     name() Q_DECL_OVERRIDE;
    QList<QUrl> images() Q_DECL_OVERRIDE;
    virtual QUrl        path();
    virtual QUrl        uploadPath();
    virtual QUrl        uploadRoot();
    bool        isDirectory() Q_DECL_OVERRIDE;

    // functions used internally:
    void addImages(const QList<QUrl>& images);
    void addImage(const QUrl& image);

private:

    QUrl        m_albumPath;
    QList<QUrl> m_images;
};

} // namespace KXMLKipiCmd

#endif // __KIPIIMAGECOLLECTIONSHARED_H
