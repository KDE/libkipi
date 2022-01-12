/*
    SPDX-FileCopyrightText: 2008-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef IMAGECOLLECTIONSELECTOR_H
#define IMAGECOLLECTIONSELECTOR_H

// Qt includes

#include <QWidget>
#include <QList>

// KIPI includes

#include "libkipi_export.h"

namespace KIPI
{

class ImageCollection;

/**
 * @class ImageCollectionSelector imagecollectionselector.h <KIPI/ImageCollectionSelector>
 *
 * Re-implement this widget in your KIPI host application to use your dedicated model/view
 */
class LIBKIPI_EXPORT ImageCollectionSelector : public QWidget
{

Q_OBJECT

public:

    ImageCollectionSelector(QWidget* const parent = nullptr);
    ~ImageCollectionSelector() override;

    virtual QList<ImageCollection> selectedImageCollections() const;
    virtual void enableVirtualCollections(bool flag = true);

Q_SIGNALS:

    void selectionChanged();
};

} // namespace KIPI

#endif /* IMAGECOLLECTIONSELECTOR_H */
