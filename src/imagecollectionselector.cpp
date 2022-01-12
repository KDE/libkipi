/*
    SPDX-FileCopyrightText: 2008-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// Local includes

#include "imagecollectionselector.h"
#include "imagecollection.h"

namespace KIPI
{

ImageCollectionSelector::ImageCollectionSelector(QWidget* const parent)
    : QWidget(parent)
{
}

ImageCollectionSelector::~ImageCollectionSelector()
{
}

QList<ImageCollection> ImageCollectionSelector::selectedImageCollections() const
{
    return QList<ImageCollection>();
}

void ImageCollectionSelector::enableVirtualCollections(bool /*flag*/)
{
}

} // namespace KIPI
