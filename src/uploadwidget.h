/*
    SPDX-FileCopyrightText: 2008-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KIPI_UPLOADWIDGET_H
#define KIPI_UPLOADWIDGET_H

// Qt includes

#include <QWidget>

// Local includes

#include "libkipi_export.h"

namespace KIPI
{

class ImageCollection;

/**
 * @class UploadWidget uploadwidget.h <KIPI/UploadWidget>
 *
 * Re-implement this widget in your KIPI host application to use your dedicated model/view
 */
class LIBKIPI_EXPORT UploadWidget : public QWidget
{
    Q_OBJECT

public:

    UploadWidget(QWidget* const parent);
    ~UploadWidget() override;

    virtual ImageCollection selectedImageCollection() const;

Q_SIGNALS:

    void selectionChanged();
};

} // namespace KIPI

#endif /* KIPI_UPLOADWIDGET_H */
