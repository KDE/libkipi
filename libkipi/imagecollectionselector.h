/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-02-22
 * Description : image collection selector
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

#ifndef IMAGECOLLECTIONSELECTOR_H
#define IMAGECOLLECTIONSELECTOR_H

// Qt includes.

#include <QtGui/QWidget>
#include <QtCore/QList>

// KIPI includes.

#include "imagecollection.h"
#include "libkipi_export.h"

namespace KIPI 
{

class LIBKIPI_EXPORT ImageCollectionSelector : public QWidget 
{

Q_OBJECT

public:

    ImageCollectionSelector(QWidget *parent=0);
    virtual ~ImageCollectionSelector();

    virtual QList<ImageCollection> selectedImageCollections() const;

signals:

    void selectionChanged();
};

} // namespace KIPI

#endif /* IMAGECOLLECTIONSELECTOR_H */
