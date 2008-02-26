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

#include "uploadwidget.h"
#include "uploadwidget.moc"

namespace KIPI
{

/*!
  \class KIPI::UploadWidget
  This widget is used to specify an upload directory for new images.
*/
UploadWidget::UploadWidget(QWidget* parent)
            : QWidget(parent)
{
}

UploadWidget::~UploadWidget()
{
}

ImageCollection UploadWidget::selectedImageCollection() const
{
    return ImageCollection();
}

} // namespace KIPI
