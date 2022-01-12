/*
    SPDX-FileCopyrightText: 2009-2010 Michael G. Hansen <mike at mghansen dot de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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
    ~KipiImageCollectionShared() override;

    QString     name() override;
    QList<QUrl> images() override;
    virtual QUrl        path();
    virtual QUrl        uploadPath();
    virtual QUrl        uploadRoot();
    bool        isDirectory() override;

    // functions used internally:
    void addImages(const QList<QUrl>& images);
    void addImage(const QUrl& image);

private:

    QUrl        m_albumPath;
    QList<QUrl> m_images;
};

} // namespace KXMLKipiCmd

#endif // __KIPIIMAGECOLLECTIONSHARED_H
