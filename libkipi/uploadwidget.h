/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date   2008-02-26
 * @brief  template widget to upload items in collection
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

#ifndef KIPI_UPLOADWIDGET_H
#define KIPI_UPLOADWIDGET_H

// Qt includes

#include <QtGui/QWidget>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

class ImageCollection;

/** Re-implement this widget in your KIPI host application to use your dedicated model/view */

class LIBKIPI_EXPORT UploadWidget : public QWidget
{
    Q_OBJECT

public:

    UploadWidget(QWidget* const parent);
    virtual ~UploadWidget();

    virtual ImageCollection selectedImageCollection() const;

Q_SIGNALS:

    void selectionChanged();
};

} // namespace KIPI

#endif /* KIPI_UPLOADWIDGET_H */
