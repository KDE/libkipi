/* ============================================================
 *
 * This file is a part of kipi-plugins project
 * http://www.kipi-plugins.org
 *
 * Date        : 2008-02-26
 * Description : template widget to upload items in collection.
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

#ifndef KIPI_UPLOADWIDGET_H
#define KIPI_UPLOADWIDGET_H

// Qt includes.

#include <QtGui/QWidget>

// Local includes.

#include "imagecollection.h"
#include "libkipi_export.h"

namespace KIPI
{

/** Re-implement this widget in your kipi host application to use your dedicaced model/view */

class LIBKIPI_EXPORT UploadWidget : public QWidget 
{
Q_OBJECT

public:

    UploadWidget(QWidget* parent);
    virtual ~UploadWidget();

    virtual ImageCollection selectedImageCollection() const;

Q_SIGNALS:

    void selectionChanged();
};

} // namespace KIPI

#endif /* KIPI_UPLOADWIDGET_H */
