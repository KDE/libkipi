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

#include "uploadwidget.moc"

// Local includes

#include "imagecollection.h"

namespace KIPI
{

/*!
  \class KIPI::UploadWidget
  This widget is used to specify an upload directory for new images.
*/
UploadWidget::UploadWidget(QWidget* const parent)
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
