/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-02-22
 * Description : image collection selector template
 *
 * Copyright (C) 2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * ============================================================ */

// Local includes.

#include "imagecollectionselector.h"
#include "imagecollectionselector.moc"

namespace KIPI 
{

ImageCollectionSelector::ImageCollectionSelector(QWidget* parent)
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

} // namespace KIPI
