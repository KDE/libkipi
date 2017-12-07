/** ===========================================================
 * @file
 *
 * This file is a part of KDE project
 *
 *
 * @date   2009-11-21
 * @brief  kipi host test application
 *
 * @author Copyright (C) 2009-2010 by Michael G. Hansen
 *         <a href="mailto:mike at mghansen dot de">mike at mghansen dot de</a>
 * @author Copyright (C) 2011-2018 by Gilles Caulier
 *         <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

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

    KipiImageCollectionSelector(KipiInterface* const interface, QWidget* const parent=0);
    QList<ImageCollection> selectedImageCollections() const Q_DECL_OVERRIDE;

public Q_SLOTS:

    void slotItemSelectionChanged();

private:

    KipiInterface*         m_interface;
    QListWidget*           m_listWidget;
    QList<ImageCollection> m_allAlbums;
};

} // namespace KXMLKipiCmd

#endif // __KIPIIMAGECOLLECTIONSELECTOR_H
