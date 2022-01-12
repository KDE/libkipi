/*
    SPDX-FileCopyrightText: 2009-2010 Michael G. Hansen <mike at mghansen dot de>
    SPDX-FileCopyrightText: 2011-2018 Gilles Caulier <caulier dot gilles at gmail dot com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef __KIPIIMAGECOLLECTIONSELECTOR_H
#define __KIPIIMAGECOLLECTIONSELECTOR_H

// Libkipi includes

#include "imagecollection.h"
#include "imagecollectionselector.h"

class QListWidget;

using namespace KIPI;

namespace KXMLKipiCmd
{

class KipiInterface;

class KipiImageCollectionSelector : public ImageCollectionSelector
{
    Q_OBJECT

public:

    KipiImageCollectionSelector(KipiInterface* const interface, QWidget* const parent = nullptr);
    QList<ImageCollection> selectedImageCollections() const override;

public Q_SLOTS:

    void slotItemSelectionChanged();

private:

    KipiInterface*         m_interface;
    QListWidget*           m_listWidget;
    QList<ImageCollection> m_allAlbums;
};

} // namespace KXMLKipiCmd

#endif // __KIPIIMAGECOLLECTIONSELECTOR_H
