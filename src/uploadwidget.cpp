/*
    SPDX-FileCopyrightText: 2008-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// Local includes

#include "uploadwidget.h"
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
