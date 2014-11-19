/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2008-02-22
 * @brief  image collection selector template
 *
 * @author Copyright (C) 2008-2012 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
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

#ifndef IMAGECOLLECTIONSELECTOR_H
#define IMAGECOLLECTIONSELECTOR_H

// Qt includes

#include <QtGui/QWidget>
#include <QtCore/QList>

// KIPI includes

#include "libkipi_export.h"

namespace KIPI
{

class ImageCollection;

/** Re-implement this widget in your KIPI host application to use your dedicated model/view
 */
class LIBKIPI_EXPORT ImageCollectionSelector : public QWidget
{

Q_OBJECT

public:

    ImageCollectionSelector(QWidget* const parent=0);
    virtual ~ImageCollectionSelector();

    virtual QList<ImageCollection> selectedImageCollections() const;
    virtual void enableVirtualCollections(bool flag = true);

Q_SIGNALS:

    void selectionChanged();
};

} // namespace KIPI

#endif /* IMAGECOLLECTIONSELECTOR_H */
